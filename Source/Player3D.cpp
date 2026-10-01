#include "Player3D.h"
#include "Model.h"
#include "ModelAnimation.h"
#include "Master.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "GameScene.h"
#include "Wall.h"
#include <string>
#include <iostream>
#include <cstring>
#include "EffectPool.h"
#include "Enemy3D.h"
#include "Stage.h"
#include "Camera.h"
#include "EffekseerObject.h"
#include "Tree.h"
#include "Scene.h"
#include "Debug.h"
#include "Config.h"
#include "DrawHp.h"
#include "Effect.h"
#include "DrawCircle1.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "InfClass.h"
#include "HaveMoneyClass.h"

/// @brief Player3Dクラスのコンストラクタ
/// @param filename プレイヤーモデルのファイルパス
/// @param initPos 初期配置座標
/// @param jumppower ジャンプ力
/// @param speed 移動速度
/// @param hp 初期最大HP
/// @param m_IsSeparateAnim アニメーション分離処理を行うかどうかのフラグ
/// @details プレイヤーモデルの初期化、各種アニメーションとコライダーの生成、マネージャー類への登録を行う
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool m_IsSeparateAnim)
	: Object3D(initPos)
	, m_Attack(3)
	, m_JumpAttack(5)
	, m_SlideAttack(7)
	, m_IsJumpColliderActive(false)
	, m_IsInvisible(false)
	, m_IsJumping(false)
	, m_Speed(speed)
	, JumpPower(jumppower)
	, m_Hp(hp)
	, m_MaxHp(hp)
	, m_Size(60.0f)
	, m_RideOldHp(0)
	, m_JumpPower(150.0f)
	, m_AttackSlideSpeed(20.0f)
	, m_AttackSlideCount(0)
	, m_AttackJumpCount(0)
	, m_AttackCount(0)
	, m_AttackSelectionIndex(0)
	, m_IsJumpFalling(false)
	, m_HasReachedJumpPeak(false)
	, m_IsAttackSlideTargetFound(false)
	, m_EvasionSpeed(20.0f)
	, m_IsStageOut(true)
	, m_IsDead(false)
{
	Master::m_Player = this;

	SetTag(Object3D::Tag3D_Player3D);
	m_BuffManager = new BuffManager();
	m_EquipmentManager = new EquipmentManager();
	m_ShortInventory = new ShortInventory();
	m_Model = new Model(filename, initPos, m_IsSeparateAnim);
	m_HaveMoney = new HaveMoneyClass(0);

	m_Model->AddAttachment("Resource/model/props/sword/01_sword.mv1", "mixamorig:RightHand", VGet(1.5f, -6.0f, 1.0f), VGet(-DX_PI_F / 3.0f, DX_PI_F / 6.0f, -DX_PI_F / 6.0f));
	m_Model->AddAnimation(ANIMATION_NEUTRAL, "Resource/model/character/11_idle.mv1");
	m_Model->AddAnimation(ANIMATION_RUN, "Resource/model/character/12_run.mv1");
	m_Model->AddAnimation(ANIMATION_DYING, "Resource/model/character/13_die.mv1");
	m_Model->AddAnimation(ANIMATION_ATTACK, "Resource/model/character/16_normal_attack.mv1");
	m_Model->AddAnimation(ANIMATION_ATTACKSLIDE, "Resource/model/character/15_attack_1.mv1");
	m_Model->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/model/character/17_jump_attack.mv1");
	m_Model->AddAnimation(ANIMATION_SLIDE, "Resource/model/character/18_evade.mv1");

	Master::m_Camera->Initialize();
	m_ItemManager = Master::m_ItemManager;

	Item::ItemInformation* item = new Item::ItemInformation();
	item->Count = 3;
	item->ID = Item::HEAL;
	item->Name = "Heal";
	m_ItemManager->AddItem(item);

	float HpRatio = (float)m_Hp / m_MaxHp;
	m_MaxHp = m_Hp;
	m_NormalSpeed = m_Speed;

	m_NormalAttack = 10;
	m_Attack = 10;

	m_CapsuleCollider = new CapsuleCollider(this, m_Position, VAdd(m_Position, VGet(0.0f, m_Size, 0.0f)), m_Size);
	m_AttachCollider = new SphereCollider(this, m_Model->GetAttachmentPosition(), 60.0f);
	m_AttackSlideCollider = new SphereCollider(this, m_Position, 200.0f);
	m_SearchEnemyCollider = new SphereCollider(this, VAdd(m_Position, VGet(0.0f, 120.0f, 0.0f)), 500.0f);
	m_AttackJumpCollider = new SphereCollider(this, VAdd(m_Position, VGet(0.0f, 120.0f, 0.0f)), 300.0f);

	m_FirstPosition = initPos;
	m_AttackState = kAttackNormal;
}

/// @brief Player3Dクラスのデストラクタ
/// @details 動的確保したモデルやコライダーの破棄、グローバル参照のクリアを行う
Player3D::~Player3D()
{
	// ダングリングポインタによるクラッシュを防ぐため参照をクリア
	if (Master::m_Player == this) Master::m_Player = nullptr;
	delete m_Model;
	delete m_ShortInventory;
	delete m_BuffManager;
	delete m_EquipmentManager;
	delete m_HaveMoney;
	CollDelete();
}

