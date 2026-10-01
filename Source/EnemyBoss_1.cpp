#include"EnemyBoss_1.h"
#include"Model.h"
#include"Master.h"
#include"Player3D.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"GameScene.h"
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

/// @param 初期化パラメータ（位置、HP、速度、当たり判定サイズ、探索範囲、所持金など）
/// @details 3Dモデル、アニメーション、当たり判定用のメモリ確保と設定
EnemyBoss_1::EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool m_IsSeparateAnim)
	:Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3, money, m_IsSeparateAnim)
{
	mfjumpPower = 150.0f;       // ジャンプ攻撃の最大到達高度
	HighPositionFlag = false;   // ジャンプの頂点到達状態の管理
	m_AttackType = BossAttackType::kCombo;           // 現在の攻撃パターンの種類
	m_Attack1ComboCount = 0;   // 騾｣邯夐ｭ疲ｳ墓判謦・・谿九ｊ逋ｺ蜍募屓謨ｰ
	m_Chance = kAttackChanceThreshold;               // 攻撃頻度の重み付けパラメータ
	m_AttackInterval = 60;      // 連続攻撃を防ぐためのクールタイム（フレーム）
	m_AttackCount = 0;          // クールタイム計測用カウンタ
	m_JumpChargeTimer = 0;
	m_JumpVelocity = 0.0f;
	m_Gravity = 4.0f;

	SetTag(Object3D::Tag3D_Enemy3D);

	m_Model->AddAnimation(ANIMATION_NEUTRAL, "Resource/model/character/11_idle.mv1");
	m_Model->AddAnimation(ANIMATION_RUN, "Resource/model/character/12_run.mv1");
	m_Model->AddAnimation(ANIMATION_DYING, "Resource/model/character/13_die.mv1");
	m_Model->AddAnimation(ANIMATION_ATTACKMAGIC, "Resource/model/character/14_magic_attack.mv1");
	m_Model->AddAnimation(ANIMATION_ATTACK, "Resource/model/character/17_jump_attack.mv1");

	m_Model->SetScale(VGet(4.0f, 4.0f, 4.0f));

	m_JumpAttackCoiider = new SphereCollider(this, m_Position, 400.0f);
}

EnemyBoss_1::~EnemyBoss_1()
{

}

/// @details ボスの座標更新、攻撃判定、アニメーション進行
void EnemyBoss_1::Update()
{
	SceneGame* game = Master::m_SceneManager->GetSceneGame();
	if (game && game->m_GameManager) {
		auto phase = game->m_GameManager->GetCurrentPhase();
		// 画面遷移中に予期せぬ攻撃・座標移動が発生するバグを防ぐため処理を停止
		if (phase == GameManager::Phase::kFadeOutToBoss || phase == GameManager::Phase::kFadeInBoss) {
			return;
		}
	}

	if (m_IsDead)
	{
		DeathEnemy();
	}
	else
	{
		if (m_Model != nullptr)
		{

			Attack();
		// 攻撃モーション中の不自然な滑り移動を防ぐため座標更新を停止
			if (m_Model->GetNowState() != ANIMATION_ATTACK && m_Model->GetNowState() != ANIMATION_ATTACKJUMP)
			{
				RotationByMove();
				Move();
			}

			UpdateJumpPhysics();

		// 攻撃中などMove()が呼ばれない時もモデルの座標を物理座標に同期させる
			m_Model->SetPosition(m_Position);

			m_Model->Update();
			UpdateColliderPosition();
			m_JumpAttackCoiider->m_Position = m_Position;

		// 地面抜けバグを防ぐためのY座標の下限補正
			if (m_Position.y < m_InitPosition.y)
			{
				m_Position.y = m_InitPosition.y;
				m_Model->SetPosition(m_Position);
			}

		}
	}
}

/// @details ボスモデルとデバッグ用コライダーの画面描画
void EnemyBoss_1::Draw()
{
	if (m_Model != nullptr)
	{
		m_Model->Draw();
	}
}

/// @details 乱数による攻撃パターンの決定と魔法オブジェクトの生成
void EnemyBoss_1::Attack()
{
	AnimationState now = m_Model->GetNowState();

			// クールタイム消化済みかつターゲットを捕捉している場合のみ攻撃開始
	if (m_AttackCount >= m_AttackInterval && m_IsHitAttackSearchFlag)
	{
		m_AttackCount = 0;
		m_IsHitAttackSearchFlag = false;

		m_AttackType = static_cast<BossAttackType>(GetRand(2));

		if (m_AttackType == BossAttackType::kCombo)
		{
			m_Attack1ComboCount = 3;
		}
		else if (m_AttackType == BossAttackType::kMagic)
		{
			m_Model->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			m_Model->SetLoop(false);
			m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);

			// ボスの弾のサイズと当たり判定を1.5倍にする (50.0f -> 75.0f)
			new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(m_Position, VGet(0.0f, 100.0f, 0.0f)), kMagicScale, kMagicDamage, kMagicSpeed, m_GoPosition, 0, kMagicLifetime);
			VECTOR leftGo = VTransform(m_GoPosition, MGetRotY(-30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(m_Position, VGet(0.0f, 100.0f, 0.0f)), kMagicScale, kMagicDamage, kMagicSpeed, leftGo, 0, kMagicLifetime);
			VECTOR rightGo = VTransform(m_GoPosition, MGetRotY(30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(m_Position, VGet(0.0f, 100.0f, 0.0f)), kMagicScale, kMagicDamage, kMagicSpeed, rightGo, 0, kMagicLifetime);
		}
		else if (m_AttackType == BossAttackType::kJump)
		{
			m_Model->ChangeAnimation(ANIMATION_ATTACK);
			m_Model->SetLoop(false);
			m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			// ジャンプ開始前のタメ時間（アニメーション同期）のために初期化
			m_JumpChargeTimer = 0;
			m_ForwardSpeed = 0.0f;
			m_JumpVelocity = 0.0f;
			HighPositionFlag = false;
		}
	}

	// パターン0の場合、モーション完了に合わせて段階的に魔法を生成する仕様
	if (m_AttackType == BossAttackType::kCombo && m_Attack1ComboCount > 0)
	{
		if (now == ANIMATION_NEUTRAL || now == ANIMATION_RUN)
		{
			m_Model->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			m_Model->SetLoop(false);
			m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);

			// ボスの弾のサイズと当たり判定を1.5倍にする (100.0f -> 150.0f)
			new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(m_Position, VGet(0.0f, 100.0f, 0.0f)), 150.0f, 5, 30.0f, m_GoPosition, 0, kMagicLifetime);

			m_Attack1ComboCount--;
		}
	}

	if (!(now == ANIMATION_ATTACKMAGIC) && !(now == ANIMATION_ATTACK))
	{
		if (m_Attack1ComboCount <= 0)
		{
			m_AttackCount++;
		}
		m_IsAttackHitJudgmentFlag = false;
	}
}

