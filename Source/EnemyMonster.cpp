#include "EnemyMonster.h"
#include "Model.h"
#include "Master.h"
#include "Player3D.h"
#include "ObjectManager.h"
#include "Scene3D.h"
#include "SceneManager.h"
#include "Magic_Ene.h"
#include "CapsuleCollider.h"


/*
 * 目的（EnemyMonsterのEnemyMonster処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
EnemyMonster::EnemyMonster(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_)
	: Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3, money, is_separate_anim_)
	, attack_state_(AttackState::None)
	, charge_timer_(0)
	, jump_timer_(0)
	, has_landed_hit_(false)
	, jump_velocity_(0.0f)
	, gravity_(4.0f)
	, forward_speed_(20.0f)
{
	chance_ = 20; // Drop chance
	attack_interval_ = 120; // 2 seconds between jump attacks
	attack_count_ = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	
	// モデルが対応していればアニメーションを追加
	if (model_) {
		model_->SetScale(VGet(3.0f, 3.0f, 3.0f)); // Make it a bit large
		
		model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
		model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
		model_->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
		model_->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/Model/Jump Attack.mv1");
	}

	// Landing attack collider (large radius)
	landing_attack_collider_ = new SphereCollider(this, position_, 800.0f);
}

EnemyMonster::~EnemyMonster()
{
	// landing_attack_collider_ is deleted by Object3D/ColliderManager automatically
}


/*
 * 目的（EnemyMonsterのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyMonster::Update()
{
	AnimationState  state = model_->GetNowState();
	if (is_dead_)
	{
		DeathEnemy();
	}
	else
	{
		if (model_ != nullptr)
		{
			Attack();
			
			// ジャンプ攻撃中でない場合のみ通常移動
			if (attack_state_ == AttackState::None)
			{
				RotationByMove();
				Move();
			}

			model_->Update();
			model_->SetPosition(position_);
			UpdateColliderPosition();
			landing_attack_collider_->position_ = position_; // Update collider position
		}
	}
}


/*
 * 目的（EnemyMonsterのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyMonster::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}

	// デバッグ描画
	if (Master::debug_->Getdebug() == true)
	{
		DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 150.0f, 0.0f)),
			size_,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
		// ジャンプ中の攻撃範囲を描画
		if (attack_state_ == AttackState::Jumping || attack_state_ == AttackState::Landing) {
			DrawSphere3D(position_, 300.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
		}
	}
}


/*
 * 目的（EnemyMonsterのAttack処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyMonster::Attack()
{
	switch (attack_state_)
	{
	case AttackState::None:
		UpdateAttackIdle();
		break;
	case AttackState::Charging:
		UpdateAttackCharging();
		break;
	case AttackState::Jumping:
		UpdateAttackJumping();
		break;
	case AttackState::Landing:
		UpdateAttackLanding();
		break;
	}
}

void EnemyMonster::UpdateAttackIdle()
{
	if (attack_count_ >= attack_interval_ && IsPlayerInJumpRange())
	{
		attack_state_ = AttackState::Charging;
		charge_timer_ = 0;
		attack_count_ = 0;
		has_landed_hit_ = false;
		SetJumpDirectionToPlayer();
		return;
	}

	attack_count_++;
}

void EnemyMonster::UpdateAttackCharging()
{
	charge_timer_++;

	// チャージ中はプレイヤーを狙う
	target_angle_ = atan2f(go_position_.x, go_position_.z);
	RotationByMove();
	model_->ChangeAnimation(ANIMATION_ATTACKJUMP);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_NEUTRAL);

	if (charge_timer_ > 30)
	{
		StartJumpAttack();
	}
}

void EnemyMonster::UpdateAttackJumping()
{
	position_.y += jump_velocity_;
	position_.x += jump_target_dir_.x * forward_speed_;
	position_.z += jump_target_dir_.z * forward_speed_;
	jump_velocity_ -= gravity_;

	if (position_.y <= jump_start_y_)
	{
		position_.y = jump_start_y_;
		attack_state_ = AttackState::Landing;
		charge_timer_ = 0;

		new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 50.0f, 0.0f)), 50.0f, 5, 30.0f, VGet(0,0,0), 0, 150);
	}
}

void EnemyMonster::UpdateAttackLanding()
{
	charge_timer_++;
	if (charge_timer_ > 30)
	{
		attack_state_ = AttackState::None;
		is_attack_hit_judgment_flag_ = false;
	}
}

bool EnemyMonster::IsPlayerInJumpRange() const
{
	const float jumpTime = (80.0f / gravity_) * 2.0f;
	const float maxJumpDistance = jumpTime * 20.0f;

	auto playerObj = Master::player_;
	if (playerObj == nullptr) return false;

	VECTOR toPlayer = VSub(playerObj->GetPosition(), position_);
	toPlayer.y = 0.0f;
	return VSquareSize(toPlayer) <= maxJumpDistance * maxJumpDistance;
}

void EnemyMonster::SetJumpDirectionToPlayer()
{
	VECTOR toPlayer = go_position_;
	toPlayer.y = 0.0f;
	jump_target_dir_ = VSquareSize(toPlayer) > 0.0f ? VNorm(toPlayer) : VGet(0, 0, 1);
}

void EnemyMonster::StartJumpAttack()
{
	attack_state_ = AttackState::Jumping;
	jump_velocity_ = 80.0f;
	jump_start_y_ = position_.y;

	auto playerObj = Master::player_;
	if (playerObj == nullptr)
	{
		forward_speed_ = 20.0f;
		return;
	}

	VECTOR toPlayer = VSub(playerObj->GetPosition(), position_);
	toPlayer.y = 0.0f;
	const float dist = VSize(toPlayer);
	jump_target_dir_ = dist > 0.0f ? VNorm(toPlayer) : VGet(0, 0, 1);

	const float jumpTime = (jump_velocity_ / gravity_) * 2.0f;
	forward_speed_ = dist / jumpTime;
}


/*
 * 目的（EnemyMonsterのOnTrigger処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyMonster::OnTrigger(Collider* collider, Collider* check)
{
	if (hp_ <= 0) return;

	auto player_ = Master::player_;
	if (player_ == nullptr) return;
	Player3D* pPlayer = Master::player_;
	if (pPlayer == nullptr) return;

	if (attack_state_ == AttackState::Landing && !has_landed_hit_)
	{
		if (collider == landing_attack_collider_ && check == pPlayer->GetCollisionCollider())
		{
			pPlayer->Damage(attack_ * 2.0f); // Landing attack deals 2x damage
			has_landed_hit_ = true;
		}
	}

	// 通常の当たり判定のため基底クラスを呼び出し
	Enemy::OnTrigger(collider, check);
}


/*
 * 目的（EnemyMonsterのDeathEnemy処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyMonster::DeathEnemy()
{
	is_dead_ = true;
	model_->ChangeAnimation(ANIMATION_DYING);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (model_->IsAnimationLoopFinish())
	{
		GiveRewards();
		Delete();
		SetDeleteFlag(true);
	}
	
	//model_->Draw();
	model_->Update();
}


/*
 * 目的（EnemyMonsterのDelete処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyMonster::Delete()
{
	Enemy::Delete();
	if (landing_attack_collider_ != nullptr)
	{
		landing_attack_collider_->SetDeleteFlag(true);
		landing_attack_collider_ = nullptr;
	}
}

