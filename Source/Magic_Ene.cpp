#include "Magic_Ene.h"
#include <cmath>
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Effect.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"

/// @param filename = モデルパス, initPos = 初期位置, r = 当たり判定半径, damage = 威力, speed = 速度, movevec = 移動方向, count = 寿命カウンタ, time = 最大寿命
/// @details SEの再生、攻撃力の初期化
Magic_Ene::Magic_Ene(std::string filename, VECTOR initPos, float r, float damage, float speed, VECTOR movevec, int count, int time)
	: Magic(filename, initPos, r, damage, speed, movevec, count, time)
{
	attack_ = 3;
	Master::sound_manager_->PlaySE(SoundManager::SE_FIRE);
}

Magic_Ene::~Magic_Ene()
{
}

/// @details 移動処理、当たり判定の追従、および寿命到達時の消滅処理
void Magic_Ene::Update()
{
	Magic::Update();
}

/// @brief ボス・敵の魔法弾の描画（禍々しくプロっぽく見せる）
void Magic_Ene::Draw()
{
	// 脈打つスケール演出
	float pulse = 1.0f + 0.15f * sinf(DeleteCount * 0.3f);
	float scale = magic_size_ * pulse;
	
	// 加算半透明で発光しているように見せる
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	
	// 外側のオーラ（赤紫～暗黒っぽい色味の光）
	SetDrawBright(255, 30, 80);
	DrawBillboard3D(position_, 0.5f, 0.5f, scale * 1.8f, DeleteCount * 0.1f, graph_handle_, TRUE);
	
	// 中間のオーラ（逆回転、オレンジや紫）
	SetDrawBright(200, 50, 255);
	DrawBillboard3D(position_, 0.5f, 0.5f, scale * 1.3f, -DeleteCount * 0.15f, graph_handle_, TRUE);
	
	// 芯の部分（まばゆい白、回転させない）
	SetDrawBright(255, 255, 255);
	DrawBillboard3D(position_, 0.5f, 0.5f, scale * 0.7f, 0.0f, graph_handle_, TRUE);
	
	// 描画設定を元に戻す
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
void Magic_Ene::OnEnter(Collider* collider, Collider* check)
{
}

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
/// @details プレイヤーとの接触時にダメージを適用し、弾を即座に破棄する
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

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
void Magic_Ene::OnExit(Collider* collider, Collider* check)
{
}