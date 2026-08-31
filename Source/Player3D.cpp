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
/// @param is_separate_anim_ アニメーション分離処理を行うかどうかのフラグ
/// @details プレイヤーモデルの初期化、各種アニメーションとコライダーの生成、マネージャー類への登録を行う
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool is_separate_anim_)
	: Object3D(initPos)
	, attack_(3)
	, jump_attack_(5)
	, slide_attack_(7)
	, is_jump_collider_active_(false)
	, is_invisible_(false)
	, is_jumping_(false)
	, speed_(speed)
	, kJumpPower(jumppower)
	, hp_(hp)
	, max_hp_(hp)
	, size_(60.0f)
	, ride_old_hp_(0)
	, jump_power_(150.0f)
	, attack_slide_speed_(20.0f)
	, attack_slide_count_(0)
	, attack_jump_count_(0)
	, attack_count_(0)
	, attack_selection_index_(0)
	, is_jump_falling_(false)
	, has_reached_jump_peak_(false)
	, is_attack_slide_target_found_(false)
	, evasion_speed_(20.0f)
	, is_stage_out_(true)
	, is_dead_(false)
{
	Master::player_ = this;

	SetTag(Object3D::Tag3D_Player3D);
	buff_manager_ = new BuffManager();
	equipment_manager_ = new EquipmentManager();
	short_inventory_ = new ShortInventory();
	model_ = new Model(filename, initPos, is_separate_anim_);
	have_money_ = new HaveMoneyClass(0);

	model_->AddAttachment("Resource/3Dモデル/武器/01_剣モデル.mv1", "mixamorig:RightHand");
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/3Dモデル/キャラクターとアニメーション/11_待機アニメーション.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/3Dモデル/キャラクターとアニメーション/12_走りアニメーション.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/3Dモデル/キャラクターとアニメーション/13_死亡アニメーション.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/3Dモデル/キャラクターとアニメーション/16_通常攻撃アニメーション.mv1");
	model_->AddAnimation(ANIMATION_ATTACKSLIDE, "Resource/3Dモデル/キャラクターとアニメーション/15_攻撃アニメーション１.mv1");
	model_->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/3Dモデル/キャラクターとアニメーション/17_ジャンプ攻撃アニメーション.mv1");
	model_->AddAnimation(ANIMATION_SLIDE, "Resource/3Dモデル/キャラクターとアニメーション/18_回避アニメーション.mv1");

	Master::camera_->Initialize();
	item_manager_ = Master::item_manager_;

	Item::ItemInformation* item = new Item::ItemInformation();
	item->Count = 3;
	item->ID = Item::HEAL;
	item->Name = "Heal";
	item_manager_->AddItem(item);

	float HpRatio = (float)hp_ / max_hp_;
	max_hp_ = hp_;
	normal_speed_ = speed_;

	normal_attack_ = 10;
	attack_ = 10;

	capsule_collider_ = new CapsuleCollider(this, position_, VAdd(position_, VGet(0.0f, size_, 0.0f)), size_);
	attach_collider_ = new SphereCollider(this, model_->GetAttachmentPosition(), 60.0f);
	attack_slide_collider_ = new SphereCollider(this, position_, 200.0f);
	search_enemy_collider_ = new SphereCollider(this, VAdd(position_, VGet(0.0f, 120.0f, 0.0f)), 500.0f);
	attack_jump_collider_ = new SphereCollider(this, VAdd(position_, VGet(0.0f, 120.0f, 0.0f)), 300.0f);

	first_position_ = initPos;
	attack_state_ = kAttackNormal;
}

/// @brief Player3Dクラスのデストラクタ
/// @details 動的確保したモデルやコライダーの破棄、グローバル参照のクリアを行う
Player3D::~Player3D()
{
	// ダングリングポインタによるクラッシュを防ぐため参照をクリア
	if (Master::player_ == this) Master::player_ = nullptr;
	delete model_;
	delete short_inventory_;
	CollDelete();
}

