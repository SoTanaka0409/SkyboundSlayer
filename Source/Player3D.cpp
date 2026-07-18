#include"Player3D.h"
#include"Model.h"
#include"ModelAnimation.h"
#include"Master.h"
#include"InputManager.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Wall.h"
#include<string>
#include <iostream>
#include <cstring> 
#include "EffectPool.h"
#include"Enemy3D.h"
#include"stage.h"
#include"Camera.h"
#include"EffekseerObject.h"
#include"Tree.h"
#include"Scene.h"
#include"Debug.h"
#include"Config.h"
#include"DrawHp.h"
#include"Effect.h"
#include"DrawCircle1.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include"InfClass.h"
#include"HaveMoneyClass.h"

// 入力：filename = モデルファイルパス, initPos = 初期座標, jumppower = ジャンプ力, speed = 移動速度, hp = 初期体力, is_separate_anim_ = 上半身/下半身の分離フラグ
// 出力：なし
// 副作用：プレイヤーモデル、各種コライダー、インベントリ、およびマネージャー群の動的生成と初期化
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool is_separate_anim_)
	:Object3D(initPos)
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
{
	Master::player_ = this;

	SetTag(Object3D::Tag3D_Player3D);
	buff_manager_ = new BuffManager();
	equipment_manager_ = new EquipmentManager();
	short_inventory_ = new ShortInventory();
	model_ = new Model(filename, initPos, is_separate_anim_);
	have_money_ = new HaveMoneyClass(0);

	model_->AddAttachment("Resource/3D/Sabel.mv1", "mixamorig:RightHand");
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Attack_3.mv1");
	model_->AddAnimation(ANIMATION_ATTACKSLIDE, "Resource/Model/Attack.mv1");
	model_->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/Model/Jump Attack.mv1");
	model_->AddAnimation(ANIMATION_SLIDE, "Resource/Model/kaihi.mv1");

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

	// アーキテクチャ設計：本体の押し出し判定、武器先端（アタッチメント）、索敵範囲、特殊攻撃範囲など、役割の異なる複数の物理コライダーを合成してアクション判定を構築する
	capsule_collider_ = new CapsuleCollider(this, position_, VAdd(position_, VGet(0.0f, size_, 0.0f)), size_);
	attach_collider_ = new SphereCollider(this, model_->GetAttachmentPosition(), 60.0f);
	attack_slide_collider_ = new SphereCollider(this, position_, 200.0f);
	search_enemy_collider_ = new SphereCollider(this, VAdd(position_, VGet(0.0f, 120.0f, 0.0f)), 500.0f);
	attack_jump_collider_ = new SphereCollider(this, VAdd(position_, VGet(0.0f, 120.0f, 0.0f)), 300.0f);

	first_position_ = initPos;
	attack_state_ = kAttackNormal;
}

// 入力：なし
// 出力：なし
// 副作用：動的に確保されたモデル・インベントリ等の解放と、コライダーの破棄予約
Player3D::~Player3D()
{
	if (Master::player_ == this) Master::player_ = nullptr;
	delete model_;
	delete short_inventory_;
	CollDelete();
}

// 入力：なし
// 出力：なし
// 副作用：各種タイマーの進行、入力状態の監視、およびアクション処理のディスパッチ
void Player3D::Update()
{
	UpdateInvincibilityTimer();

	if (short_inventory_)
	{
		short_inventory_->Update();
	}

	if (ShouldSkipGameplayUpdate() || Master::is_pause_on_ || model_ == nullptr)
	{
		return;
	}

	ValidateTarget();
	UpdateGameplayActions();
}

// 入力：なし
// 出力：なし
// 副作用：無敵タイマーのデクリメント
void Player3D::UpdateInvincibilityTimer()
{
	// UX仕様：被弾後や回避（スライディング）発動時に付与される無敵時間（i-frames）を管理し、プレイヤーに体制を立て直す猶予を与える
	if (invincible_timer_ > 0)
	{
		invincible_timer_--;
	}
}

// 入力：なし
// 出力：ゲームプレイの進行をスキップすべきならtrue
// 副作用：なし
bool Player3D::ShouldSkipGameplayUpdate() const
{
	if (Master::is_stat_shop_on_) return true;
	return IsBossFadeActive();
}

// 入力：なし
// 出力：ボスのフェード演出中ならtrue
// 副作用：なし
bool Player3D::IsBossFadeActive() const
{
	SceneGame* game = Master::scene_manager_->GetSceneGame();
	if (!game || !game->game_manager_) return false;

	auto phase = game->game_manager_->GetCurrentPhase();
	return phase == GameManager::Phase::kFadeOutToBoss || phase == GameManager::Phase::kFadeInBoss;
}

// 入力：なし
// 出力：なし
// 副作用：ロックオン対象の敵が死亡・破棄されていた場合、target_ポインタをnullにリセットする
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