/// @param collider 自身のコライダー、check 衝突対象のコライダー
/// @details プレイヤーのHP減少と、ヒット済みフラグの設定
void EnemyBoss_1::OnTrigger(Collider* collider, Collider* check)
{
	if (m_Hp <= 0)return;
	AnimationState now = m_Model->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == m_JumpAttackCoiider && check->m_ParentObject->GetTag() == Tag3D_Player3D)
		{
			Player3D* pPlayer = Master::m_Player;
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				// 多段ヒットを防ぐため、1回のジャンプ攻撃につきダメージは1度のみ
				if (now == ANIMATION_ATTACK && !m_IsAttackHitJudgmentFlag)
				{
					pPlayer->Damage(kJumpAttackDamage);
					m_IsAttackHitJudgmentFlag = true;
				}
			}
		}
	}
}

/// @details アニメーション完了後のインスタンス破棄予約、およびクリアフラグの更新
void EnemyBoss_1::DeathEnemy()
{
	m_IsDead = true;
	m_Model->ChangeAnimation(ANIMATION_DYING);
	m_Model->SetLoop(false);
	m_Model->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (m_Model->IsAnimationLoopFinish())
	{
		GiveRewards();
		// ゲーム進行管理上、ボスの討伐数をクリア条件としているためのカウントアップ
		Master::m_GameClearCount++;

		Delete();
		SetDeleteFlag(true);
	}

	m_Model->Update();
}

/// @details 保持しているコライダーのメモリ解放フラグ設定
void EnemyBoss_1::Delete()
{
	Enemy::Delete();
	// メモリリーク防止のため、動的確保した固有コライダーを破棄する
	if (m_JumpAttackCoiider != nullptr)
	{
		m_JumpAttackCoiider->SetDeleteFlag(true);
		m_JumpAttackCoiider = nullptr;
	}
}

/// @details 攻撃タイプ2時のボスのY座標および軌道計算の更新
void EnemyBoss_1::UpdateJumpPhysics()
{
	if (m_Model->GetNowState() == ANIMATION_ATTACK && m_AttackType == BossAttackType::kJump)
	{
		m_JumpChargeTimer++;
		
		// 溜め期間中（30フレーム目まで）はプレイヤーの方向を向く
		if (m_JumpChargeTimer <= kJumpChargeFrames)
		{
			VECTOR toPlayer = VSub(Master::m_Player->GetPosition(), m_Position);
			m_TargetAngle = atan2f(toPlayer.x, toPlayer.z);
			RotationByMove();
		}

		// 30フレーム目（アニメーションの溜めが終わるタイミング）でジャンプの物理パラメータを計算・設定
		if (m_JumpChargeTimer == kJumpChargeFrames)
		{
			m_JumpVelocity = kJumpInitialVelocity; // EnemyMonsterと同じ初速
			
			Player3D* pPlayer = Master::m_Player;
			float dist = 0.0f;
			if (pPlayer) {
				VECTOR toPlayer = VSub(pPlayer->GetPosition(), m_Position);
				toPlayer.y = 0.0f;
				dist = VSize(toPlayer);
				if (dist > 0.1f) {
					m_JumpTargetDir = VNorm(toPlayer);
				} else {
					m_JumpTargetDir = m_GoPosition;
				}
			} else {
				m_JumpTargetDir = m_GoPosition;
			}
			
			// ジャンプの総フレーム数 = (80 / 4) * 2 = 40フレーム
			float jump_time = (m_JumpVelocity / m_Gravity) * 2.0f;
			m_ForwardSpeed = dist / jump_time;
			
			if (m_ForwardSpeed > 60.0f) {
				m_ForwardSpeed = 60.0f;
			}
		}
		
		// 30フレーム目以降から実際の移動を開始
		if (m_JumpChargeTimer > 30)
		{
			m_Position.y += m_JumpVelocity;
			m_Position.x += m_JumpTargetDir.x * m_ForwardSpeed;
			m_Position.z += m_JumpTargetDir.z * m_ForwardSpeed;
			m_JumpVelocity -= m_Gravity;

			// 逹蝨ｰ蛻､螳・
			if (m_Position.y <= m_InitPosition.y)
			{
				if (m_JumpVelocity < 0.0f) {
					Master::m_SoundManager->PlaySE(SoundManager::SE_BOSS_JUMP);
				}
				m_Position.y = m_InitPosition.y;
				// 着地したら横滑り（水平移動）を停止
				m_ForwardSpeed = 0.0f;
				m_JumpVelocity = 0.0f;
			}
		}
	}
	else
	{
		m_JumpChargeTimer = 0;
	}
}