/// @brief 毎フレームの更新処理を行う
/// @details プレイヤーの状態更新、入力を受け付けてのアクション実行を行う
void Player3D::Update()
{
	UpdateInvincibilityTimer();

	if (short_inventory_)
	{
		short_inventory_->Update();
	}

	if (is_dead_ && model_ != nullptr)
	{
		model_->Update();
		return;
	}

	// イベント進行中やポーズ中はプレイヤーの操作・座標更新をブロックする
	if (ShouldSkipGameplayUpdate() || Master::is_pause_on_ || model_ == nullptr)
	{
		return;
	}

	ValidateTarget();
	UpdateGameplayActions();
}

/// @brief 無敵時間タイマーの減算処理を行う
void Player3D::UpdateInvincibilityTimer()
{
	if (invincible_timer_ > 0)
	{
		invincible_timer_--;
	}
}

/// @brief ゲームプレイ更新処理をスキップすべきか判定する
/// @return bool スキップすべき場合はtrue
bool Player3D::ShouldSkipGameplayUpdate() const
{
	if (Master::is_stat_shop_on_) return true;
	return IsBossFadeActive();
}

/// @brief ボス戦への遷移演出中かどうか判定する
/// @return bool 遷移演出中であればtrue
bool Player3D::IsBossFadeActive() const
{
	SceneGame* game = Master::scene_manager_->GetSceneGame();
	if (!game || !game->game_manager_) return false;

	auto phase = game->game_manager_->GetCurrentPhase();
	return phase == GameManager::Phase::kFadeOutToBoss || phase == GameManager::Phase::kFadeInBoss;
}

/// @brief ロックオン対象の敵オブジェクトが有効かどうか判定・検証する
/// @details ターゲットが存在しない、または削除済みの場合はnullにリセットする
void Player3D::ValidateTarget()
{
	if (target_ == nullptr) return;

	bool isValid = false;
	const auto& enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	for (auto obj : enemies)
	{
		if (target_ == obj && !obj->IsDeleteFlag())
		{
			isValid = true;
			break;
		}
	}

	if (!isValid) target_ = nullptr;
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
	model_->Update();
}

/// @brief 待機状態へのリセット処理を行う
/// @details 待機状態に戻った際のターゲット解除とアニメーション初期化を行う
void Player3D::ResetNUETRAL()
{
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_NEUTRAL)
	{
		if (model_->GetIsSeparate() == true) model_->separate_animation_->SetAnimationCount(0.5f);
		else model_->animation_->SetAnimationCount(0.5f);

		target_search_count_ = 0;
		target_ = nullptr;
	}
}

/// @brief バフマネージャーおよびショートインベントリの更新を行う
void Player3D::ManagerUpdate()
{
	buff_manager_->DeleteList();
	buff_manager_->Update();
	short_inventory_->Update();
}

/// @brief プレイヤーおよび関連画面要素の描画処理を行う
/// @details プレイヤーモデルやUI、デバッグ情報の描画を行う
void Player3D::Draw()
{
	if (!CanDrawPlayer()) return;

	if (!Master::is_pause_on_ && !is_dead_)
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
	return (hp_ > 0 || is_dead_) && !Master::is_stat_shop_on_ && model_ != nullptr;
}

/// @brief プレイヤーの死亡処理を開始する
void Player3D::StartDeath()
{
	if (is_dead_ || model_ == nullptr) return;

	hp_ = 0.0f;
	is_dead_ = true;
	CollDelete();
	model_->ChangeAnimation(ANIMATION_DYING);
	model_->SetAnimationBlend(false);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_MAX);
}

/// @brief 死亡アニメーションが再生終了したか判定する
/// @return bool 再生終了していればtrue
bool Player3D::IsDeathAnimationFinished() const
{
	return is_dead_ && model_ != nullptr && model_->IsAnimationLoopFinish();
}

