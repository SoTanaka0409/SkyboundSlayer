#include"Magic.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Effect.h"
#include "EffectPool.h"

/*
 * 魔法弾の初期化を行う。
 * [入力] filename:画像パス, initPos:初期位置, r:表示サイズと判定半径, damage:攻撃力, speed:移動速度, movevec:移動方向, count:初期寿命, time:最大寿命
 * [出力] なし
 * [副作用] 画像と当たり判定を生成する
 */
Magic::Magic(std::string filename, VECTOR initPos, float r, float damage, float speed, VECTOR movevec, int count, int time)
	:Object3D(initPos)
	, speed_(speed)
	, mfAttack_chara(damage)
	, magic_size_(r)
	, move_vec_(movevec)
	, DeleteCount(count)
	, DeleteTime(time)
	, Filename(filename)
	, attack_(0)
{
	graph_handle_ = LoadGraph(filename.c_str());
	hit_collider_ = new SphereCollider(this, position_, magic_size_);
}

Magic::~Magic()
{
	DeleteGraph(graph_handle_);
}

/*
 * 魔法弾をビルボードとして描画する。
 * [入力] なし
 * [出力] なし
 * [副作用] 描画ブレンドモードを一時的に変更する
 */
void Magic::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawBillboard3D(position_, 0.5f, 0.5f, magic_size_, 0.0f, graph_handle_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/*
 * 魔法弾の寿命と移動を更新する。
 * [入力] なし
 * [出力] なし
 * [副作用] 寿命到達時に自身を削除対象にする
 */
void Magic::Update()
{
	DeleteCount++;
	Move();
	if (DeleteCount > DeleteTime)
	{
		Death();
	}
}

/*
 * 魔法弾を進行方向へ移動させる。
 * [入力] なし
 * [出力] なし
 * [副作用] position_を更新する
 */
void Magic::Move()
{
	position_ = VAdd(position_, VScale(move_vec_, speed_));
}

/*
 * 魔法弾を削除対象にする。
 * [入力] なし
 * [出力] なし
 * [副作用] 着弾エフェクトを再生し、自身と当たり判定の削除フラグを立てる
 */
void Magic::Death()
{
	EffectPool::GetInstance()->Play(position_, Filename, GetColorU8(255, 0, 0, 0), magic_size_, 0.1f);
	SetDeleteFlag(true);
	hit_collider_->SetDeleteFlag(true);
}

/*
 * 接触開始時の衝突処理を行う。
 * [入力] collider:自分の当たり判定, check:接触相手
 * [出力] なし
 * [副作用] なし
 */
void Magic::OnEnter(Collider* collider, Collider* check)
{
}

/*
 * 接触中の衝突処理を行う。
 * [入力] collider:自分の当たり判定, check:接触相手
 * [出力] なし
 * [副作用] なし
 */
void Magic::OnTrigger(Collider* collider, Collider* check)
{
}

/*
 * 接触終了時の衝突処理を行う。
 * [入力] collider:自分の当たり判定, check:接触相手
 * [出力] なし
 * [副作用] なし
 */
void Magic::OnExit(Collider* collider, Collider* check)
{
}