/// @brief 毎フレームの更新処理を行う
/// @details プレイヤーの状態更新、入力を受け付けてのアクション実行を行う
void Player3D::Update()
{
	UpdateInvincibilityTimer();

	if (m_ShortInventory)
	{
		m_ShortInventory->Update();
	}

	if (m_IsDead && m_Model != nullptr)
	{
		m_Model->Update();
		return;
	}

	// イベント進行中やポーズ中はプレイヤーの操作・座標更新をブロックする
	if (ShouldSkipGameplayUpdate() || Master::m_IsPauseOn || m_Model == nullptr)
	{
		return;
	}

	ValidateTarget();
	UpdateGameplayActions();
}

/// @brief 無敵時間タイマーの減算処理を行う
void Player3D::UpdateInvincibilityTimer()
{
	if (m_InvincibleTimer > 0)
	{
		m_InvincibleTimer--;
	}
}

/// @brief ゲームプレイ更新処理をスキップすべきか判定する
/// @return bool スキップすべき場合はtrue
bool Player3D::ShouldSkipGameplayUpdate() const
{
	if (Master::m_IsStatShopOn) return true;
	return IsBossFadeActive();
}

/// @brief ボス戦への遷移演出中かどうか判定する
/// @return bool 遷移演出中であればtrue
bool Player3D::IsBossFadeActive() const
{
	SceneGame* game = Master::m_SceneManager->GetSceneGame();
	if (!game || !game->m_GameManager) return false;

	auto phase = game->m_GameManager->GetCurrentPhase();
	return phase == GameManager::Phase::kFadeOutToBoss || phase == GameManager::Phase::kFadeInBoss;
}

/// @brief ロックオン対象の敵オブジェクトが有効かどうか判定・検証する
/// @details ターゲットが存在しない、または削除済みの場合はnullにリセットする
void Player3D::ValidateTarget()
{
	if (m_Target == nullptr) return;

	bool isValid = false;
	const auto& enemies = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	for (auto obj : enemies)
	{
		if (m_Target == obj && !obj->IsDeleteFlag())
		{
			isValid = true;
			break;
		}
	}

	if (!isValid) m_Target = nullptr;
}

/// @brief 内部システム（バフやショートインベントリ）の更新を行う
void Player3D::UpdatePlayerSystems()
{
	ManagerUpdate();
}

/// @brief ゲームプレイ中のアクション（移動、攻撃、回避等）を一括処理する
/// @details ロックオン更新、移動、攻撃、回避などのプレイヤーアクションを実行する
void Player3D::UpdateGameplayActions()
{
	UpdateTargetLock();
	UpdatePlayerSystems();

	ResetNUETRAL();
	Evasion();
	SelectAttack();
	MoveEx();
	UpdateColliderPosition();
	RotationByMove();
	SearchEnemy();
	m_Model->Update();
}

/// @brief 待機状態へのリセット処理を行う
/// @details 待機状態に戻った際のターゲット解除とアニメーション初期化を行う
void Player3D::ResetNUETRAL()
{
	AnimationState now = m_Model->GetNowState();
	if (now == ANIMATION_NEUTRAL)
	{
		if (m_Model->GetIsSeparate() == true) m_Model->m_SeparateAnimation->SetAnimationCount(0.5f);
		else m_Model->m_Animation->SetAnimationCount(0.5f);

		m_TargetSearchCount = 0;
		m_Target = nullptr;
	}
}

/// @brief バフマネージャーおよびショートインベントリの更新を行う
void Player3D::ManagerUpdate()
{
	m_BuffManager->DeleteList();
	m_BuffManager->Update();
	m_ShortInventory->Update();
}

/// @brief プレイヤーおよび関連画面要素の描画処理を行う
/// @details プレイヤーモデルやUI、デバッグ情報の描画を行う
void Player3D::Draw()
{
	if (!CanDrawPlayer()) return;

	if (!Master::m_IsPauseOn && !m_IsDead)
	{
		DrawStatusBars();
	}

	DrawPlayerModel();
	DrawDebugInfo();
	DrawAttachmentDebug();
}

/// @brief プレイヤーの描画が可能かどうか判定する
/// @return bool 描画可能であればtrue
bool Player3D::CanDrawPlayer() const
{
	return (m_Hp > 0 || m_IsDead) && !Master::m_IsStatShopOn && m_Model != nullptr;
}

/// @brief プレイヤーの死亡処理を開始する
void Player3D::StartDeath()
{
	if (m_IsDead || m_Model == nullptr) return;

	m_Hp = 0.0f;
	m_IsDead = true;
	CollDelete();
	m_Model->ChangeAnimation(ANIMATION_DYING);
	m_Model->SetAnimationBlend(false);
	m_Model->SetLoop(false);
	m_Model->SetLoopFinishState(ANIMATION_MAX);
}

/// @brief 死亡アニメーションが再生終了したか判定する
/// @return bool 再生終了していればtrue
bool Player3D::IsDeathAnimationFinished() const
{
	return m_IsDead && m_Model != nullptr && m_Model->IsAnimationLoopFinish();
}

/// @brief プレイヤー3Dモデルの描画を行う
void Player3D::DrawPlayerModel()
{
	m_Model->Draw();
}