// 入力：なし
// 出力：なし
// 副作用：バフマネージャーおよびインベントリのフレーム更新
void Player3D::UpdatePlayerSystems()
{
	ManagerUpdate();
}

// 入力：なし
// 出力：なし
// 副作用：プレイヤーの移動、攻撃、回避、カメラ更新などの具体的なアクショングルーチンの実行
void Player3D::UpdateGameplayActions()
{
	UpdateTargetLock();
	UpdatePlayerSystems();

	if (InputManager::CheckDownKey(KEY_INPUT_Q))
	{
		UpdateViewMode();
	}

	ResetNUETRAL();
	Evasion();
	SelectAttack();
	MoveEx();
	UpdateColliderPosition();
	RotationByMove();
	SearchEnemy();
	model_->Update();
}

// 入力：なし
// 出力：なし
// 副作用：攻撃や回避アクションが終了した際、自動的にニュートラル状態へ遷移させる
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

// 入力：なし
// 出力：なし
// 副作用：バフおよびインベントリのクリーンアップと更新
void Player3D::ManagerUpdate()
{
	buff_manager_->DeleteList();
	buff_manager_->Update();
	short_inventory_->Update();
}

// 入力：なし
// 出力：なし
// 副作用：プレイヤーモデル、HUD、およびデバッグ情報のレンダリング
void Player3D::Draw()
{
	if (!CanDrawPlayer()) return;

	if (!Master::is_pause_on_)
	{
		DrawStatusBars();
	}

	DrawPlayerModel();
	DrawDebugInfo();
	DrawAttachmentDebug();
}

bool Player3D::CanDrawPlayer() const
{
	return hp_ > 0 && !Master::is_stat_shop_on_ && model_ != nullptr;
}

void Player3D::DrawPlayerModel()
{
	model_->Draw();
}

void Player3D::DrawDebugInfo()
{
	if (Master::debug_ == nullptr || !Master::debug_->Getdebug()) return;

	DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 150.0f, 0.0f)), size_, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
	DrawFormatString(100, 300, GetColor(255, 255, 255), "Attack:%f", GetAllStatusState(Object3D::Status_Attack));
	DrawFormatString(100, 400, GetColor(255, 255, 255), "Equipment:%f", equipment_manager_->GetDamage());
	DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f       Z:%f", position_.x, position_.y, position_.z);
	DrawFormatString(100, 500, GetColor(255, 255, 255), "Speed:%f", GetAllStatusState(Object3D::Status_Speed));
}

void Player3D::DrawAttachmentDebug()
{
	if (Master::debug_ == nullptr || !Master::debug_->Getdebug()) return;

	DrawSphere3D(model_->GetAttachmentPosition(), 30.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
}

// 入力：なし
// 出力：なし
// 副作用：WASD入力に基づく移動ベクトルの算出、地形への追従、およびアニメーションの切り替え
void Player3D::MoveEx()
{
	AnimationState state = model_->GetNowState();
	// アーキテクチャ設計：攻撃モーション中や回避中などは通常の移動入力を受け付けないようにブロックし、アクションのキャンセルを抑制して操作の重みを持たせる
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
	//CheckStageOut();

	model_->SetPosition(position_);
	model_->SetRotation(rotation_);
}

// 入力：damage = 敵等から受ける基準ダメージ量
// 出力：なし
// 副作用：HPの減算。ただし無敵タイマー中や回避アクション中は無効化する
void Player3D::Damage(float damage)
{
	AnimationState now = model_->GetNowState();
	if (invincible_timer_ > 0) return;
	if (now == ANIMATION_SLIDE || now == ANIMATION_ATTACKSLIDE) return;

	hp_ -= (damage - equipment_manager_->GetDamage());
}

// 入力：なし
// 出力：なし
// 副作用：設定されたステージ半径を超えた場合、座標を一つ前のフレームに押し戻すことで画面外への落下を防ぐ
void Player3D::CheckStageOut()
{
	float radiusX = Config::StageRadius_x;
	float radiusZ = Config::StageRadius_z;
	SceneGame* game = Master::scene_manager_->GetSceneGame();
	VECTOR centerPos;

	// レベルデザイン：ボス戦時は行動エリアを強制的に狭め、逃げ回ることを許さず近接インファイトでの対処を強要する空間クランプ処理
	if (game->game_manager_->GetCurrentPhase() == GameManager::Phase::kBoss)
	{
		centerPos = Config::GetStageBossCenter();
		radiusX = Config::BossStageRadius;
		radiusZ = Config::BossStageRadius;
	}
	else
	{
		centerPos = VGet(Config::GetStageCenter().x, 0, Config::GetStageCenter().z);
	}

	float dx = position_.x - centerPos.x;
	float dz = position_.z - centerPos.z;
	float normX = dx / radiusX;
	float normZ = dz / radiusZ;
	float distance = sqrtf(normX * normX + normZ * normZ);

	if (distance > 1.0f)
	{
		position_ = old_position_;
	}
}

// 入力：なし
// 出力：なし
// 副作用：スペースキー入力でスタミナ/クールダウンを消費し、無敵フレームを持つ緊急回避（スライディング）アクションをトリガーする
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
		position_ = VAdd(position_, VScale(previous_move_vec_, evasion_speed_ + upgrade_evasion_speed_));
		model_->SetPosition(position_);
	}
}