/// @brief プレイヤー3Dモデルの描画を行う
void Player3D::DrawPlayerModel()
{
	model_->Draw();
}

/// @brief デバッグ情報の画面表示を行う
void Player3D::DrawDebugInfo()
{
	if (Master::debug_ == nullptr || !Master::debug_->Getdebug()) return;

	DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 150.0f, 0.0f)), size_, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
	DrawFormatString(100, 300, GetColor(255, 255, 255), "Attack:%f", GetAllStatusState(Object3D::Status_Attack));
	DrawFormatString(100, 400, GetColor(255, 255, 255), "Equipment:%f", equipment_manager_->GetDamage());
	DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f        Z:%f", position_.x, position_.y, position_.z);
	DrawFormatString(100, 500, GetColor(255, 255, 255), "Speed:%f", GetAllStatusState(Object3D::Status_Speed));
}

/// @brief 武器アタッチメント位置のデバッグ球体描画を行う
void Player3D::DrawAttachmentDebug()
{
	if (Master::debug_ == nullptr || !Master::debug_->Getdebug()) return;

	DrawSphere3D(model_->GetAttachmentPosition(), 30.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
}

/// @brief キー入力に基づいた移動処理および向きの補間計算を行う
/// @details 入力に基づくプレイヤーの座標と向きの更新を行う
void Player3D::MoveEx()
{
	AnimationState state = model_->GetNowState();
	// 攻撃モーション中や回避中は不自然な滑り移動を防ぐためWASD入力をブロックする
	if (state == ANIMATION_ATTACKJUMP || state == ANIMATION_ATTACK || state == ANIMATION_JUMP_OUT || state == ANIMATION_SLIDE || state == ANIMATION_ATTACKSLIDE || Master::is_stat_shop_on_)
	{
		return;
	}

	move_vec_ = VGet(0.0f, 0.0f, 0.0f);
	VECTOR forwardMoveVector = VSub(Master::camera_->GetlookAtPosition(), Master::camera_->GetPosition());
	VECTOR left_move_vector_ = VCross(forwardMoveVector, VGet(0.0f, 1.0f, 0.0f));

	bool isMove = (move_vec_.x != 0.0f || move_vec_.z != 0.0f);

	if (CheckHitKey(KEY_INPUT_A))
	{
		move_vec_ = VAdd(move_vec_, left_move_vector_);
		isMove = true;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		move_vec_ = VAdd(move_vec_, VScale(left_move_vector_, -1.0f));
		isMove = true;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		move_vec_ = VAdd(move_vec_, forwardMoveVector);
		isMove = true;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		move_vec_ = VAdd(move_vec_, VScale(forwardMoveVector, -1.0f));
		isMove = true;
	}

	if (state != ANIMATION_JUMP_IN && state != ANIMATION_JUMP_LOOP)
	{
		if (isMove) model_->ChangeAnimation(ANIMATION_RUN);
		else model_->ChangeAnimation(ANIMATION_NEUTRAL);
	}

	old_position_ = position_;

	if (isMove)
	{
		forwardMoveVector = VNorm(forwardMoveVector);
		left_move_vector_ = VNorm(left_move_vector_);
		move_vec_ = VNorm(move_vec_);

		target_angle_ = atan2f(move_vec_.x, move_vec_.z);
		previous_move_vec_ = move_vec_;
		position_ = VAdd(position_, VScale(move_vec_, GetAllStatusState(Object3D::Status_Speed)));
	}

	TerrainFollow();
	CheckStageOut();

	model_->SetPosition(position_);
	model_->SetRotation(rotation_);
}

/// @brief プレイヤーへのダメージ計算および適用処理を行う
/// @param damage 受けるダメージ量
/// @details HPの減算処理および0以下時の死亡処理発火を行う
void Player3D::Damage(float damage)
{
	AnimationState now = model_->GetNowState();
	// 無敵時間中、または回避モーション中はダメージ判定を無効化する
	if (invincible_timer_ > 0) return;
	if (now == ANIMATION_SLIDE || now == ANIMATION_ATTACKSLIDE) return;

	hp_ -= (damage - equipment_manager_->GetDamage());
	if (hp_ <= 0.0f)
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

	SceneGame* game = Master::scene_manager_->GetSceneGame();
	if (game != nullptr && game->game_manager_ != nullptr &&
		game->game_manager_->GetCurrentPhase() == GameManager::Phase::kBoss)
	{
		centerPos = Config::GetStageBossCenter();
		radiusX = Config::BossStageRadius;
		radiusZ = Config::BossStageRadius;
	}

	float dx = position_.x - centerPos.x;
	float dz = position_.z - centerPos.z;
	float normX = dx / radiusX;
	float normZ = dz / radiusZ;
	float distance = sqrtf(normX * normX + normZ * normZ);

	if (distance <= 1.0f)
	{
		is_stage_out_ = false;
		return;
	}

	// 境界外への脱出バグを防ぐため、円形境界の縁へ座標を強制補正する
	float scale = 1.0f / distance;
	position_.x = centerPos.x + dx * scale;
	position_.z = centerPos.z + dz * scale;
	is_stage_out_ = true;
}

/// @brief 回避行動を実行する
/// @details 回避アクションの実行と無敵時間の付与を行う
void Player3D::Evasion()
{
	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (model_->GetIsSeparate()) model_->separate_animation_->SetAnimationCount(1.2f);
		else model_->animation_->SetAnimationCount(1.2f);
		model_->ChangeAnimation(ANIMATION_SLIDE);
		model_->SetLoop(false);
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);

		invincible_timer_ = 30 + upgrade_evasion_invincibility_;
	}

	if (model_->GetNowState() == ANIMATION_SLIDE)
	{
		VECTOR evasionDir = previous_move_vec_;
		evasionDir.y = 0.0f; // y軸方向への移動をキャンセル
		if (VSquareSize(evasionDir) > 0.0001f) {
			evasionDir = VNorm(evasionDir);
		}
		position_ = VAdd(position_, VScale(evasionDir, evasion_speed_ + upgrade_evasion_speed_));
		model_->SetPosition(position_);
	}
}