/// @brief デバッグ情報の画面表示を行う
void Player3D::DrawDebugInfo()
{
	if (Master::m_Debug == nullptr || !Master::m_Debug->Getdebug()) return;

	DrawCapsule3D(m_Position, VAdd(m_Position, VGet(0.0f, 150.0f, 0.0f)), m_Size, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
	DrawFormatString(100, 300, GetColor(255, 255, 255), "Attack:%f", GetAllStatusState(Object3D::Status_Attack));
	DrawFormatString(100, 400, GetColor(255, 255, 255), "Equipment:%f", m_EquipmentManager->GetDamage());
	DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f        Z:%f", m_Position.x, m_Position.y, m_Position.z);
	DrawFormatString(100, 500, GetColor(255, 255, 255), "Speed:%f", GetAllStatusState(Object3D::Status_Speed));
}

/// @brief 武器アタッチメント位置のデバッグ球体描画を行う
void Player3D::DrawAttachmentDebug()
{
	if (Master::m_Debug == nullptr || !Master::m_Debug->Getdebug()) return;

	DrawSphere3D(m_Model->GetAttachmentPosition(), 30.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
}

/// @brief キー入力に基づいた移動処理および向きの補間計算を行う
/// @details 入力に基づくプレイヤーの座標と向きの更新を行う
void Player3D::MoveEx()
{
	AnimationState state = m_Model->GetNowState();
	// 攻撃モーション中や回避中は不自然な滑り移動を防ぐためWASD入力をブロックする
	if (state == ANIMATION_ATTACKJUMP || state == ANIMATION_ATTACK || state == ANIMATION_JUMP_OUT || state == ANIMATION_SLIDE || state == ANIMATION_ATTACKSLIDE || Master::m_IsStatShopOn)
	{
		return;
	}

	m_MoveVec = VGet(0.0f, 0.0f, 0.0f);
	VECTOR forwardMoveVector = VSub(Master::m_Camera->GetlookAtPosition(), Master::m_Camera->GetPosition());
	VECTOR m_LeftMoveVector = VCross(forwardMoveVector, VGet(0.0f, 1.0f, 0.0f));

	bool isMove = (m_MoveVec.x != 0.0f || m_MoveVec.z != 0.0f);

	if (CheckHitKey(KEY_INPUT_A))
	{
		m_MoveVec = VAdd(m_MoveVec, m_LeftMoveVector);
		isMove = true;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		m_MoveVec = VAdd(m_MoveVec, VScale(m_LeftMoveVector, -1.0f));
		isMove = true;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		m_MoveVec = VAdd(m_MoveVec, forwardMoveVector);
		isMove = true;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		m_MoveVec = VAdd(m_MoveVec, VScale(forwardMoveVector, -1.0f));
		isMove = true;
	}

	if (state != ANIMATION_JUMP_IN && state != ANIMATION_JUMP_LOOP)
	{
		if (isMove) m_Model->ChangeAnimation(ANIMATION_RUN);
		else m_Model->ChangeAnimation(ANIMATION_NEUTRAL);
	}

	m_OldPosition = m_Position;

	if (isMove)
	{
		forwardMoveVector = VNorm(forwardMoveVector);
		m_LeftMoveVector = VNorm(m_LeftMoveVector);
		m_MoveVec = VNorm(m_MoveVec);

		m_TargetAngle = atan2f(m_MoveVec.x, m_MoveVec.z);
		m_PreviousMoveVec = m_MoveVec;
		m_Position = VAdd(m_Position, VScale(m_MoveVec, GetAllStatusState(Object3D::Status_Speed)));
	}

	TerrainFollow();
	CheckStageOut();

	m_Model->SetPosition(m_Position);
	m_Model->SetRotation(m_Rotation);
}

/// @brief プレイヤーへのダメージ計算および適用処理を行う
/// @param damage 受けるダメージ量
/// @details HPの減算処理および0以下時の死亡処理発火を行う
void Player3D::Damage(float damage)
{
	AnimationState now = m_Model->GetNowState();
	// 無敵時間中、または回避モーション中はダメージ判定を無効化する
	if (m_InvincibleTimer > 0) return;
	if (now == ANIMATION_SLIDE || now == ANIMATION_ATTACKSLIDE) return;

	m_Hp -= (damage - m_EquipmentManager->GetDamage());
	if (m_Hp <= 0.0f)
	{
		StartDeath();
	}
}

/// @brief プレイヤーがステージ領域外に出たか判定し補正する
/// @details プレイヤーがステージ外に出た場合、ステージ境界まで座標を押し戻す
void Player3D::CheckStageOut()
{
	float radiusX = Config::StageRadius_x;
	float radiusZ = Config::StageRadius_z;
	VECTOR centerPos = VGet(Config::GetStageCenter().x, 0.0f, Config::GetStageCenter().z);

	SceneGame* game = Master::m_SceneManager->GetSceneGame();
	if (game != nullptr && game->m_GameManager != nullptr &&
		game->m_GameManager->GetCurrentPhase() == GameManager::Phase::kBoss)
	{
		centerPos = Config::GetStageBossCenter();
		radiusX = Config::BossStageRadius;
		radiusZ = Config::BossStageRadius;
	}

	float dx = m_Position.x - centerPos.x;
	float dz = m_Position.z - centerPos.z;
	float normX = dx / radiusX;
	float normZ = dz / radiusZ;
	float distance = sqrtf(normX * normX + normZ * normZ);

	if (distance <= 1.0f)
	{
		m_IsStageOut = false;
		return;
	}

	// 境界外への脱出バグを防ぐため、円形境界の縁へ座標を強制補正する
	float scale = 1.0f / distance;
	m_Position.x = centerPos.x + dx * scale;
	m_Position.z = centerPos.z + dz * scale;
	m_IsStageOut = true;
}

/// @brief 回避行動を実行する
/// @details 回避アクションの実行と無敵時間の付与を行う
void Player3D::Evasion()
{
	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (m_Model->GetIsSeparate()) m_Model->m_SeparateAnimation->SetAnimationCount(1.2f);
		else m_Model->m_Animation->SetAnimationCount(1.2f);
		m_Model->ChangeAnimation(ANIMATION_SLIDE);
		m_Model->SetLoop(false);
		m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);

		m_InvincibleTimer = 30 + m_UpgradeEvasionInvincibility;
	}

	if (m_Model->GetNowState() == ANIMATION_SLIDE)
	{
		VECTOR evasionDir = m_PreviousMoveVec;
		evasionDir.y = 0.0f; // y軸方向への移動をキャンセル
		if (VSquareSize(evasionDir) > 0.0001f) {
			evasionDir = VNorm(evasionDir);
		}
		m_Position = VAdd(m_Position, VScale(evasionDir, m_EvasionSpeed + m_UpgradeEvasionSpeed));
		m_Model->SetPosition(m_Position);
	}
}

