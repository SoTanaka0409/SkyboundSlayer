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
/// @param m_IsSeparateAnim アニメーションを分離読み込みするかどうかのフラグ
/// @details 各種戦闘パラメータ（攻撃間隔等）の初期設定および着地判定用コライダーの生成を行う
EnemyMonster::EnemyMonster(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool m_IsSeparateAnim)
	: Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3, money, m_IsSeparateAnim)
	, m_AttackState(AttackState::None)
	, m_ChargeTimer(0)
	, m_JumpTimer(0)
	, m_HasLandedHit(false)
	, m_JumpVelocity(0.0f)
	, m_Gravity(4.0f)
	, m_ForwardSpeed(20.0f)
{
	m_Chance = 20;
	m_AttackInterval = 120; // 戦闘のテンポを担保するため、ジャンプ攻撃のクールダウンを2秒(120f)に設定
	m_AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);

	if (m_Model)
	{
		m_Model->SetScale(VGet(3.0f, 3.0f, 3.0f));
		m_Model->AddAnimation(ANIMATION_NEUTRAL, "Resource/model/character/11_idle.mv1");
		m_Model->AddAnimation(ANIMATION_RUN, "Resource/model/character/12_run.mv1");
		m_Model->AddAnimation(ANIMATION_DYING, "Resource/model/character/13_die.mv1");
		m_Model->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/model/character/17_jump_attack.mv1");
	}

	// レベルデザイン：着地攻撃は範囲が広いため、プレイヤーが回避行動をとるための十分な視覚的猶予を持たせる大きな半径で設定
	m_LandingAttackCollider = new SphereCollider(this, m_Position, 800.0f);
}

/// @brief EnemyMonsterクラスのデストラクタ
EnemyMonster::~EnemyMonster()
{
}

/// @brief 毎フレームの状態更新処理を行う
/// @details 死亡時の演出進行、または攻撃ステートに応じた移動処理とモデル更新の同期を行う
void EnemyMonster::Update()
{
	if (m_IsDead)
	{
		DeathEnemy();
	}
	else if (m_Model != nullptr)
	{
		Attack();

		// 設計ルール：ジャンプ攻撃中は重力と放物線移動で座標が更新されるため、通常移動（Move）を排他制御して動きの破綻を防止
		if (m_AttackState == AttackState::None)
		{
			RotationByMove();
			Move();
		}

		m_Model->Update();
		m_Model->SetPosition(m_Position);
		UpdateColliderPosition();
		m_LandingAttackCollider->m_Position = m_Position;
	}
}

/// @brief 3Dモデルの描画処理を行う
/// @details モデルの描画。デバッグ時のみ物理コライダーと攻撃判定範囲を可視化する
void EnemyMonster::Draw()
{
	if (m_Model != nullptr)
	{
		m_Model->Draw();
	}
}

/// @brief 現在の攻撃ステートに基づいた各フェーズ更新メソッドの実行
void EnemyMonster::Attack()
{
	switch (m_AttackState)
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
	if (m_AttackCount >= m_AttackInterval && IsPlayerInJumpRange())
	{
		m_AttackState = AttackState::Charging;
		m_ChargeTimer = 0;
		m_AttackCount = 0;
		m_HasLandedHit = false;
		SetJumpDirectionToPlayer();
		return;
	}
	m_AttackCount++;
}

/// @brief 攻撃の溜め（予兆演出）状態の更新処理
void EnemyMonster::UpdateAttackCharging()
{
	m_ChargeTimer++;
	m_TargetAngle = atan2f(m_GoPosition.x, m_GoPosition.z);
	RotationByMove();
	m_Model->ChangeAnimation(ANIMATION_ATTACKJUMP);
	m_Model->SetLoop(false);
	m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);

	// UX仕様：プレイヤーに「攻撃が来る」という予兆を認識させ、回避行動の準備期間として30フレームの硬直を設ける
	if (m_ChargeTimer > 30)
	{
		StartJumpAttack();
	}
}