/// @brief 移動ベクトルに基づき、モデルの向きを滑らかに補間計算・回転させる
/// @details 移動方向へ向けたモデルの滑らかな回転処理を行う
void Player3D::RotationByMove()
{
	float subAngle = target_angle_ - angle_;

	if (subAngle < -DX_PI_F) subAngle += DX_TWO_PI_F;
	if (subAngle > DX_PI_F) subAngle -= DX_TWO_PI_F;

	if (subAngle > 0.0f)
	{
		subAngle -= kRotateSpeed;
		if (subAngle < 0.0f) subAngle = 0.0f;
	}
	else if (subAngle < 0.0f)
	{
		subAngle += kRotateSpeed;
		if (subAngle > 0.0f) subAngle = 0.0f;
	}

	angle_ = target_angle_ - subAngle;
	rotation_.y = angle_ + DX_PI_F;
	model_->SetRotation(rotation_);
}

/// @brief ジャンプ処理を実行する
/// @details ジャンプの開始と上方向への初速付与を行う
void Player3D::Jump()
{
	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		position_.y += 300.0f;
		is_jumping_ = true;
		jump_power_ = kJumpPower;
	}
}

/// @brief 通常攻撃を実行する
/// @details 通常攻撃の実行と敵のヒット判定リセットを行う
void Player3D::Attack()
{
	AnimationState now = model_->GetNowState();
	int mouse_input_ = GetMouseInput();

	if (mouse_input_ & MOUSE_INPUT_LEFT && attack_count_ >= attack_cooldown_ && now != ANIMATION_ATTACK)
	{
		attack_count_ = 0;
		Master::sound_manager_->PlaySE(SoundManager::SE_ATTACK);

		model_->ChangeAnimation(ANIMATION_ATTACK);
		model_->SetLoop(false);
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);

		if (model_->GetIsSeparate() == true) model_->separate_animation_->SetAnimationCount(1.3f);
		else model_->animation_->SetAnimationCount(0.5f);
	}

	const auto& pObjList = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	if (now == ANIMATION_ATTACK)
	{
		attack_state_ = kAttackNormal;

		if (attack_count_ % 5 == 0)
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
	int mouse_input_ = GetMouseInput();

	if (mouse_input_ & MOUSE_INPUT_LEFT && attack_jump_count_ >= attack_jump_cooldown_ && !is_jumping_)
	{
		Master::sound_manager_->PlaySE(SoundManager::SE_JUMP);
		is_jumping_ = true;
		attack_jump_count_ = 0;
		jump_power_ = kJumpPower;

		model_->ChangeAnimation(ANIMATION_ATTACKJUMP);
		model_->SetLoop(false);
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);

		if (model_->GetIsSeparate() == true) model_->separate_animation_->SetAnimationCount(1.0f);
		else model_->animation_->SetAnimationCount(1.0f);
	}

	if (is_jumping_ && attack_state_ == kAttackJump)
	{
		if (jump_power_ >= position_.y && !has_reached_jump_peak_)
		{
			position_ = VAdd(position_, VGet(0.0f, 5.0f, 0.0f));
		}
		if (jump_power_ <= position_.y)
		{
			has_reached_jump_peak_ = true;
			is_jump_falling_ = true;
		}
		if (has_reached_jump_peak_)
		{
			position_ = VAdd(position_, VGet(0.0f, jump_power_, 0.0f));
			jump_power_ -= 1.0f;
		}

		float groundY = -10000.0f;
		const auto& stageList = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Stage);
		for (int i = 0; i < stageList.size(); i++)
		{
			Stage* pStage = stageList.at(i)->CastTo<Stage>();
			if (pStage != nullptr)
			{
				VECTOR hit = pStage->CheckHit_Line(VAdd(position_, VGet(0.0f, 1000.0f, 0.0f)), VAdd(position_, VGet(0.0f, -1000.0f, 0.0f)));
				if (hit.x != 0.0f || hit.y != 0.0f || hit.z != 0.0f)
				{
					if (hit.y > groundY) groundY = hit.y;
				}
			}
		}
		if (groundY == -10000.0f) groundY = 0.0f;

		// 着地判定時のみコライダーをアクティブにし、空中で敵に触れてもダメージが発生しない仕様にする
		if (position_.y <= groundY)
		{
			if (!is_jump_collider_active_)
			{
				is_jump_collider_active_ = true;
				new EffekseerObject("JumpAttack", "Resource/エフェクト/ジャンプ攻撃/01_ジャンプ攻撃エフェクト.efk", position_, this, false);

				const auto& pObjList = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
				for (int i = 0; i < (int)pObjList.size(); i++)
				{
					Enemy* pEne = pObjList.at(i)->CastTo<Enemy>();
					if (pEne == nullptr) continue;
					pEne->SetHitJudgmentFlagPlayer(false);
				}
			}
			position_.y = groundY;
			is_jumping_ = false;
			is_jump_falling_ = false;
			has_reached_jump_peak_ = false;
		}
		model_->SetPosition(position_);
	}
	else
	{
		is_jump_collider_active_ = false;
		has_reached_jump_peak_ = false;
		is_jumping_ = false;
		is_jump_falling_ = false;
	}
}

