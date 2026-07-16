#include"Enemy.h"
#include"Model.h"
#include"Master.h"
#include"Player3D.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Stage.h"
#include"DrawHp.h"
#include"Coin.h"


#include"Wall.h"
#include"Scene.h"
#include"Tree.h"
#include"ColliderManager.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"


Enemy::Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2, float Serch3,int money, bool isSeparateAnim)
	:Object3D(initPos)
	, hp_(hp)
	, speed_(speed)
	, mbInvisible(false)
	, mfAngle(0.0f)
	, mfTargetAngle(0.0f)
	, mfSize(HitSize)
	, HitSerch(Serch1)
	, HitAttackSerch(Serch2)
	, HitStopSerch(Serch3)
	, AlgHit(20)
	, attack_(attack)
	, NoPosition(VGet(0, 0, 0))
	, AttackHitJudgmentflag(false)
	, isHitAttackSearch(false)
	, isHitSerchStop(false)
	, isHitAttack(false)
	, isHitSearch(false)
	, HitJudgmentflag_Player(false)
	, WalkCount(0)
	, WalkTimer(0)
	, isDead(false)
	, max_hp_(hp)
	,mfHaveMoney(money)
{
	SetTag(Object3D::Tag3D_Enemy3D);
	model_ = new Model(filename, initPos, isSeparateAnim);
	model_->SetScale(VGet(1.3f, 1.3f, 1.3f));
	VinitPos = initPos;
	max_hp_ = hp_;
	mfNormalSpeed = speed_;

	mpCapsuleCollider = new CapsuleCollider(this, position_, VAdd(position_, VGet(0.0f, mfSize/2, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, model_->GetAttachmentPosition(), 50.0f);
	mpSerchCollider = new SphereCollider(this, position_, HitSerch);
	mpAttackCollider = new SphereCollider(this, position_, HitAttackSerch);
	mpStopCollider = new SphereCollider(this, position_, HitStopSerch);
	
	
}

Enemy::~Enemy()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
	
}

void Enemy::Update()
{
	
		if (model_ != nullptr)
		{
			DeathEnemy();
			Attack();
			CollPositionUpdate();
			RotationByMove();
			Move();
		
			model_->Update();
			
		}

	
}

void Enemy::Draw()
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

void Enemy::AttackList()
{
	

}

void Enemy::Attack()
{
	AnimationState now = model_->GetNowState();
	
	if (AttackCount >= AttackInterval&& isHitAttackSearch)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		AttackCount = 0;
		model_->ChangeAnimation(ANIMATION_ATTACK);
		model_->SetLoop(false);
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);
		isHitAttackSearch = false;
	}
	if (!(now == ANIMATION_ATTACK))
	{
		AttackHitJudgmentflag = false;
		AttackCount++;
	}



}




void Enemy::Move()
{
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK||now==ANIMATION_ATTACKMAGIC||now==ANIMATION_ATTACKJUMP)return;
	if (Master::SafePointOn)position_ = VinitPos;
	

	auto mpPlayer = Master::mpPlayer;
	Player3D* pPlayer = Master::mpPlayer;

	moveVec = VGet(0.0f, 0.0f, 0.0f);
	
	{
		if (isHitSerchStop) { speed_ = 0; } else { speed_ = mfNormalSpeed; }
		
		GoPosition = VSub(pPlayer->GetPosition(), position_);
		GoPosition.y = 0.0f;
		if (VSquareSize(GoPosition) > 0.0001f) GoPosition = VNorm(GoPosition);
		moveVec = GoPosition;

		bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
		if (isMove) { model_->ChangeAnimation(ANIMATION_RUN); mfTargetAngle = atan2f(moveVec.x, moveVec.z); }
		else { model_->ChangeAnimation(ANIMATION_NEUTRAL); }

		VECTOR oldPosition = position_;
		position_ = VAdd(position_, VScale(moveVec, speed_));

		VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
		hitPos = VGet(0.0f, 0.0f, 0.0f);
		bool isHit = false;
		TerrainFollow(0.0f, 150.0f, 40.0f, 150.0f, -40.0f, 4.0f);
		const auto& walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
		if (!walls.empty())
		{
			for (int i = 0; i < walls.size(); i++)
			{
				Wall* wall = walls.at(i)->CastTo<Wall>();
				if (wall != nullptr)
				{
					std::vector<VERTEX3D> vertex = wall->GetVertex();

					if (HitCheck_Capsule_Triangle(
						position_,
						VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
						HitCheck_Capsule_Triangle(
							position_,
							VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
							80.0f,
							vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
						)
					{



						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);
						float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
						slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

						position_ = oldPosition;
						position_ = VAdd(position_, VScale(slide, speed_));
					}
				}
			}
		}




		model_->SetPosition(position_);


	}
}

