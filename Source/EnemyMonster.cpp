#include "EnemyMonster.h"
#include "Model.h"
#include "Master.h"
#include "Player3D.h"
#include "ObjectManager.h"
#include "Scene3D.h"
#include "SceneManager.h"
#include "Magic_Ene.h"
#include "CapsuleCollider.h"

EnemyMonster::EnemyMonster(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, float xp, int money, bool isSeparateAnim)
	: Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3, xp, money, isSeparateAnim)
	, mAttackState(AttackState::None)
	, mChargeTimer(0)
	, mJumpTimer(0)
	, mHasLandedHit(false)
	, mJumpVelocity(0.0f)
	, mGravity(4.0f)
	, mForwardSpeed(20.0f)
{
	mnChance = 20; // Drop chance
	AttackInterval = 120; // 2 seconds between jump attacks
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	//"C:\Users\student\Desktop\LevelMonster\Resource\Model\monster.mv1"
	// Setup model and animations if needed (assuming "T.mv1" or passed filename)
	// Add animations if the model supports them
	if (mpModel) {
		mpModel->SetScale(VGet(3.0f, 3.0f, 3.0f)); // Make it a bit large
		
		mpModel->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
		mpModel->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
		mpModel->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
		mpModel->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/Model/Jump Attack.mv1");
	}

	// Landing attack collider (large radius)
	mpLandingAttackCollider = new SphereCollider(this, mvPosition, 800.0f);
}

EnemyMonster::~EnemyMonster()
{
	// mpLandingAttackCollider is deleted by Object3D/ColliderManager automatically
}

void EnemyMonster::Update()
{
	AnimationState  state = mpModel->GetNowState();
	if (isDead)
	{
		DeathEnemy();
	}
	else
	{
		if (mpModel != nullptr)
		{
			Attack();
			
			// Only move normally if not currently in a jump attack sequence
			if (mAttackState == AttackState::None)
			{
				RotationByMove();
				Move();
			}

			mpModel->Update();
			mpModel->SetPosition(mvPosition);
			CollPositionUpdate();
			mpLandingAttackCollider->mvPosition = mvPosition; // Update collider position
		}
	}
}

void EnemyMonster::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}

	// Debug draw
	if (Master::mpDebug->Getdebug() == true)
	{
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
		// Draw attack radius if jumping
		if (mAttackState == AttackState::Jumping || mAttackState == AttackState::Landing) {
			DrawSphere3D(mvPosition, 300.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
		}
	}
}

void EnemyMonster::Attack()
{
	if (mAttackState == AttackState::None)
	{
		// Calculate the exact fixed jump distance
		float jumpTime = (80.0f / mGravity) * 2.0f; // 40.0f frames
		float maxJumpDistance = jumpTime * 20.0f; // 800.0f
		
		auto playerObj = Master::mpPlayer;
		bool isPlayerInJumpRange = false;
		if (playerObj) {
			VECTOR playerPos = playerObj->GetPosition();
			VECTOR toPlayer = VSub(playerPos, mvPosition);
			toPlayer.y = 0.0f;
			if (VSquareSize(toPlayer) <= maxJumpDistance * maxJumpDistance) {
				isPlayerInJumpRange = true;
			}
		}

		if (AttackCount >= AttackInterval && isPlayerInJumpRange)
		{
			// Start Attack
			mAttackState = AttackState::Charging;
			mChargeTimer = 0;
			AttackCount = 0;
			mHasLandedHit = false;
			

			// Determine jump direction towards the player (GoPosition is already normalized)
			VECTOR toPlayer = GoPosition;
			toPlayer.y = 0.0f;
			if (VSquareSize(toPlayer) > 0.0f) {
				mJumpTargetDir = VNorm(toPlayer);
			} else {
				mJumpTargetDir = VGet(0,0,1);
			}
		}
		else
		{
			AttackCount++;
		}
	}
	else if (mAttackState == AttackState::Charging)
	{
		mChargeTimer++;
		
		// Aim at the player while charging
		mfTargetAngle = atan2f(GoPosition.x, GoPosition.z);
		RotationByMove();
		mpModel->ChangeAnimation(ANIMATION_ATTACKJUMP);
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		// Wait for 30 frames (0.5s) to charge
		if (mChargeTimer > 30)
		{
		
			mAttackState = AttackState::Jumping;
			mJumpVelocity = 80.0f; // Initial upward velocity
			mJumpStartY = mvPosition.y; // Record start height

			// Dynamically adjust forward speed so the landing point is exactly the player
			auto playerObj = Master::mpPlayer;
			if (playerObj) {
				VECTOR playerPos = playerObj->GetPosition();
				VECTOR toPlayer = VSub(playerPos, mvPosition);
				toPlayer.y = 0.0f;
				float dist = VSize(toPlayer);
				if (dist > 0.0f) {
					mJumpTargetDir = VNorm(toPlayer);
				} else {
					mJumpTargetDir = VGet(0,0,1);
				}
				float jumpTime = (mJumpVelocity / mGravity) * 2.0f;
				mForwardSpeed = dist / jumpTime;
			} else {
				mForwardSpeed = 20.0f; // Fallback
			}
		}
	}
	else if (mAttackState == AttackState::Jumping)
	{
		// Apply velocity
		mvPosition.y += mJumpVelocity;
		mvPosition.x += mJumpTargetDir.x * mForwardSpeed;
		mvPosition.z += mJumpTargetDir.z * mForwardSpeed;

		// Apply gravity
		mJumpVelocity -= mGravity;

		// Check landing
		if (mvPosition.y <= mJumpStartY)
		{
			mvPosition.y = mJumpStartY; // Snap to ground
			mAttackState = AttackState::Landing;
			mChargeTimer = 0;
			
			// Visual effect for landing
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 50.0f, 0.0f)), 50.0f, 5, 30.0f, VGet(0,0,0), 0, 150);
		}
	}
	else if (mAttackState == AttackState::Landing)
	{
		// Stay in landing state for a moment (recovery)
		mChargeTimer++;
		if (mChargeTimer > 30)
		{
			
			mAttackState = AttackState::None;
			AttackHitJudgmentflag = false; // Reset attack flag
		}
	}
}

void EnemyMonster::OnTrigger(Collider* collider, Collider* check)
{
	if (mfHp <= 0) return;

	auto mpPlayer = Master::mpPlayer;
	if (mpPlayer == nullptr) return;
	Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);
	if (pPlayer == nullptr) return;

	if (mAttackState == AttackState::Landing && !mHasLandedHit)
	{
		if (collider == mpLandingAttackCollider && check == pPlayer->GetCollisionCollider())
		{
			pPlayer->Damage(mfAttack * 2.0f); // Landing attack deals 2x damage
			mHasLandedHit = true;
		}
	}

	// Call base class for normal collisions
	Enemy::OnTrigger(collider, check);
}

void EnemyMonster::DeathEnemy()
{
		isDead = true;

		DeathColliderPosition();

		GiveRewards();
	
	Delete();
	SetDeleteFlag(true);
}

void EnemyMonster::Delete()
{
	Enemy::Delete();
	if (mpLandingAttackCollider != nullptr)
	{
		mpLandingAttackCollider->SetDeleteFlag(true);
	}
}