/// @brief ジャンプ滞空（空中移動・重力計算）状態の更新処理
void EnemyMonster::UpdateAttackJumping()
{
	m_Position.y += m_JumpVelocity;
	m_Position.x += m_JumpTargetDir.x * m_ForwardSpeed;
	m_Position.z += m_JumpTargetDir.z * m_ForwardSpeed;
	m_JumpVelocity -= m_Gravity;

	if (m_Position.y <= m_JumpStartY)
	{
		m_Position.y = m_JumpStartY;
		m_AttackState = AttackState::Landing;
		m_ChargeTimer = 0;

		new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(m_Position, VGet(0.0f, 50.0f, 0.0f)), 50.0f, 5, 30.0f, VGet(0, 0, 0), 0, 150);
	}
}

/// @brief 着地硬直・範囲判定発生状態の更新処理
void EnemyMonster::UpdateAttackLanding()
{
	m_ChargeTimer++;
	if (m_ChargeTimer > 30)
	{
		m_AttackState = AttackState::None;
		m_IsAttackHitJudgmentFlag = false;
	}
}

/// @brief プレイヤーがジャンプ攻撃の射程範囲内にいるか判定する
/// @return bool 射程内であればtrue
bool EnemyMonster::IsPlayerInJumpRange() const
{
	const float jumpTime = (80.0f / m_Gravity) * 2.0f;
	const float maxJumpDistance = jumpTime * 20.0f;

	auto playerObj = Master::m_Player;
	if (playerObj == nullptr) return false;

	VECTOR toPlayer = VSub(playerObj->GetPosition(), m_Position);
	toPlayer.y = 0.0f;
	return VSquareSize(toPlayer) <= maxJumpDistance * maxJumpDistance;
}

/// @brief プレイヤーの方向に向けてジャンプベクトルを設定する
void EnemyMonster::SetJumpDirectionToPlayer()
{
	VECTOR toPlayer = m_GoPosition;
	toPlayer.y = 0.0f;
	m_JumpTargetDir = VSquareSize(toPlayer) > 0.0f ? VNorm(toPlayer) : VGet(0, 0, 1);
}

/// @brief ジャンプ攻撃の物理パラメータ（初速、方向、滞空時間）を計算して開始する
void EnemyMonster::StartJumpAttack()
{
	m_AttackState = AttackState::Jumping;
	m_JumpVelocity = 80.0f;
	m_JumpStartY = m_Position.y;

	auto playerObj = Master::m_Player;
	if (playerObj == nullptr)
	{
		m_ForwardSpeed = 20.0f;
		return;
	}

	VECTOR toPlayer = VSub(playerObj->GetPosition(), m_Position);
	toPlayer.y = 0.0f;
	const float dist = VSize(toPlayer);
	m_JumpTargetDir = dist > 0.0f ? VNorm(toPlayer) : VGet(0, 0, 1);

	const float jumpTime = (m_JumpVelocity / m_Gravity) * 2.0f;
	m_ForwardSpeed = dist / jumpTime;
}

/// @brief 接触判定の継続処理
/// @param collider 自身のコライダー
/// @param check 相手のコライダー
/// @details 着地攻撃時にプレイヤーとの接触を確認し、ダメージ（2倍補正）を適用して被弾フラグを立てる
void EnemyMonster::OnTrigger(Collider* collider, Collider* check)
{
	if (m_Hp <= 0) return;

	Player3D* pPlayer = Master::m_Player;
	if (pPlayer == nullptr) return;

	if (m_AttackState == AttackState::Landing && !m_HasLandedHit)
	{
		if (collider == m_LandingAttackCollider && check == pPlayer->GetCollisionCollider())
		{
			pPlayer->Damage(m_Attack * 2.0f);
			m_HasLandedHit = true;
		}
	}
	Enemy::OnTrigger(collider, check);
}

/// @brief 死亡時の処理を行う
/// @details 死亡アニメーションの再生と、完了後の報酬付与・オブジェクト削除を行う
void EnemyMonster::DeathEnemy()
{
	m_IsDead = true;
	m_Model->ChangeAnimation(ANIMATION_DYING);
	m_Model->SetLoop(false);
	m_Model->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (m_Model->IsAnimationLoopFinish())
	{
		GiveRewards();
		Delete();
		SetDeleteFlag(true);
	}
	m_Model->Update();
}

/// @brief オブジェクト破棄時の解放処理を行う
/// @details 着地攻撃用コライダーの明示的な破棄を行う
void EnemyMonster::Delete()
{
	Enemy::Delete();
	if (m_LandingAttackCollider != nullptr)
	{
		m_LandingAttackCollider->SetDeleteFlag(true);
		m_LandingAttackCollider = nullptr;
	}
}