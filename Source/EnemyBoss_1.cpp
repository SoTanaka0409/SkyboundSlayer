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
EnemyBoss_1::EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_)
	:Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3, money, is_separate_anim_)
{
	mfjumpPower = 150.0f;       // ジャンプ攻撃時の最大到達高度
	HighPositionFlag = false;   // ジャンプの頂点到達状態の管理
	attack_type_ = 0;           // 現在の攻撃パターンの種類
	attack1_combo_count_ = 0;   // 連続魔法攻撃の残り発動回数
	chance_ = 30;               // 攻撃頻度の重み付けパラメータ
	attack_interval_ = 60;      // 連続攻撃を防ぐためのクールタイム（フレーム）
	attack_count_ = 0;          // クールタイム計測用カウンタ

	SetTag(Object3D::Tag3D_Enemy3D);

	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/3Dモデル/キャラクターとアニメーション/11_待機アニメーション.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/3Dモデル/キャラクターとアニメーション/12_走りアニメーション.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/3Dモデル/キャラクターとアニメーション/13_死亡アニメーション.mv1");
	model_->AddAnimation(ANIMATION_ATTACKMAGIC, "Resource/3Dモデル/キャラクターとアニメーション/14_魔法攻撃アニメーション.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/3Dモデル/キャラクターとアニメーション/17_ジャンプ攻撃アニメーション.mv1");

	model_->SetScale(VGet(4.0f, 4.0f, 4.0f));

	jump_attack_coiider_ = new SphereCollider(this, position_, 400.0f);
}

EnemyBoss_1::~EnemyBoss_1()
{

}

/// @details ボスの座標更新、攻撃判定、アニメーション進行
void EnemyBoss_1::Update()
{
	SceneGame* game = Master::scene_manager_->GetSceneGame();
	if (game && game->game_manager_) {
		auto phase = game->game_manager_->GetCurrentPhase();
		// 画面遷移中に予期せぬ攻撃や座標移動が発生するバグを防ぐため処理を停止
		if (phase == GameManager::Phase::kFadeOutToBoss || phase == GameManager::Phase::kFadeInBoss) {
			return;
		}
	}

	if (is_dead_)
	{
		DeathEnemy();
	}
	else
	{
		if (model_ != nullptr)
		{

			Attack();
			// 攻撃モーション中の不自然な滑り移動を防ぐため座標更新を停止
			if (model_->GetNowState() != ANIMATION_ATTACK && model_->GetNowState() != ANIMATION_ATTACKJUMP)
			{
				RotationByMove();
				Move();
			}

			UpdateJumpPhysics();

			model_->Update();
			UpdateColliderPosition();
			jump_attack_coiider_->position_ = position_;

			// 地面抜けバグを防ぐためのY座標の下限補正
			if (position_.y < init_position_.y)
			{
				position_.y = init_position_.y;
			}

		}
	}
}

/// @details ボスモデルとデバッグ用コライダーの画面描画
void EnemyBoss_1::Draw()
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

/// @details 乱数による攻撃パターンの決定と魔法オブジェクトの生成
void EnemyBoss_1::Attack()
{
	AnimationState now = model_->GetNowState();

	// クールタイム消化済みかつターゲットを捕捉している場合のみ攻撃開始
	if (attack_count_ >= attack_interval_ && is_hit_attack_search_flag_)
	{
		attack_count_ = 0;
		is_hit_attack_search_flag_ = false;

		attack_type_ = GetRand(2);

		if (attack_type_ == 0)
		{
			attack1_combo_count_ = 3;
		}
		else if (attack_type_ == 1)
		{
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);

			// 広範囲をカバーするため、正面と左右30度の3方向へ魔法を同時発射
			new Magic_Ene("Resource/画像/戦闘/01_ダメージ表示画像.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, go_position_, 0, 150);
			VECTOR leftGo = VTransform(go_position_, MGetRotY(-30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/画像/戦闘/01_ダメージ表示画像.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, leftGo, 0, 150);
			VECTOR rightGo = VTransform(go_position_, MGetRotY(30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/画像/戦闘/01_ダメージ表示画像.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, rightGo, 0, 150);
		}
		else if (attack_type_ == 2)
		{
			model_->ChangeAnimation(ANIMATION_ATTACK);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			mfjumpPower = 400.0f;
			HighPositionFlag = false;
		}
	}

	// パターン0の場合、モーション完了に合わせて段階的に魔法を生成する仕様
	if (attack_type_ == 0 && attack1_combo_count_ > 0)
	{
		if (now == ANIMATION_NEUTRAL || now == ANIMATION_RUN)
		{
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);

			new Magic_Ene("Resource/画像/戦闘/01_ダメージ表示画像.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 100.0f, 5, 30.0f, go_position_, 0, 150);

			attack1_combo_count_--;
		}
	}

	if (!(now == ANIMATION_ATTACKMAGIC) && !(now == ANIMATION_ATTACK))
	{
		if (attack1_combo_count_ <= 0)
		{
			attack_count_++;
		}
		is_attack_hit_judgment_flag_ = false;
	}
}

/// @param 自身のコライダー、衝突対象のコライダー
/// @details プレイヤーのHP減少と、ヒット済みフラグの設定
void EnemyBoss_1::OnTrigger(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return;
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == jump_attack_coiider_ && check->parent_object_->GetTag() == Tag3D_Player3D)
		{
			Player3D* pPlayer = Master::player_;
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				// 多段ヒットを防ぐため、1回のジャンプ攻撃につきダメージは1度のみ
				if (now == ANIMATION_ATTACK && !is_attack_hit_judgment_flag_)
				{
					pPlayer->Damage(attack_);
					is_attack_hit_judgment_flag_ = true;
				}
			}
		}
	}
}

/// @details アニメーション完了後のインスタンス破棄予約、およびクリアフラグの更新
void EnemyBoss_1::DeathEnemy()
{
	is_dead_ = true;
	model_->ChangeAnimation(ANIMATION_DYING);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (model_->IsAnimationLoopFinish())
	{
		GiveRewards();
		// ゲーム進行管理上、ボスの討伐数をクリア条件としているためのカウントアップ
		Master::game_clear_count_++;

		Delete();
		SetDeleteFlag(true);
	}

	model_->Update();
}

/// @details 保持しているコライダーのメモリ解放フラグ設定
void EnemyBoss_1::Delete()
{
	Enemy::Delete();
	// メモリリーク防止のため、動的確保した固有コライダーを破棄する
	if (jump_attack_coiider_ != nullptr)
	{
		jump_attack_coiider_->SetDeleteFlag(true);
		jump_attack_coiider_ = nullptr;
	}
}

/// @details 攻撃タイプ2時のボスのY座標の直接更新
void EnemyBoss_1::UpdateJumpPhysics()
{
	if (model_->GetNowState() == ANIMATION_ATTACK && attack_type_ == 2)
	{
		// エンジンの重力を無視し、ジャンプ攻撃の頂点に向けた強制的な軌道計算を行う
		if (!HighPositionFlag)
		{
			position_ = VAdd(position_, VGet(0.0f, kJumpAscendSpeed, 0.0f));
			if (position_.y >= init_position_.y + mfjumpPower)
			{
				HighPositionFlag = true;
			}
		}
		else
		{
			position_ = VAdd(position_, VGet(0.0f, kJumpDescendSpeed, 0.0f));
		}

		if (position_.y <= init_position_.y)
		{
			position_.y = init_position_.y;
		}
	}
	else
	{
		HighPositionFlag = false;
	}
}