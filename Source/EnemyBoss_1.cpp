#include"EnemyBoss_1.h"
#include"Model.h"
#include"Master.h"
#include"Player3D.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Stage.h"
#include"DrawHp.h"
#include"SceneGame.h"
#include"GameManager.h"
#include"Wall.h"
#include"Scene.h"
#include"Tree.h"
#include"Effect.h"
#include"InputManager.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Magic_Ene.h"

EnemyBoss_1::EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3,int money, bool isSeparateAnim)
	:Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3,money, isSeparateAnim)



{
	mbMagic = true;
	mfjumpPower = 150.0f;
	HighPositionFlag = false;
	mbjumpDown = false;
	mbjump = false;
	OnJumpCollider = false;
	mAttackType = 0;
	mAttack1ComboCount = 0;
	mnChance = 30;
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	model_->AddAnimation(ANIMATION_ATTACKMAGIC, "Resource/Model/MagicAttack.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Jump Attack.mv1");

	model_->SetScale(VGet(4.0f, 4.0f, 4.0f));
	
	mpJumpAttackCoiider = new SphereCollider(this, position_, 400.0f);

	mpDebug = new Debug();

	
}

EnemyBoss_1::~EnemyBoss_1()
{

}

void EnemyBoss_1::Update()
{
	SceneGame* game = Master::mpSceneManager->GetSceneGame();
	if (game && game->game_manager_) {
		auto phase = game->game_manager_->GetCurrentPhase();
		if (phase == GameManager::Phase::kFadeOutToBoss || phase == GameManager::Phase::kFadeInBoss) {
			return;
		}
	}

	if (isDead)
	{
		DeathEnemy();
	}
	else
	{
		if (model_ != nullptr)
		{
		
			Attack();
			if (!(model_->GetNowState() == ANIMATION_ATTACK) || !(model_->GetNowState() == ANIMATION_ATTACKJUMP))
			{
				RotationByMove();
				Move();
			}

			if (model_->GetNowState() == ANIMATION_ATTACK && mAttackType == 2)
			{
				if (!HighPositionFlag)
				{
					position_ = VAdd(position_, VGet(0.0f, 15.0f, 0.0f));
					if (position_.y >= VinitPos.y + mfjumpPower)
					{
						HighPositionFlag = true;
						mbjumpDown = true;
					}
				}
				else
				{
					position_ = VAdd(position_, VGet(0.0f, -25.0f, 0.0f));
				}

				if (position_.y <= VinitPos.y)
				{
					OnJumpCollider = true;
					position_.y = VinitPos.y;
				}
			}
			else
			{
				OnJumpCollider = false;
				HighPositionFlag = false;
				mbjump = false;
				mbjumpDown = false;
			}

			model_->Update();
			CollPositionUpdate();
			mpJumpAttackCoiider->position_ = position_;
			
			if (position_.y < VinitPos.y)
			{
				position_.y = VinitPos.y;
			}

		}
	}
}

void EnemyBoss_1::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
	if (Master::mpDebug->Getdebug() == true)
	{
		DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
	}


}

void EnemyBoss_1::Attack()
{
	AnimationState now = model_->GetNowState();

	if (AttackCount >= AttackInterval && isHitAttackSearch)
	{
		AttackCount = 0;
		isHitAttackSearch = false;
		
		mAttackType = GetRand(2);
		
		if (mAttackType == 0)
		{
			mAttack1ComboCount = 3;
		}
		else if (mAttackType == 1)
		{
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
			VECTOR leftGo = VTransform(GoPosition, MGetRotY(-30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, leftGo, 0, 150);
			VECTOR rightGo = VTransform(GoPosition, MGetRotY(30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, rightGo, 0, 150);
		}
		else if (mAttackType == 2)
		{
			model_->ChangeAnimation(ANIMATION_ATTACK);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			mfjumpPower = 400.0f;
			HighPositionFlag = false;
			mbjumpDown = false;
			mbjump = true;
			OnJumpCollider = false;
		}
	}

	if (mAttackType == 0 && mAttack1ComboCount > 0)
	{
		if (now == ANIMATION_NEUTRAL || now == ANIMATION_RUN)
		{
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
			
			mAttack1ComboCount--;
		}
	}

	if (!(now == ANIMATION_ATTACKMAGIC) && !(now == ANIMATION_ATTACK))
	{
		if (mAttack1ComboCount <= 0)
		{
			AttackCount++;
		}
		AttackHitJudgmentflag = false;
	}
}
void EnemyBoss_1::OnTrigger(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpJumpAttackCoiider && check->parent_object_->GetTag() == Tag3D_Player3D)
		{
			Player3D* pPlayer = Master::mpPlayer;
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
				{

					pPlayer->Damage(attack_);
					AttackHitJudgmentflag = true;
				}
			}

		}
	}
}

void EnemyBoss_1::DeathEnemy()
{
	isDead = true;
	model_->ChangeAnimation(ANIMATION_DYING);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (model_->IsAnimationLoopFinish())
	{
		GiveRewards();
		Master::GameClearCount++;
		
		Delete();
		SetDeleteFlag(true);
	}

	model_->Update();
}

void EnemyBoss_1::Delete()
{
	Enemy::Delete();
	if (mpJumpAttackCoiider != nullptr)
	{
		mpJumpAttackCoiider->SetDeleteFlag(true);
		mpJumpAttackCoiider = nullptr;
	}
}