// 入力：なし
// 出力：なし
// 副作用：移動ベクトルに応じた目標角度へのモデルの滑らかな回転補間（イージング）
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

// 入力：なし
// 出力：なし
// 副作用：上方向への速度ベクトル付与およびジャンプステートへの移行
void Player3D::Jump()
{
	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		position_.y += 300.0f;
		is_jumping_ = true;
		jump_power_ = kJumpPower;
	}
}

// 入力：なし
// 出力：なし
// 副作用：通常攻撃アニメーションの再生、効果音の再生、および攻撃判定用パーティクルの生成
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
			new EffekseerObject("PlayerAttack", "Resource/effect/tktk01/PlayerAttack_3.efkproj", position_, this, true);

			// バグ回避：多段ヒット攻撃において、同一の敵に1フレームで重複してダメージが入り即死するのを防ぐため、攻撃周期ごとにヒットフラグを強制リセットする
			for (int i = 0; i < pObjList.size(); i++)
			{
				Enemy* pEne = pObjList.at(i)->CastTo<Enemy>();
				if (pEne == nullptr) continue;
				pEne->SetHitJudgmentFlagPlayer(false);
			}
		}
	}
}

// 入力：なし
// 出力：なし
// 副作用：跳躍モーションからの急降下攻撃アニメーション、および着地時の広範囲衝撃波（エフェクトとダメージ判定）の生成
void Player3D::AttackJump()
{
	AnimationState now = model_->GetNowState();
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

	if (now == ANIMATION_ATTACKJUMP)
	{
		attack_state_ = kAttackJump;
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

		// 演出仕様：着地した瞬間に地面用判定コライダー（is_jump_collider_active_）をオンにし、ド派手な土煙エフェクトを発生させて重量感と威力を強調する
		if (position_.y <= groundY)
		{
			if (!is_jump_collider_active_)
			{
				is_jump_collider_active_ = true;
				new EffekseerObject("JumpAttack", "Resource/effect/Pierre02/JumpAttack.efkproj", position_, this, false);

				// 着地時に全敵のヒットフラグをリセットして確実にダメージが入るようにする
				const auto& pObjList = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
				for (int i = 0; i < (int)pObjList.size(); i++)
				{
					Enemy* pEne = pObjList.at(i)->CastTo<Enemy>();
					if (pEne == nullptr) continue;
					pEne->SetHitJudgmentFlagPlayer(false);
				}
			}
			position_.y = groundY;
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

// 入力：なし
// 出力：なし
// 副作用：ロックオン対象に向かっての高速突進（スライディング攻撃）および専用エフェクトの生成
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

		// スライド攻撃開始時に全敵のヒットフラグをリセットして確実にダメージが入るようにする
		const auto& pObjListSlide = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < (int)pObjListSlide.size(); i++)
		{
			Enemy* pEneSlide = pObjListSlide.at(i)->CastTo<Enemy>();
			if (pEneSlide == nullptr) continue;
			pEneSlide->SetHitJudgmentFlagPlayer(false);
		}

		new EffekseerObject("Slash", "Resource/effect/sword/slash.efkproj", position_, this, true);
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

// 入力：なし
// 出力：なし
// 副作用：画面手前の2Dレイヤーに対し、HPゲージや各スキルのリロード（クールダウン）状況をオーバーレイ描画
void Player3D::DrawStatusBars()
{
	// UI仕様：ゲーム中の視認性を最優先するため、3Dオブジェクトとしてではなく2Dの画面スクリーン座標に直接ゲージ類を描画する
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

void Player3D::UpdateViewMode()
{
}

void Player3D::SearchEnemy()
{
}

// 入力：collider = 自身の判定領域, check = 衝突相手のコライダー
// 出力：なし
// 副作用：敵へのターゲットロックオン捕捉、および壁など静的オブジェクトへのめり込み防止（押し戻し）
void Player3D::OnEnter(Collider* collider, Collider* check)
{
	// 索敵用巨大球形コライダー（search_enemy_collider_）の範囲内に敵が入った場合、最も距離が近い敵をスライディング攻撃用のロックオンターゲットに設定する
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

	if (collider == capsule_collider_ && check->parent_object_->GetTag() == Tag3D_Obj)
	{
		position_ = old_position_;
	}
}

// 入力：collider = 自身の判定領域, check = 衝突相手のコライダー
// 出力：なし
// 副作用：武器コライダーと敵との接触時のダメージ適用、および攻撃ヒット時のカメラシェイク演出トリガー
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
				if (now == ANIMATION_ATTACK && attack_state_ == kAttackNormal && !is_jumping_ && !pEne->IsHitJudgmentFlagPlayer())
				{
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));

					// UX仕様：攻撃が敵にヒットした瞬間に強めのカメラシェイクと流血（ダメージ）エフェクトを発生させ、剣戟アクションとしての「重い手応え」をプレイヤーにダイレクトに伝える
					Master::camera_->SetupShake(5.0f, 10.0f, 5.0f);
					EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/2d/Damage.png", GetColorU8(255, 0, 30, 0), 30.0f, 0.1f);
				}
			}
		}
	}

	if (collider == attack_jump_collider_ && check->parent_object_->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = check->parent_object_->CastTo<Enemy>();
		if (pEne == nullptr) return;
		if (check == pEne->GetEnemyCollider())
		{
			if (now == ANIMATION_ATTACKJUMP && attack_state_ == kAttackJump && !pEne->IsHitJudgmentFlagPlayer())
			{
				pEne->SetHitJudgmentFlagPlayer(true);
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + jump_attack_);

				Master::camera_->SetupShake(5.0f, 10.0f, 5.0f);
				EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/2d/Damage.png", GetColorU8(255, 100, 0, 0), 45.0f, 0.5f);
			}
		}
	}

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
				EffectPool::GetInstance()->Play(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/2d/Damage.png", GetColorU8(35, 0, 255, 0), 60.0f, 1.0f);
			}
		}
	}
}