/// @brief スライド（突進）攻撃を実行する
/// @details ターゲットに向かってのスライド攻撃と座標更新を行う
void Player3D::AttackSlide()
{
	AnimationState now = model_->GetNowState();
	int mouse_input_ = GetMouseInput();

	if (mouse_input_ & MOUSE_INPUT_LEFT && attack_slide_count_ >= attack_slide_cooldown_)
	{
		if (target_ != nullptr)
		{
			Master::sound_manager_->PlaySE(SoundManager::SE_ATTACKSLIDE);
			if (model_->GetIsSeparate()) model_->separate_animation_->SetAnimationCount(1.2f);
			else model_->animation_->SetAnimationCount(1.2f);

			attack_slide_count_ = 0;
			attack_slide_direction_ = (VSub(target_->GetPosition(), position_));
			attack_slide_step_ = VScale(attack_slide_direction_, 2.5f / 30.0f);
		}

		model_->ChangeAnimation(ANIMATION_ATTACKSLIDE);
		model_->SetLoop(false);
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);

		const auto& pObjListSlide = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < (int)pObjListSlide.size(); i++)
		{
			Enemy* pEneSlide = pObjListSlide.at(i)->CastTo<Enemy>();
			if (pEneSlide == nullptr) continue;
			pEneSlide->SetHitJudgmentFlagPlayer(false);
		}

		new EffekseerObject("Slash", "Resource/エフェクト/スライド攻撃/02_スライド攻撃エフェクト再生用.efk", position_, this, true);
	}

	if (now == ANIMATION_ATTACKSLIDE && attack_state_ == kAttackSlide)
	{
		attack_slide_direction_ = VNorm(attack_slide_direction_);
		target_angle_ = atan2f(attack_slide_direction_.x, attack_slide_direction_.z);

		if (attack_slide_count_ < 30)
		{
			position_ = VAdd(position_, attack_slide_step_);
			Master::camera_->AddHorizontalAngle(90.0f / 30.0f);
		}
		model_->SetPosition(position_);
	}
}