/// @brief 移動ベクトルに基づき、モデルの向きを滑らかに補間計算・回転させる
/// @details 移動方向へ向けたモデルの滑らかな回転処理を行う
void Player3D::RotationByMove()
{
	float subAngle = m_TargetAngle - m_Angle;

	if (subAngle < -DX_PI_F) subAngle += DX_TWO_PI_F;
	if (subAngle > DX_PI_F) subAngle -= DX_TWO_PI_F;

	if (subAngle > 0.0f)
	{
		subAngle -= RotateSpeed;
		if (subAngle < 0.0f) subAngle = 0.0f;
	}
	else if (subAngle < 0.0f)
	{
		subAngle += RotateSpeed;
		if (subAngle > 0.0f) subAngle = 0.0f;
	}

	m_Angle = m_TargetAngle - subAngle;
	m_Rotation.y = m_Angle + DX_PI_F;
	m_Model->SetRotation(m_Rotation);
}

/// @brief ジャンプ処理を実行する
/// @details ジャンプの開始と上方向への初速付与を行う
void Player3D::Jump()
{
	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		m_Position.y += 300.0f;
		m_IsJumping = true;
		m_JumpPower = JumpPower;
	}
}

/// @brief 通常攻撃を実行する
/// @details 通常攻撃の実行と敵のヒット判定リセットを行う
void Player3D::Attack()
{
	AnimationState now = m_Model->GetNowState();
	int m_MouseInput = GetMouseInput();

	if (m_MouseInput & MOUSE_INPUT_LEFT && m_AttackCount >= m_AttackCooldown && now != ANIMATION_ATTACK)
	{
		m_AttackCount = 0;
		Master::m_SoundManager->PlaySE(SoundManager::SE_SLASH);

		m_Model->ChangeAnimation(ANIMATION_ATTACK);
		m_Model->SetLoop(false);
		m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);

		if (m_Model->GetIsSeparate() == true) m_Model->m_SeparateAnimation->SetAnimationCount(0.9f);
		else m_Model->m_Animation->SetAnimationCount(0.35f);
	}

	const auto& pObjList = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	if (now == ANIMATION_ATTACK)
	{
		m_AttackState = kAttackNormal;

		if (m_AttackCount == 0)
		{
			// 1回の攻撃モーションで多段ヒットしすぎるのを防ぐため、一定フレームごとにヒットフラグをリセットする
			for (int i = 0; i < pObjList.size(); i++)
			{
				Enemy* pEne = pObjList.at(i)->CastTo<Enemy>();
				if (pEne == nullptr) continue;
				pEne->SetHitJudgmentFlagPlayer(false);
			}
		}
	}
}

