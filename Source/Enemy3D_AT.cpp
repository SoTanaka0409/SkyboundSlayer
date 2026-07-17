#include "Enemy3D_AT.h"
#include "Model.h"
#include "Master.h"
#include "Player3D.h"
#include "Object3D.h"
#include "ObjectManager.h"
#include "Scene3D.h"
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

// 入力：filename=基本モデルパス, initPos=初期座標, hp=体力, speed=移動速度, HitSize=判定半径, Serch1~3=索敵・攻撃判定半径群, Attack=攻撃力, money=撃破報酬, is_separate_anim_=アニメ分割フラグ
// 副作用：ベースモデルの読み込み、攻撃用のサーベル（3D武器モデル）の右手ボーンへのアタッチ、および各種モーションの登録
Enemy3D_AT::Enemy3D_AT(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, float Attack, int money, bool is_separate_anim_)
	: Enemy(filename, initPos, hp, speed, Attack, HitSize, Serch1, Serch2, Serch3, money, is_separate_anim_)
{
	// ゲームバランス調整：遠距離魔法敵（Enemy3D）よりも脅威度の高い近接突進タイプであるため、ドロップ確率を50%に調整して物資の過剰供給を抑制
	chance_ = 50;
	attack_interval_ = 120;
	attack_count_ = 0;
	SetTag(Object3D::Tag3D_Enemy3D);

	model_->SetScale(VGet(1.3f, 1.3f, 1.3f));

	// 演出仕様：敵の右手に武器モデルを確実に連動（アタッチ）させ、アニメーションによる手首のひねりや振りに刃物の位置を完全同期させる
	model_->AddAttachment("Resource/3D/Sabel.mv1", "mixamorig:RightHand");
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Attack.mv1");
}

Enemy3D_AT::~Enemy3D_AT()
{
}

// 入力：なし
// 出力：なし
// 副作用：死亡状態移行、または生存時における攻撃判定、移動・旋回の更新、および骨格アニメーションに対応したコリジョンの位置同期
void Enemy3D_AT::Update()
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

			// 操作性制約：近接斬撃モーション中に足元が滑る不自然な滑走移動を抑制するため、攻撃アニメーション中のみ歩行物理と旋回入力を完全に遮断
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

// 入力：なし
// 出力：なし
// 副作用：武器がアタッチされた敵モデルの描画、およびデバッグ用カプセル形状（実判定サイズ）のワイヤーフレーム表示
void Enemy3D_AT::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}

	// デバッグ支援：斬撃のリーチや自機のUFO衝突コライダーが、敵の物理空間のどの範囲と重なっているかをリアルタイムで視覚確認するための立体プリミティブ描画
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