/// @brief 画面上のHPゲージ、スキルクールタイムUI等のステータス描画を行う
/// @details HPバーとスキルクールタイムUIの描画を行う
void Player3D::DrawStatusBars()
{
	float maxHp = GetAllStatusState(Object3D::Status_Hp);
	if (maxHp <= 0.0f) maxHp = 1.0f;
	hp_ = hp_ < 0.0f ? 0.0f : hp_;
	hp_ = hp_ > maxHp ? maxHp : hp_;
	float hpRatio = hp_ / maxHp;
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
	DrawFormatString(48, 34, GetColor(245, 226, 174), "HP");
	DrawFormatString(314, 34, GetColor(232, 225, 215), "%d / %d", (int)hp_, (int)maxHp);

	float slideRatio = attack_slide_cooldown_ > 0 ? (float)attack_slide_count_ / (float)attack_slide_cooldown_ : 1.0f;
	slideRatio = slideRatio < 0.0f ? 0.0f : slideRatio;
	slideRatio = slideRatio > 1.0f ? 1.0f : slideRatio;
	float jumpRatio = attack_jump_cooldown_ > 0 ? (float)attack_jump_count_ / (float)attack_jump_cooldown_ : 1.0f;
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

	if (have_money_) have_money_->Draw();
	short_inventory_->Draw();
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
	if (collider == search_enemy_collider_ && check->parent_object_->GetTag() == Object3D::Tag3D_Enemy3D)
	{
		auto pEne = check->parent_object_->CastTo<Enemy>();
		if (pEne == nullptr) return;
		VECTOR enemyDistance = VSub(pEne->GetPosition(), position_);

		if (check == pEne->GetEnemyCollider())
		{
			is_attack_slide_target_found_ = true;
			float enemyDistanceSize = VSize(enemyDistance);
			target_search_count_++;
			if (target_search_count_ == 1)
			{
				nearest_target_distance_ = enemyDistanceSize;
			}

			if (nearest_target_distance_ <= enemyDistanceSize)
			{
				nearest_target_distance_ = enemyDistanceSize;
				target_ = pEne;
			}
		}
	}

	// 障害物にめり込んだ際、直前の座標に巻き戻すことで壁抜けを防ぐ
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == Tag3D_Obj)
	{
		position_ = old_position_;
	}

	ApplyJumpAttackHit(collider, check);
}