/// @brief ジャンプ攻撃を実行する
/// @details ジャンプ攻撃の軌道計算と着地時のエフェクト生成を行う
void Player3D::AttackJump()
{
	int m_MouseInput = GetMouseInput();

	if (m_MouseInput & MOUSE_INPUT_LEFT && m_AttackJumpCount >= m_AttackJumpCooldown && !m_IsJumping)
	{
		Master::m_SoundManager->PlaySE(SoundManager::SE_JUMP);
		m_IsJumping = true;
		m_AttackJumpCount = 0;
		m_JumpPower = JumpPower;

		m_Model->ChangeAnimation(ANIMATION_ATTACKJUMP);
		m_Model->SetLoop(false);
		m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);

		if (m_Model->GetIsSeparate() == true) m_Model->m_SeparateAnimation->SetAnimationCount(1.0f);
		else m_Model->m_Animation->SetAnimationCount(1.0f);
	}

	if (m_IsJumping && m_AttackState == kAttackJump)
	{
		if (m_JumpPower >= m_Position.y && !m_HasReachedJumpPeak)
		{
			m_Position = VAdd(m_Position, VGet(0.0f, 5.0f, 0.0f));
		}
		if (m_JumpPower <= m_Position.y)
		{
			m_HasReachedJumpPeak = true;
			m_IsJumpFalling = true;
		}
		if (m_HasReachedJumpPeak)
		{
			m_Position = VAdd(m_Position, VGet(0.0f, m_JumpPower, 0.0f));
			m_JumpPower -= 1.0f;
		}

		float groundY = -10000.0f;
		const auto& stageList = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Stage);
		for (int i = 0; i < stageList.size(); i++)
		{
			Stage* pStage = stageList.at(i)->CastTo<Stage>();
			if (pStage != nullptr)
			{
				VECTOR hit = pStage->CheckHit_Line(VAdd(m_Position, VGet(0.0f, 1000.0f, 0.0f)), VAdd(m_Position, VGet(0.0f, -1000.0f, 0.0f)));
				if (hit.x != 0.0f || hit.y != 0.0f || hit.z != 0.0f)
				{
					if (hit.y > groundY) groundY = hit.y;
				}
			}
		}
		if (groundY == -10000.0f) groundY = 0.0f;

		// 着地判定時のみコライダーをアクティブにし、空中で敵に触れてもダメージが発生しない仕様にする
		if (m_Position.y <= groundY)
		{
			if (!m_IsJumpColliderActive)
			{
				m_IsJumpColliderActive = true;
				new EffekseerObject("JumpAttack", "Resource/effect/jump_attack/01_jump_attack.efk", m_Position, this, false);

				const auto& pObjList = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
				for (int i = 0; i < (int)pObjList.size(); i++)
				{
					Enemy* pEne = pObjList.at(i)->CastTo<Enemy>();
					if (pEne == nullptr) continue;
					pEne->SetHitJudgmentFlagPlayer(false);
				}
			}
			m_Position.y = groundY;
			m_IsJumping = false;
			m_IsJumpFalling = false;
			m_HasReachedJumpPeak = false;
		}
		m_Model->SetPosition(m_Position);
	}
	else
	{
		m_IsJumpColliderActive = false;
		m_HasReachedJumpPeak = false;
		m_IsJumping = false;
		m_IsJumpFalling = false;
	}
}

/// @brief スライド（突進）攻撃を実行する
/// @details ターゲットに向かってのスライド攻撃と座標更新を行う
void Player3D::AttackSlide()
{
	AnimationState now = m_Model->GetNowState();
	int m_MouseInput = GetMouseInput();

	if (m_MouseInput & MOUSE_INPUT_LEFT && m_AttackSlideCount >= m_AttackSlideCooldown)
	{
		if (m_Target != nullptr)
		{
			Master::m_SoundManager->PlaySE(SoundManager::SE_SLIDE_ATTACK);
			if (m_Model->GetIsSeparate()) m_Model->m_SeparateAnimation->SetAnimationCount(1.2f);
			else m_Model->m_Animation->SetAnimationCount(1.2f);

			m_AttackSlideCount = 0;
			m_AttackSlideDirection = (VSub(m_Target->GetPosition(), m_Position));
			m_AttackSlideStep = VScale(m_AttackSlideDirection, 2.5f / 30.0f);
		}

		m_Model->ChangeAnimation(ANIMATION_ATTACKSLIDE);
		m_Model->SetLoop(false);
		m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);

		const auto& pObjListSlide = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < (int)pObjListSlide.size(); i++)
		{
			Enemy* pEneSlide = pObjListSlide.at(i)->CastTo<Enemy>();
			if (pEneSlide == nullptr) continue;
			pEneSlide->SetHitJudgmentFlagPlayer(false);
		}

		new EffekseerObject("Slash", "Resource/effect/slide_attack/02_slide_attack_effect_playback.efk", m_Position, this, true);
	}

	if (now == ANIMATION_ATTACKSLIDE && m_AttackState == kAttackSlide)
	{
		m_AttackSlideDirection = VNorm(m_AttackSlideDirection);
		m_TargetAngle = atan2f(m_AttackSlideDirection.x, m_AttackSlideDirection.z);

		if (m_AttackSlideCount < 30)
		{
			m_Position = VAdd(m_Position, m_AttackSlideStep);
			Master::m_Camera->AddHorizontalAngle(90.0f / 30.0f);
		}
		m_Model->SetPosition(m_Position);
	}
}

