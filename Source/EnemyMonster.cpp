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
	, mGravity(2.0f)
	, mForwardSpeed(20.0f)
{
	mnChance = 20; // Drop chance
	AttackInterval = 120; // 2 seconds between jump attacks
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);

	// Setup model and animations if needed (assuming "T.mv1" or passed filename)
	// Add animations if the model supports them
	if (mpModel) {
		mpModel->SetScale(VGet(3.0f, 3.0f, 3.0f)); // Make it a bit large
	}

	// Landing attack collider (large radius)
	mpLandingAttackCollider = new SphereCollider(this, mvPosition, 300.0f);
}

EnemyMonster::~EnemyMonster()
{
	// mpLandingAttackCollider is deleted by Object3D/ColliderManager automatically
}

void EnemyMonster::Update()
{
	if (mfHp <= 0)
	{
		DeathEnemy();
		return;
	}

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
		CollPositionUpdate();
		mpLandingAttackCollider->mvPosition = mvPosition; // Update collider position
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
		if (AttackCount >= AttackInterval && isHitAttackSearch)
		{
			// Start Attack
			mAttackState = AttackState::Charging;
			mChargeTimer = 0;
			AttackCount = 0;
			mHasLandedHit = false;

			// Determine jump direction towards the player (GoPosition)
			VECTOR toPlayer = VSub(GoPosition, mvPosition);
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
		// Wait for 30 frames (0.5s) to charge
		if (mChargeTimer > 30)
		{
			mAttackState = AttackState::Jumping;
			mJumpVelocity = 40.0f; // Initial upward velocity
			mForwardSpeed = 20.0f; // Forward speed
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
		if (mvPosition.y <= VinitPos.y)
		{
			mvPosition.y = VinitPos.y; // Snap to ground
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

	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
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
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	isDead = true;

	DeathColliderPosition();

	mpInventory->DropInventory(mnChance, mbWeapon, mbItem);
	if (player != nullptr) {
		player->mpHaveMoney->AddMoney(mfHaveMoney);
		player->mpLevelUp->AddXp(mfHaveXp);
	}
	
	Delete();
	SetDeleteFlag(true);
}