/// @brief 他のコライダーと接触中の判定処理
/// @param collider 自身のコライダー
/// @param check 接触した相手のコライダー
/// @details 各種攻撃コライダーが敵にヒットした際のダメージ計算とエフェクト生成を行う
void Player3D::OnTrigger(Collider* collider, Collider* check)
{
	AnimationState now = model_->GetNowState();

	if (now == ANIMATION_ATTACK)
	{
		if (collider == attach_collider_ && check->parent_object_->GetTag() == Object3D::Tag3D_Enemy3D)
		{
			Enemy* pEne = check->parent_object_->CastTo<Enemy>();
			if (pEne == nullptr) return;

			if (check == pEne->GetEnemyCollider())
			{
				// 多段ヒットを防ぐため、既にこの攻撃がヒットした敵は除外する
				if (now == ANIMATION_ATTACK && attack_state_ == kAttackNormal && !is_jumping_ && !pEne->IsHitJudgmentFlagPlayer())
				{
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));

					Master::camera_->SetupShake(5.0f, 10.0f, 5.0f);
					EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/画像/戦闘/01_ダメージ表示画像.png", GetColorU8(255, 0, 30, 0), 30.0f, 0.1f);
				}
			}
		}
	}

	ApplyJumpAttackHit(collider, check);

	if (collider == attack_slide_collider_ && check->parent_object_->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = check->parent_object_->CastTo<Enemy>();
		if (pEne == nullptr) return;
		if (check == pEne->GetEnemyCollider())
		{
			if (now == ANIMATION_ATTACKSLIDE && attack_state_ == kAttackSlide && !pEne->IsHitJudgmentFlagPlayer())
			{
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + slide_attack_);
				pEne->SetHitJudgmentFlagPlayer(true);

				Master::camera_->SetupShake(5.0f, 10.0f, 5.0f);
				EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/画像/戦闘/01_ダメージ表示画像.png", GetColorU8(35, 0, 255, 0), 60.0f, 1.0f);
			}
		}
	}
}

/// @brief ジャンプ攻撃のヒット判定適用を行う
/// @param collider 判定チェックを行う自身のコライダー
/// @param check 接触した相手のコライダー
void Player3D::ApplyJumpAttackHit(Collider* collider, Collider* check)
{
	if (collider != attack_jump_collider_) return;
	if (!is_jump_collider_active_) return;
	if (attack_state_ != kAttackJump) return;
	if (check == nullptr || check->parent_object_ == nullptr) return;
	if (check->parent_object_->GetTag() != Tag3D_Enemy3D) return;

	Enemy* pEne = check->parent_object_->CastTo<Enemy>();
	if (pEne == nullptr) return;
	if (check != pEne->GetEnemyCollider()) return;
	if (pEne->IsHitJudgmentFlagPlayer()) return;

	pEne->SetHitJudgmentFlagPlayer(true);
	pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + jump_attack_);

	Master::camera_->SetupShake(5.0f, 10.0f, 5.0f);
	EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/画像/戦闘/01_ダメージ表示画像.png", GetColorU8(255, 100, 0, 0), 45.0f, 0.5f);
}