/// @brief 画面上のHPゲージ、スキルクールタイムUI等のステータス描画を行う
/// @details HPバーとスキルクールタイムUIの描画を行う
void Player3D::DrawStatusBars()
{
	float maxHp = GetAllStatusState(Object3D::Status_Hp);
	if (maxHp <= 0.0f) maxHp = 1.0f;
	m_Hp = m_Hp < 0.0f ? 0.0f : m_Hp;
	m_Hp = m_Hp > maxHp ? maxHp : m_Hp;
	float hpRatio = m_Hp / maxHp;
	hpRatio = hpRatio < 0.0f ? 0.0f : hpRatio;
	hpRatio = hpRatio > 1.0f ? 1.0f : hpRatio;

	const int white = GetColor(255, 255, 255);
	const int panel = GetColor(18, 17, 20);
	const int panelLight = GetColor(46, 42, 45);
	const int iron = GetColor(78, 74, 76);
	const int gold = GetColor(198, 154, 64);
	const int goldDark = GetColor(98, 73, 32);
	const int redBack = GetColor(62, 14, 16);
	const int red = GetColor(214, 43, 35);
	const int redLight = GetColor(255, 101, 67);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 185);
	DrawBox(24, 24, 404, 92, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(30, 28, 398, 86, panel, true);
	DrawBox(34, 32, 394, 38, panelLight, true);
	DrawLine(30, 28, 398, 28, gold, 1);
	DrawLine(30, 86, 398, 86, goldDark, 1);
	DrawLine(30, 28, 30, 86, goldDark, 1);
	DrawLine(398, 28, 398, 86, gold, 1);

	DrawBox(48, 49, 382, 76, GetColor(8, 8, 10), true);
	DrawBox(52, 53, 378, 72, redBack, true);
	int hpWidth = (int)(326.0f * hpRatio);
	if (hpWidth > 0)
	{
		DrawBox(52, 53, 52 + hpWidth, 72, red, true);
		DrawBox(52, 53, 52 + hpWidth, 58, redLight, true);
		DrawLine(52, 72, 52 + hpWidth, 72, GetColor(102, 8, 8), 1);
	}
	for (int i = 1; i < 10; ++i)
	{
		int markX = 52 + (326 * i / 10);
		DrawLine(markX, 53, markX, 72, GetColor(34, 18, 18), 1);
	}
	DrawLine(48, 49, 382, 49, white, 1);
	DrawLine(48, 76, 382, 76, iron, 1);
	DrawFormatString(48, 24, GetColor(245, 226, 174), "HP");
	DrawFormatString(314, 24, GetColor(232, 225, 215), "%d / %d", (int)m_Hp, (int)maxHp);

	float slideRatio = m_AttackSlideCooldown > 0 ? (float)m_AttackSlideCount / (float)m_AttackSlideCooldown : 1.0f;
	slideRatio = slideRatio < 0.0f ? 0.0f : slideRatio;
	slideRatio = slideRatio > 1.0f ? 1.0f : slideRatio;
	float jumpRatio = m_AttackJumpCooldown > 0 ? (float)m_AttackJumpCount / (float)m_AttackJumpCooldown : 1.0f;
	jumpRatio = jumpRatio < 0.0f ? 0.0f : jumpRatio;
	jumpRatio = jumpRatio > 1.0f ? 1.0f : jumpRatio;

	const int slotW = 220;
	const int slotH = 86;
	const int gap = 28;
	const int baseX = Config::ScreenWidth / 2 - (slotW * 2 + gap) / 2;
	const int baseY = Config::ScreenHeight - 148;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 175);
	DrawBox(baseX - 18, baseY - 18, baseX + slotW * 2 + gap + 18, baseY + slotH + 20, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int slotX = baseX;
	DrawBox(slotX, baseY, slotX + slotW, baseY + slotH, panel, true);
	DrawBox(slotX + 5, baseY + 5, slotX + slotW - 5, baseY + slotH - 5, GetColor(25, 29, 34), true);
	DrawLine(slotX, baseY, slotX + slotW, baseY, gold, 1);
	DrawLine(slotX, baseY + slotH, slotX + slotW, baseY + slotH, goldDark, 1);
	DrawLine(slotX, baseY, slotX, baseY + slotH, goldDark, 1);
	DrawLine(slotX + slotW, baseY, slotX + slotW, baseY + slotH, gold, 1);
	DrawFormatString(slotX + 20, baseY + 14, GetColor(196, 232, 255), "SLIDE");
	DrawBox(slotX + 20, baseY + 54, slotX + slotW - 20, baseY + 70, GetColor(9, 17, 22), true);
	DrawBox(slotX + 22, baseY + 56, slotX + 22 + (int)((slotW - 44) * slideRatio), baseY + 68, GetColor(38, 186, 224), true);
	DrawLine(slotX + 22, baseY + 56, slotX + 22 + (int)((slotW - 44) * slideRatio), baseY + 56, GetColor(141, 239, 255), 1);
	if (slideRatio >= 1.0f) DrawFormatString(slotX + slotW - 76, baseY + 14, GetColor(255, 238, 156), "READY");

	slotX = baseX + slotW + gap;
	DrawBox(slotX, baseY, slotX + slotW, baseY + slotH, panel, true);
	DrawBox(slotX + 5, baseY + 5, slotX + slotW - 5, baseY + slotH - 5, GetColor(31, 24, 36), true);
	DrawLine(slotX, baseY, slotX + slotW, baseY, gold, 1);
	DrawLine(slotX, baseY + slotH, slotX + slotW, baseY + slotH, goldDark, 1);
	DrawLine(slotX, baseY, slotX, baseY + slotH, goldDark, 1);
	DrawLine(slotX + slotW, baseY, slotX + slotW, baseY + slotH, gold, 1);
	DrawFormatString(slotX + 20, baseY + 14, GetColor(231, 204, 255), "JUMP");
	DrawBox(slotX + 20, baseY + 54, slotX + slotW - 20, baseY + 70, GetColor(18, 10, 22), true);
	DrawBox(slotX + 22, baseY + 56, slotX + 22 + (int)((slotW - 44) * jumpRatio), baseY + 68, GetColor(176, 76, 230), true);
	DrawLine(slotX + 22, baseY + 56, slotX + 22 + (int)((slotW - 44) * jumpRatio), baseY + 56, GetColor(242, 170, 255), 1);
	if (jumpRatio >= 1.0f) DrawFormatString(slotX + slotW - 76, baseY + 14, GetColor(255, 238, 156), "READY");

	if (m_HaveMoney) m_HaveMoney->Draw();
	m_ShortInventory->Draw();
}

/// @brief 視点・表示モードの切り替え更新を行う
void Player3D::UpdateViewMode()
{
}

/// @brief 周囲の敵キャラクターの検索処理を行う
void Player3D::SearchEnemy()
{
}

