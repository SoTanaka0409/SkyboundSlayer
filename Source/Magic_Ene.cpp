#include "Magic_Ene.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Effect.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"

// 入力：filename = モデルパス, initPos = 初期位置, r = 当たり判定半径, damage = 威力, speed = 速度, movevec = 移動方向, count = 寿命カウンタ, time = 最大寿命
// 副作用：SEの再生、攻撃力の初期化
Magic_Ene::Magic_Ene(std::string filename, VECTOR initPos, float r, float damage, float speed, VECTOR movevec, int count, int time)
	: Magic(filename, initPos, r, damage, speed, movevec, count, time)
{
	attack_ = 3;
	Master::sound_manager_->PlaySE(SoundManager::SE_FIRE);
}

Magic_Ene::~Magic_Ene()
{
}

// 入力：なし
// 出力：なし
// 副作用：移動処理、当たり判定の追従、および寿命到達時の消滅処理
void Magic_Ene::Update()
{
	DeleteCount++;
	Move();
	hit_collider_->position_ = position_;

	// レベルデザイン：弾幕シューティングとしての空間制御のため、一定時間で自動的に消滅させて画面内の弾密度を適正に保つ
	if (DeleteCount > DeleteTime)
	{
		Death();
	}
}

// 入力：collider = 自身の判定領域, check = 衝突相手のコライダー
// 出力：なし
// 副作用：なし
void Magic_Ene::OnEnter(Collider* collider, Collider* check)
{
}

// 入力：collider = 自身の判定領域, check = 衝突相手のコライダー
// 出力：なし
// 副作用：プレイヤーとの接触時にダメージを適用し、弾を即座に破棄する
void Magic_Ene::OnTrigger(Collider* collider, Collider* check)
{
	// アーキテクチャ設計：弾幕の密度が高くても処理落ちを防ぐため、プレイヤーと接触した瞬間のみ衝突判定を行い、即座にオブジェクトを回収する
	if (collider == hit_collider_ && check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = check->parent_object_->CastTo<Player3D>();
		if (check == pPlayer->GetCollisionCollider())
		{
			pPlayer->Damage(mfAttack_chara + attack_);
			Death();
		}
	}
}

// 入力：collider = 自身の判定領域, check = 衝突相手のコライダー
// 出力：なし
// 副作用：なし
void Magic_Ene::OnExit(Collider* collider, Collider* check)
{
}