/// @brief ロックオンしているターゲットの生存確認と自動解除を行う
/// @details ターゲットした敵が消滅した際のロックオン解除を行う
void Player3D::UpdateTargetLock()
{
	if (target_ != nullptr)
	{
		bool isTargetValid = false;
		const auto& ene_list_ = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < ene_list_.size(); i++)
		{
			if (target_ == ene_list_.at(i) && !target_->IsDeleteFlag())
			{
				isTargetValid = true;
				break;
			}
		}
		if (!isTargetValid)
		{
			target_ = nullptr;
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
	AnimationState now = model_->GetNowState();
	if (InputManager::CheckDownKey(KEY_INPUT_E) && now != ANIMATION_ATTACK)
	{
		attack_selection_index_++;
		if (attack_selection_index_ > 2)
		{
			attack_selection_index_ = 0;
		}
	}

	switch (attack_selection_index_)
	{
	case 0:
		attack_state_ = kAttackNormal;
		Attack();
		break;
	case 1:
		attack_state_ = kAttackSlide;
		AttackSlide();
		break;
	case 2:
		attack_state_ = kAttackJump;
		AttackJump();
		break;
	default:
		break;
	}

	if (Master::debug_)
	{
		DrawFormatString(300, 300, GetColor(255, 255, 255), "%d", attack_selection_index_);
	}

	// 攻撃モーション終了時にヒット判定フラグをリセットし、次回の攻撃が当たるようにする
	if (now != ANIMATION_ATTACK && now != ANIMATION_ATTACKJUMP && now != ANIMATION_ATTACKSLIDE)
	{
		const auto& mpEne = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
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
	attack_count_++;
	attack_slide_count_++;
	attack_jump_count_++;
}

/// @brief アニメーションやプレイヤー座標に合わせてコライダー位置を同期・更新する
/// @details プレイヤーの現在座標やモーションに応じた各種コライダーの位置更新を行う
void Player3D::UpdateColliderPosition()
{
	AnimationState now = model_->GetNowState();

	capsule_collider_->position_ = position_;
	capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));

	// 非アクティブなコライダーの誤判定を防ぐため、画面外の座標へ退避させる
	attach_collider_->position_ = VGet(1000, 10000, 1000);
	search_enemy_collider_->position_ = position_;
	attack_slide_collider_->position_ = VGet(1000, 10000, 1000);
	attack_jump_collider_->position_ = VGet(1000, 10000, 1000);

	if (now == ANIMATION_ATTACKSLIDE && attack_state_ == kAttackSlide)
	{
		attack_slide_collider_->position_ = position_;
	}
	else if (attack_state_ == kAttackJump && is_jump_collider_active_)
	{
		attack_jump_collider_->position_ = position_;
	}
	else if (now == ANIMATION_ATTACK)
	{
		attach_collider_->position_ = model_->GetAttachmentPosition();
	}
}

/// @brief バフ・装備・アビリティ強化等の補正を含めた最終ステータス値を計算取得する
/// @param state 取得したいステータスの種類
/// @return float 最終ステータス値
/// @details バフや装備補正を含めた最終ステータス値を計算して返す
float Player3D::GetAllStatusState(Object3D::StatusState state)
{
	if (buff_manager_ == nullptr) return 0;

	// バフや装備品による補正値を加算し、実ダメージ計算等に用いる最終的なステータス値を返す
	if (state == Status_Attack)
	{
		return normal_attack_ + buff_manager_->GetBuff(state) + upgrade_attack_;
	}
	if (state == Status_Speed)
	{
		return speed_ + buff_manager_->GetBuff(state) + upgrade_speed_;
	}
	if (state == Status_Hp)
	{
		return max_hp_ + upgrade_max_hp_;
	}
	return 0.0f;
}

/// @brief 自身に紐づく動的コライダー群の破棄処理を行う
/// @details 動的生成されたコライダーの破棄予約を行う
void Player3D::CollDelete()
{
	// メモリリーク防止のため、インスタンス破棄時に紐づくコライダーも破棄する
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
	if (attack_jump_collider_ != nullptr)
	{
		attack_jump_collider_->SetDeleteFlag(true);
		attack_jump_collider_ = nullptr;
	}
	if (attack_slide_collider_ != nullptr)
	{
		attack_slide_collider_->SetDeleteFlag(true);
		attack_slide_collider_ = nullptr;
	}
	if (search_enemy_collider_ != nullptr)
	{
		search_enemy_collider_->SetDeleteFlag(true);
		search_enemy_collider_ = nullptr;
	}
}