void Enemy::RotationByMove()
{
	float subAngle = mfTargetAngle - mfAngle;

	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	if (subAngle > 0.0f)
	{
		subAngle -= ROTATE_SPEED;
		if (subAngle < 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	else if (subAngle < 0.0f)
	{
		subAngle += ROTATE_SPEED;
		if (subAngle > 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	mfAngle = mfTargetAngle - subAngle;

	rotation_.y = mfAngle + DX_PI_F;
	model_->SetRotation(rotation_);


}

void Enemy::Damage(float damage)
{

	hp_ -= damage;
	if(hp_ <= 0)
	{
		if (!isDead && Master::mpScoreManager != nullptr) {
			Master::mpScoreManager->AddDefeatedEnemy();
		}
		hp_ = 0;
		isDead = true;
	}
}



void Enemy::DeathEnemy()
{
	if (!isDead)return;
	
	model_->ChangeAnimation(ANIMATION_DYING);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_MAX);
	
	Delete();
	if (model_->IsAnimationLoopFinish())
	{
		GiveRewards();
		
		
		SetDeleteFlag(true);
	}
	
	model_->Update();
	
	
}

void Enemy::DeathColliderPosition()
{
	VECTOR pos = VGet(10000, 10000, 10000);
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->position_ = pos;
		mpCapsuleCollider->position2_ = pos;
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->position_ = pos;
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->position_ = pos;
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->SetDeleteFlag(true);
		mpStopCollider->position_ = pos;
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->position_ = pos;
	}

}

void Enemy::OnEnter(Collider* collider, Collider* check)
{
	auto mpPlayer = Master::mpPlayer;
	if (hp_ <= 0)return;
	if (collider == mpCapsuleCollider && check->parent_object_->GetTag() == Tag3D_Obj)
	{
		
		position_ = VinitPos;
	}
	
	if (check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::mpPlayer;
		if (player == nullptr)return;
	
		if (collider == mpSerchCollider &&player->GetCollisionCollider()==check)
		{
			isHitSearch = true;
		}
		if (collider == mpAttackCollider && player->GetCollisionCollider() == check)
		{
			isHitAttackSearch = true;

		}
		if (collider == mpStopCollider && player->GetCollisionCollider() == check)
		{
			isHitSerchStop = true;
		}
	}
}

void Enemy::OnTrigger(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = model_->GetNowState();
	if (collider == mpAttachCollider && check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::mpPlayer;
		if (player == nullptr)return;
		if (check == player->GetCollisionCollider())
		{
			if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
			{

				player->Damage(attack_);
				AttackHitJudgmentflag = true;
			}
		}
	}
	
}

void Enemy::OnExit(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = model_->GetNowState();
	if (check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		
		Player3D* player = Master::mpPlayer;
		if (player == nullptr)return;
		if (collider == mpAttachCollider && player->GetCollisionCollider() == check)
			{

			}
			if (collider == mpSerchCollider && player->GetCollisionCollider() == check)
			{
				isHitSearch = false;
			}
			if (collider == mpAttackCollider && player->GetCollisionCollider() == check)
			{
				isHitAttackSearch = false;

			}
			if (collider == mpStopCollider && player->GetCollisionCollider() == check)
			{
				isHitSerchStop = false;
			}
		
	}
}

void Enemy::CollPositionUpdate()
{
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->position_ = position_;
		mpCapsuleCollider->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
		
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->position_ = model_->GetAttachmentPosition();
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->position_ = position_;
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->position_ = VAdd(position_, VGet(0.0f, mfSize / 2, 0.0f));
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->position_ = VAdd(position_, VGet(0.0f, mfSize / 2, 0.0f));
	}
	
	


	
	
}

void Enemy::Delete()
{
	
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->SetDeleteFlag(true);
		mpCapsuleCollider = nullptr;
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->SetDeleteFlag(true);
		mpAttachCollider = nullptr;
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->SetDeleteFlag(true);
		mpSerchCollider = nullptr;
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->SetDeleteFlag(true);
		mpStopCollider = nullptr;
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->SetDeleteFlag(true);
		mpAttackCollider = nullptr;
	}
	
}


void Enemy::GiveRewards()
{
	auto mpPlayer = Master::mpPlayer;
	Player3D* player = Master::mpPlayer;
	if (player != nullptr) {
		player->have_money_->AddMoney(mfHaveMoney);
	}
}