/// @brief 他のコライダーと接触した瞬間の処理
/// @param collider 自身のコライダー
/// @param check 接触した相手のコライダー
/// @details 索敵範囲に入った敵のターゲット登録、または障害物との衝突補正を行う
void Player3D::OnEnter(Collider* collider, Collider* check)
{
	if (collider == m_SearchEnemyCollider && check->m_ParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
	{
		auto pEne = check->m_ParentObject->CastTo<Enemy>();
		if (pEne == nullptr) return;
		VECTOR enemyDistance = VSub(pEne->GetPosition(), m_Position);

		if (check == pEne->GetEnemyCollider())
		{
			m_IsAttackSlideTargetFound = true;
			float enemyDistanceSize = VSize(enemyDistance);
			m_TargetSearchCount++;
			if (m_TargetSearchCount == 1)
			{
				m_NearestTargetDistance = enemyDistanceSize;
			}

			if (m_NearestTargetDistance <= enemyDistanceSize)
			{
				m_NearestTargetDistance = enemyDistanceSize;
				m_Target = pEne;
			}
		}
	}

	// 障害物にめり込んだ際、直前の座標に巻き戻すことで壁抜けを防ぐ
	if (collider == m_CapsuleCollider && check->m_ParentObject->GetTag() == Tag3D_Obj)
	{
		m_Position = m_OldPosition;
	}

	ApplyJumpAttackHit(collider, check);
}

/// @brief 他のコライダーと接触中の判定処理
/// @param collider 自身のコライダー
/// @param check 接触した相手のコライダー
/// @details 各種攻撃コライダーが敵にヒットした際のダメージ計算とエフェクト生成を行う
void Player3D::OnTrigger(Collider* collider, Collider* check)
{
	AnimationState now = m_Model->GetNowState();

	if (now == ANIMATION_ATTACK)
	{
		if (collider == m_AttachCollider && check->m_ParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
		{
			Enemy* pEne = check->m_ParentObject->CastTo<Enemy>();
			if (pEne == nullptr) return;

			if (check == pEne->GetEnemyCollider())
			{
				// 多段ヒットを防ぐため、既にこの攻撃がヒットした敵は除外する
				if (now == ANIMATION_ATTACK && m_AttackState == kAttackNormal && !m_IsJumping && !pEne->IsHitJudgmentFlagPlayer())
				{
					pEne->SetHitJudgmentFlagPlayer(true);
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));

					Master::m_Camera->SetupShake(2.0f, 6.0f, 2.0f);
					EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/image/battle/01_damage.png", GetColorU8(255, 0, 30, 0), 30.0f, 0.1f);
				}
			}
		}
	}

	ApplyJumpAttackHit(collider, check);

	if (collider == m_AttackSlideCollider && check->m_ParentObject->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = check->m_ParentObject->CastTo<Enemy>();
		if (pEne == nullptr) return;
		if (check == pEne->GetEnemyCollider())
		{
			if (now == ANIMATION_ATTACKSLIDE && m_AttackState == kAttackSlide && !pEne->IsHitJudgmentFlagPlayer())
			{
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + m_SlideAttack);
				pEne->SetHitJudgmentFlagPlayer(true);

				Master::m_Camera->SetupShake(6.0f, 12.0f, 6.0f);
				if (Master::m_HitStopTimer == 0) Master::m_HitStopTimer = 3; // 最初のヒットのみストップ
				EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/image/battle/01_damage.png", GetColorU8(35, 0, 255, 0), 60.0f, 1.0f);
			}
		}
	}
}

/// @brief ジャンプ攻撃のヒット判定適用を行う
/// @param collider 判定チェックを行う自身のコライダー
/// @param check 接触した相手のコライダー
void Player3D::ApplyJumpAttackHit(Collider* collider, Collider* check)
{
	if (collider != m_AttackJumpCollider) return;
	if (!m_IsJumpColliderActive) return;
	if (m_AttackState != kAttackJump) return;
	if (check == nullptr || check->m_ParentObject == nullptr) return;
	if (check->m_ParentObject->GetTag() != Tag3D_Enemy3D) return;

	Enemy* pEne = check->m_ParentObject->CastTo<Enemy>();
	if (pEne == nullptr) return;
	if (check != pEne->GetEnemyCollider()) return;
	if (pEne->IsHitJudgmentFlagPlayer()) return;

	pEne->SetHitJudgmentFlagPlayer(true);
	pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + m_JumpAttack, false);

	Master::m_Camera->SetupShake(8.0f, 15.0f, 8.0f);
	if (Master::m_HitStopTimer == 0) Master::m_HitStopTimer = 4; // 最初の大ダメージ時のみストップ
	EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/image/battle/01_damage.png", GetColorU8(255, 100, 0, 0), 45.0f, 0.5f);
}

/// @brief ロックオンしているターゲットの生存確認と自動解除を行う
/// @details ターゲットした敵が消滅した際のロックオン解除を行う
void Player3D::UpdateTargetLock()
{
	if (m_Target != nullptr)
	{
		bool isTargetValid = false;
		const auto& m_EneList = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < m_EneList.size(); i++)
		{
			if (m_Target == m_EneList.at(i) && !m_Target->IsDeleteFlag())
			{
				isTargetValid = true;
				break;
			}
		}
		if (!isTargetValid)
		{
			m_Target = nullptr;
		}
	}
}

/// @brief 他のコライダーから離れた瞬間の処理
/// @param collider 自身のコライダー
/// @param check 離れた相手のコライダー
void Player3D::OnExit(Collider* collider, Collider* check)
{
}