// 入力：なし
// 出力：なし
// 副作用：ロックオン対象リストの無効化検知とターゲットのクリア
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

void Player3D::OnExit(Collider* collider, Collider* check)
{
}

// 入力：なし
// 出力：なし
// 副作用：Eキー入力による攻撃スタイルの循環切り替えと、選択されたスタイルに応じた攻撃関数の呼び出し
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

	// バグ回避：攻撃アクション中でないときは、敵全体に対するプレイヤー由来のヒットフラグ（無敵時間）を一斉にリセットし、次の攻撃判定を確実に通すための布石を打つ
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

// 入力：なし
// 出力：なし
// 副作用：各種攻撃スキルにおけるインターバルタイマーの進行
void Player3D::UpdateAttackCooldowns()
{
	attack_count_++;
	attack_slide_count_++;
	attack_jump_count_++;
}

// 入力：なし
// 出力：なし
// 副作用：プレイヤーの移動座標とアニメーション状態に同期して、判定コライダー（本体、武器、特殊範囲）の3D座標を毎フレーム追従させる
void Player3D::UpdateColliderPosition()
{
	AnimationState now = model_->GetNowState();

	capsule_collider_->position_ = position_;
	capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));

	attach_collider_->position_ = VGet(1000, 10000, 1000);
	search_enemy_collider_->position_ = position_;
	attack_slide_collider_->position_ = VGet(1000, 10000, 1000);
	attack_jump_collider_->position_ = VGet(1000, 10000, 1000);

	if (now == ANIMATION_ATTACKSLIDE && attack_state_ == kAttackSlide)
	{
		attack_slide_collider_->position_ = position_;
	}
	else if (now == ANIMATION_ATTACKJUMP && attack_state_ == kAttackJump && is_jump_collider_active_)
	{
		attack_jump_collider_->position_ = position_;
	}
	else if (now == ANIMATION_ATTACK)
	{
		attach_collider_->position_ = model_->GetAttachmentPosition();
	}
}

// 入力：state = 取得したいステータス種別（攻撃力、速度、HP等）
// 出力：装備補正やバフが乗算・加算された最終的なステータス計算値
// 副作用：なし
float Player3D::GetAllStatusState(Object3D::StatusState state)
{
	if (buff_manager_ == nullptr) return 0;

	// アーキテクチャ設計：プレイヤーの素のステータスに加えて、アイテムバフ（buff_manager_）と装備補正（upgrade_）を一元的に合算して返すことで、ダメージ計算時のステータス取得口を統一する
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

// 入力：なし
// 出力：なし
// 副作用：Playerクラスが所有・展開した各種動的コライダーの明示的な破棄フラグ設定
void Player3D::CollDelete()
{
	if (attach_collider_ != nullptr) attach_collider_->SetDeleteFlag(true);
	if (attack_jump_collider_ != nullptr) attack_jump_collider_->SetDeleteFlag(true);
	if (attack_slide_collider_ != nullptr) attack_slide_collider_->SetDeleteFlag(true);
	if (search_enemy_collider_ != nullptr) search_enemy_collider_->SetDeleteFlag(true);
}