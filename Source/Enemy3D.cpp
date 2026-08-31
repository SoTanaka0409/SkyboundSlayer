#include "Enemy3D.h"
#include "Model.h"
#include "Master.h"
#include "Player3D.h"
#include "Object3D.h"
#include "ObjectManager.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "Stage.h"
#include "DrawHp.h"
#include "Wall.h"
#include "Scene.h"
#include "Tree.h"
#include "Effect.h"
#include "InputManager.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Magic_Ene.h"

/// @param filename=モデルパス, initPos=初期座標, hp=体力, speed=移動速度, HitSize=判定半径, Serch1~3=各種索敵半径, money=撃破報酬, is_separate_anim_=アニメ分割フラグ
/// @details 3Dモデルの基底構築、各モーション（待機・走り・死亡・魔法攻撃）のアセット登録、および初期スケール適用
Enemy3D::Enemy3D(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_)
	: Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3, money, is_separate_anim_)
{
	chance_ = 70; // ゲームバランス調整：ザコ敵を倒した際のテンポを維持しつつ、程よいリソース供給を行うためのドロップ確率（70%）
	attack_interval_ = 60;
	attack_count_ = 0;
	SetTag(Object3D::Tag3D_Enemy3D);

	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/3Dモデル/キャラクターとアニメーション/11_待機アニメーション.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/3Dモデル/キャラクターとアニメーション/12_走りアニメーション.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/3Dモデル/キャラクターとアニメーション/13_死亡アニメーション.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/3Dモデル/キャラクターとアニメーション/14_魔法攻撃アニメーション.mv1");

	model_->SetScale(VGet(1.3f, 1.3f, 1.3f));
}

Enemy3D::~Enemy3D()
{
}

/// @details 死亡時の消滅処理、または生存時における攻撃判定・移動物理計算・アニメーションフレーム更新とコライダー位置同期
void Enemy3D::Update()
{
	if (is_dead_)
	{
		DeathEnemy();
	}
	else
	{
		if (model_ != nullptr)
		{
			Attack();

			// 操作性制約：魔法詠唱アニメーションの最中に敵が滑るように移動する違和感を防ぐため、攻撃モーション中のみ移動と旋回をロック
			if (!(model_->GetNowState() == ANIMATION_ATTACK))
			{
				RotationByMove();
				Move();
			}

			model_->Update();
			UpdateColliderPosition();
		}
	}
}

/// @details 3Dモデルの描画、およびデバッグモード有効時におけるコリジョン形状のワイヤーフレームオーバーレイ表示
void Enemy3D::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}

	// デバッグ支援：ゲームプレイ中にレイキャストやUFO吸引コライダーとの正確な接触範囲を目視確認するため、実判定と完全同期したカプセルを描画
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

/// @details 攻撃アニメーションの開始、インターバルカウンターのリセット、および魔法弾オブジェクト（Magic_Ene）の動的生成
void Enemy3D::Attack()
{
	AnimationState now = model_->GetNowState();

	if (attack_count_ >= attack_interval_ && is_hit_attack_search_flag_)
	{
		attack_count_ = 0;
		model_->ChangeAnimation(ANIMATION_ATTACK);

		// 外部仕様制約：攻撃モーションがループ再生されて何度も弾丸が多重生成されるバグを防ぐため、再生は1回のみに制限し自動で待機状態へ遷移させる
		model_->SetLoop(false);
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);
		is_hit_attack_search_flag_ = false;

		new Magic_Ene("Resource/画像/戦闘/01_ダメージ表示画像.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, go_position_, 0, 150);
	}

	if (!(now == ANIMATION_ATTACK))
	{
		attack_count_++;
		is_attack_hit_judgment_flag_ = false; // 次の攻撃ターンで再び自機へのダメージ判定を行えるようにするためのフラグ復帰
	}
}