/// @brief 現在選択されている攻撃タイプの切り替えと各攻撃関数の呼び出しを行う
/// @details 攻撃タイプの切り替えと対応する攻撃関数の呼び出しを行う
void Player3D::SelectAttack()
{
	UpdateAttackCooldowns();
	AnimationState now = m_Model->GetNowState();
	if (InputManager::CheckDownKey(KEY_INPUT_E) && now != ANIMATION_ATTACK)
	{
		m_AttackSelectionIndex++;
		if (m_AttackSelectionIndex > 2)
		{
			m_AttackSelectionIndex = 0;
		}
	}

	switch (m_AttackSelectionIndex)
	{
	case 0:
		m_AttackState = kAttackNormal;
		Attack();
		break;
	case 1:
		m_AttackState = kAttackSlide;
		AttackSlide();
		break;
	case 2:
		m_AttackState = kAttackJump;
		AttackJump();
		break;
	default:
		break;
	}

	if (Master::m_Debug != nullptr && Master::m_Debug->Getdebug())
	{
		DrawFormatString(300, 300, GetColor(255, 255, 255), "%d", m_AttackSelectionIndex);
	}

	// 攻撃モーション終了時にヒット判定フラグをリセットし、次回の攻撃が当たるようにする
	if (now != ANIMATION_ATTACK && now != ANIMATION_ATTACKJUMP && now != ANIMATION_ATTACKSLIDE)
	{
		const auto& mpEne = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < mpEne.size(); i++)
		{
			Enemy* pEne = mpEne.at(i)->CastTo<Enemy>();
			if (pEne == nullptr) continue;
			pEne->SetHitJudgmentFlagPlayer(false);
		}
	}
}

/// @brief 各種攻撃スキルのクールタイムタイマーを増加更新する
/// @details 各種攻撃のクールタイムカウント進行を行う
void Player3D::UpdateAttackCooldowns()
{
	m_AttackCount++;
	m_AttackSlideCount++;
	m_AttackJumpCount++;
}

/// @brief アニメーションやプレイヤー座標に合わせてコライダー位置を同期・更新する
/// @details プレイヤーの現在座標やモーションに応じた各種コライダーの位置更新を行う
void Player3D::UpdateColliderPosition()
{
	AnimationState now = m_Model->GetNowState();

	m_CapsuleCollider->m_Position = m_Position;
	m_CapsuleCollider->m_Position2 = VAdd(m_Position, VGet(0.0f, 150.0f, 0.0f));

	// 非アクティブなコライダーの誤判定を防ぐため、画面外の座標へ退避させる
	m_AttachCollider->m_Position = VGet(1000, 10000, 1000);
	m_SearchEnemyCollider->m_Position = m_Position;
	m_AttackSlideCollider->m_Position = VGet(1000, 10000, 1000);
	m_AttackJumpCollider->m_Position = VGet(1000, 10000, 1000);

	if (now == ANIMATION_ATTACKSLIDE && m_AttackState == kAttackSlide)
	{
		m_AttackSlideCollider->m_Position = m_Position;
	}
	else if (m_AttackState == kAttackJump && m_IsJumpColliderActive)
	{
		m_AttackJumpCollider->m_Position = m_Position;
	}
	else if (now == ANIMATION_ATTACK)
	{
		m_AttachCollider->m_Position = m_Model->GetAttachmentPosition();
	}
}

/// @brief バフ・装備・アビリティ強化等の補正を含めた最終ステータス値を計算取得する
/// @param state 取得したいステータスの種類
/// @return float 最終ステータス値
/// @details バフや装備補正を含めた最終ステータス値を計算して返す
float Player3D::GetAllStatusState(Object3D::StatusState state)
{
	if (m_BuffManager == nullptr) return 0;

	// バフや装備品による補正値を加算し、実ダメージ計算等に用いる最終的なステータス値を返す
	if (state == Status_Attack)
	{
		return m_NormalAttack + m_BuffManager->GetBuff(state) + m_UpgradeAttack;
	}
	if (state == Status_Speed)
	{
		return m_Speed + m_BuffManager->GetBuff(state) + m_UpgradeSpeed;
	}
	if (state == Status_Hp)
	{
		return m_MaxHp + m_UpgradeMaxHp;
	}
	return 0.0f;
}

/// @brief 自身に紐づく動的コライダー群の破棄処理を行う
/// @details 動的生成されたコライダーの破棄予約を行う
void Player3D::CollDelete()
{
	// メモリリーク防止のため、インスタンス破棄時に紐づくコライダーも破棄する
	if (m_CapsuleCollider != nullptr)
	{
		m_CapsuleCollider->SetDeleteFlag(true);
		m_CapsuleCollider = nullptr;
	}
	if (m_AttachCollider != nullptr)
	{
		m_AttachCollider->SetDeleteFlag(true);
		m_AttachCollider = nullptr;
	}
	if (m_AttackJumpCollider != nullptr)
	{
		m_AttackJumpCollider->SetDeleteFlag(true);
		m_AttackJumpCollider = nullptr;
	}
	if (m_AttackSlideCollider != nullptr)
	{
		m_AttackSlideCollider->SetDeleteFlag(true);
		m_AttackSlideCollider = nullptr;
	}
	if (m_SearchEnemyCollider != nullptr)
	{
		m_SearchEnemyCollider->SetDeleteFlag(true);
		m_SearchEnemyCollider = nullptr;
	}
}