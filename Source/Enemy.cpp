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



/// @brief Enemyの初期化（コンストラクタ）
Enemy::Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2, float Serch3,int money, bool is_separate_anim_)
	:Object3D(initPos)
	, hp_(hp)
	, speed_(speed)
	, is_invisible_(false)
	, angle_(0.0f)
	, target_angle_(0.0f)
	, size_(HitSize)
	, hit_search_(Serch1)
	, hit_attack_search_(Serch2)
	, hit_stop_search_(Serch3)
	, alg_hit_(20)
	, attack_(attack)
	, no_position_(VGet(0, 0, 0))
	, is_attack_hit_judgment_flag_(false)
	, is_hit_attack_search_flag_(false)
	, is_hit_search_stop_flag_(false)
	, is_hit_attack_flag_(false)
	, is_hit_search_flag_(false)
	, is_hit_judgment_flag_player_(false)
	, walk_count_(0)
	, walk_timer_(0)
	, is_dead_(false)
	, max_hp_(hp)
	,have_money_(money)
{
	SetTag(Object3D::Tag3D_Enemy3D);
	model_ = new Model(filename, initPos, is_separate_anim_);
	model_->SetScale(VGet(1.3f, 1.3f, 1.3f));
	init_position_ = initPos;
	max_hp_ = hp_;
	normal_speed_ = speed_;

	capsule_collider_ = new CapsuleCollider(this, position_, VAdd(position_, VGet(0.0f, size_/2, 0.0f)), size_);
	attach_collider_ = new SphereCollider(this, model_->GetAttachmentPosition(), 50.0f);
	serch_collider_ = new SphereCollider(this, position_, hit_search_);
	attack_collider_ = new SphereCollider(this, position_, hit_attack_search_);
	stop_collider_ = new SphereCollider(this, position_, hit_stop_search_);


}

Enemy::~Enemy()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}

}


/// @brief Enemyの状態更新処理
void Enemy::Update()
{

		if (model_ != nullptr)
		{
			DeathEnemy();
			Attack();
			UpdateColliderPosition();
			RotationByMove();
			Move();

			model_->Update();

		}


}


/// @brief Enemyの描画処理
void Enemy::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
	if (Master::debug_->Getdebug() == true)
	{
		DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 150.0f, 0.0f)),
			size_,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
	}


}


/// @brief EnemyのAttackList処理
void Enemy::AttackList()
{


}


/// @brief EnemyのAttack処理
void Enemy::Attack()
{
	AnimationState now = model_->GetNowState();

	if (attack_count_ >= attack_interval_&& is_hit_attack_search_flag_)
	{
		Master::sound_manager_->PlaySE(SoundManager::SE_ATTACKSLIDE);
		attack_count_ = 0;
		model_->ChangeAnimation(ANIMATION_ATTACK);
		model_->SetLoop(false);
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);
		is_hit_attack_search_flag_ = false;
	}
	if (!(now == ANIMATION_ATTACK))
	{
		is_attack_hit_judgment_flag_ = false;
		attack_count_++;
	}



}





/// @brief EnemyのMove処理
void Enemy::Move()
{
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK||now==ANIMATION_ATTACKMAGIC||now==ANIMATION_ATTACKJUMP)return;
	if (Master::is_safe_point_on_)position_ = init_position_;


	Player3D* pPlayer = Master::player_;

	move_vec_ = VGet(0.0f, 0.0f, 0.0f);

	{
		if (is_hit_search_stop_flag_) { speed_ = 0; } else { speed_ = normal_speed_; }

		go_position_ = VSub(pPlayer->GetPosition(), position_);
		go_position_.y = 0.0f;
		if (VSquareSize(go_position_) > 0.0001f) go_position_ = VNorm(go_position_);
		move_vec_ = go_position_;

		bool isMove = (move_vec_.x != 0.0f || move_vec_.z != 0.0f);
		if (isMove) { model_->ChangeAnimation(ANIMATION_RUN); target_angle_ = atan2f(move_vec_.x, move_vec_.z); }
		else { model_->ChangeAnimation(ANIMATION_NEUTRAL); }

		VECTOR old_position_ = position_;
		position_ = VAdd(position_, VScale(move_vec_, speed_));

		TerrainFollow(0.0f, 150.0f, 40.0f, 150.0f, -40.0f, 4.0f);
		const auto& walls = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
		if (!walls.empty())
		{//wallの当たり判定の処理
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
						float a = VDot(VScale(move_vec_, -1.0f), vertex.at(0).norm);
						slide = VAdd(move_vec_, VScale(vertex.at(0).norm, a));

						position_ = old_position_;
						position_ = VAdd(position_, VScale(slide, speed_));
					}
				}
			}
		}




		model_->SetPosition(position_);


	}
}


