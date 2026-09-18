#include "EnemyMonster.h"
#include "Model.h"
#include "Master.h"
#include "Player3D.h"
#include "ObjectManager.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "Magic_Ene.h"
#include "CapsuleCollider.h"

/// @brief EnemyMonsterクラスのコンストラクタ
/// @param filename 使用する3Dモデルのファイルパス
/// @param initPos 初期配置座標
/// @param hp 初期・最大体力値
/// @param speed 移動速度
/// @param HitSize カプセルコライダーの判定サイズ
/// @param Serch1 索敵用範囲半径
/// @param Serch2 攻撃開始用範囲半径
/// @param Serch3 接近停止用範囲半径
/// @param money 倒した際に獲得できる資金
/// @param is_separate_anim_ アニメーションを分離読み込みするかどうかのフラグ
/// @details 各種戦闘パラメータ（攻撃間隔等）の初期設定および着地判定用コライダーの生成を行う
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
	chance_ = 20;
	attack_interval_ = 120; // 戦闘のテンポを担保するため、ジャンプ攻撃のクールダウンを2秒(120f)に設定
	attack_count_ = 0;
	SetTag(Object3D::Tag3D_Enemy3D);

	if (model_)
	{
		model_->SetScale(VGet(3.0f, 3.0f, 3.0f));
		model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/model/character/11_idle.mv1");
		model_->AddAnimation(ANIMATION_RUN, "Resource/model/character/12_run.mv1");
		model_->AddAnimation(ANIMATION_DYING, "Resource/model/character/13_die.mv1");
		model_->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/model/character/17_jump_attack.mv1");
	}

	// レベルデザイン：着地攻撃は範囲が広いため、プレイヤーが回避行動をとるための十分な視覚的猶予を持たせる大きな半径で設定
	landing_attack_collider_ = new SphereCollider(this, position_, 800.0f);
}

/// @brief EnemyMonsterクラスのデストラクタ
EnemyMonster::~EnemyMonster()
{
}

/// @brief 毎フレームの状態更新処理を行う
/// @details 死亡時の演出進行、または攻撃ステートに応じた移動処理とモデル更新の同期を行う
void EnemyMonster::Update()
{
	if (is_dead_)
	{
		DeathEnemy();
	}
	else if (model_ != nullptr)
	{
		Attack();

		// 設計ルール：ジャンプ攻撃中は重力と放物線移動で座標が更新されるため、通常移動（Move）を排他制御して動きの破綻を防止
		if (attack_state_ == AttackState::None)
		{
			RotationByMove();
			Move();
		}

		model_->Update();
		model_->SetPosition(position_);
		UpdateColliderPosition();
		landing_attack_collider_->position_ = position_;
	}
}

/// @brief 3Dモデルの描画処理を行う
/// @details モデルの描画。デバッグ時のみ物理コライダーと攻撃判定範囲を可視化する
void EnemyMonster::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
}

/// @brief 現在の攻撃ステートに基づいた各フェーズ更新メソッドの実行
void EnemyMonster::Attack()
{
	switch (attack_state_)
	{
	case AttackState::None:     UpdateAttackIdle();     break;
	case AttackState::Charging: UpdateAttackCharging(); break;
	case AttackState::Jumping:  UpdateAttackJumping();  break;
	case AttackState::Landing:  UpdateAttackLanding();  break;
	}
}

/// @brief 待機（攻撃準備可能）状態の更新処理
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

/// @brief 攻撃の溜め（予兆演出）状態の更新処理
void EnemyMonster::UpdateAttackCharging()
{
	charge_timer_++;
	target_angle_ = atan2f(go_position_.x, go_position_.z);
	RotationByMove();
	model_->ChangeAnimation(ANIMATION_ATTACKJUMP);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_NEUTRAL);

	// UX仕様：プレイヤーに「攻撃が来る」という予兆を認識させ、回避行動の準備期間として30フレームの硬直を設ける
	if (charge_timer_ > 30)
	{
		StartJumpAttack();
	}
}

/// @brief ジャンプ滞空（空中移動・重力計算）状態の更新処理
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

		new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(position_, VGet(0.0f, 50.0f, 0.0f)), 50.0f, 5, 30.0f, VGet(0, 0, 0), 0, 150);
	}
}

/// @brief 着地硬直・範囲判定発生状態の更新処理
void EnemyMonster::UpdateAttackLanding()
{
	charge_timer_++;
	if (charge_timer_ > 30)
	{
		attack_state_ = AttackState::None;
		is_attack_hit_judgment_flag_ = false;
	}
}

/// @brief プレイヤーがジャンプ攻撃の射程範囲内にいるか判定する
/// @return bool 射程内であればtrue
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

/// @brief プレイヤーの方向に向けてジャンプベクトルを設定する
void EnemyMonster::SetJumpDirectionToPlayer()
{
	VECTOR toPlayer = go_position_;
	toPlayer.y = 0.0f;
	jump_target_dir_ = VSquareSize(toPlayer) > 0.0f ? VNorm(toPlayer) : VGet(0, 0, 1);
}

/// @brief ジャンプ攻撃の物理パラメータ（初速、方向、滞空時間）を計算して開始する
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

/// @brief 接触判定の継続処理
/// @param collider 自身のコライダー
/// @param check 相手のコライダー
/// @details 着地攻撃時にプレイヤーとの接触を確認し、ダメージ（2倍補正）を適用して被弾フラグを立てる
void EnemyMonster::OnTrigger(Collider* collider, Collider* check)
{
	if (hp_ <= 0) return;

	Player3D* pPlayer = Master::player_;
	if (pPlayer == nullptr) return;

	if (attack_state_ == AttackState::Landing && !has_landed_hit_)
	{
		if (collider == landing_attack_collider_ && check == pPlayer->GetCollisionCollider())
		{
			pPlayer->Damage(attack_ * 2.0f);
			has_landed_hit_ = true;
		}
	}
	Enemy::OnTrigger(collider, check);
}

/// @brief 死亡時の処理を行う
/// @details 死亡アニメーションの再生と、完了後の報酬付与・オブジェクト削除を行う
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
	model_->Update();
}

/// @brief オブジェクト破棄時の解放処理を行う
/// @details 着地攻撃用コライダーの明示的な破棄を行う
void EnemyMonster::Delete()
{
	Enemy::Delete();
	if (landing_attack_collider_ != nullptr)
	{
		landing_attack_collider_->SetDeleteFlag(true);
		landing_attack_collider_ = nullptr;
	}
}