/// @brief EnemyのRotationByMove処理
void Enemy::RotationByMove()
{
	float subAngle = target_angle_ - angle_;

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
	angle_ = target_angle_ - subAngle;

	rotation_.y = angle_ + DX_PI_F;
	model_->SetRotation(rotation_);


}


/// @brief EnemyのDamage処理
void Enemy::Damage(float damage)
{

	hp_ -= damage;
	if(hp_ <= 0)
	{
		if (!is_dead_ && Master::score_manager_ != nullptr) {
			Master::score_manager_->AddDefeatedEnemy();
		}
		hp_ = 0;
		is_dead_ = true;
	}
}




/// @brief EnemyのDeathEnemy処理
void Enemy::DeathEnemy()
{
	if (!is_dead_)return;

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


/// @brief EnemyのDeathColliderPosition処理
void Enemy::DeathColliderPosition()
{
	VECTOR pos = VGet(10000, 10000, 10000);
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = pos;
		capsule_collider_->position2_ = pos;
	}
	if (attach_collider_ != nullptr)
	{
		attach_collider_->position_ = pos;
	}
	if (serch_collider_ != nullptr)
	{
		serch_collider_->position_ = pos;
	}
	if (stop_collider_ != nullptr)
	{
		stop_collider_->SetDeleteFlag(true);
		stop_collider_->position_ = pos;
	}
	if (attack_collider_ != nullptr)
	{
		attack_collider_->position_ = pos;
	}

}


/// @brief EnemyのOnEnter処理
void Enemy::OnEnter(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return;
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == Tag3D_Obj)
	{

		position_ = init_position_;
	}

	if (check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::player_;
		if (player == nullptr)return;

		if (collider == serch_collider_ &&player->GetCollisionCollider()==check)
		{
			is_hit_search_flag_ = true;
		}
		if (collider == attack_collider_ && player->GetCollisionCollider() == check)
		{
			is_hit_attack_search_flag_ = true;

		}
		if (collider == stop_collider_ && player->GetCollisionCollider() == check)
		{
			is_hit_search_stop_flag_ = true;
		}
	}
}


/// @brief EnemyのOnTrigger処理
void Enemy::OnTrigger(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return;
	AnimationState now = model_->GetNowState();
	if (collider == attach_collider_ && check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::player_;
		if (player == nullptr)return;
		if (check == player->GetCollisionCollider())
		{
			if (now == ANIMATION_ATTACK && !is_attack_hit_judgment_flag_)
			{

				player->Damage(attack_);
				is_attack_hit_judgment_flag_ = true;
			}
		}
	}

}


/// @brief EnemyのOnExit処理
void Enemy::OnExit(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return;
	if (check->parent_object_->GetTag() == Tag3D_Player3D)
	{

		Player3D* player = Master::player_;
		if (player == nullptr)return;
			if (collider == serch_collider_ && player->GetCollisionCollider() == check)
			{
				is_hit_search_flag_ = false;
			}
			if (collider == attack_collider_ && player->GetCollisionCollider() == check)
			{
				is_hit_attack_search_flag_ = false;

			}
			if (collider == stop_collider_ && player->GetCollisionCollider() == check)
			{
				is_hit_search_stop_flag_ = false;
			}

	}
}


/// @brief EnemyのUpdateColliderPosition処理
void Enemy::UpdateColliderPosition()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = position_;
		capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));

	}
	if (attach_collider_ != nullptr)
	{
		attach_collider_->position_ = model_->GetAttachmentPosition();
	}
	if (serch_collider_ != nullptr)
	{
		serch_collider_->position_ = position_;
	}
	if (stop_collider_ != nullptr)
	{
		stop_collider_->position_ = VAdd(position_, VGet(0.0f, size_ / 2, 0.0f));
	}
	if (attack_collider_ != nullptr)
	{
		attack_collider_->position_ = VAdd(position_, VGet(0.0f, size_ / 2, 0.0f));
	}






}


/// @brief EnemyのDelete処理
void Enemy::Delete()
{

	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->SetDeleteFlag(true);
		capsule_collider_ = nullptr;
	}
	if (attach_collider_ != nullptr)
	{
		attach_collider_->SetDeleteFlag(true);
		attach_collider_ = nullptr;
	}
	if (serch_collider_ != nullptr)
	{
		serch_collider_->SetDeleteFlag(true);
		serch_collider_ = nullptr;
	}
	if (stop_collider_ != nullptr)
	{
		stop_collider_->SetDeleteFlag(true);
		stop_collider_ = nullptr;
	}
	if (attack_collider_ != nullptr)
	{
		attack_collider_->SetDeleteFlag(true);
		attack_collider_ = nullptr;
	}

}



/// @brief EnemyのGiveRewards処理
void Enemy::GiveRewards()
{
	Player3D* player = Master::player_;
	if (player != nullptr) {
		player->have_money_->AddMoney(have_money_);
	}
}



