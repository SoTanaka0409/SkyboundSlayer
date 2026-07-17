#include"Magic.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Effect.h"
#include "EffectPool.h"


/*
 * 目的（MagicのMagic処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Magic::Magic(std::string filename, VECTOR initPos, float r, float damage, float speed,VECTOR movevec, int count, int time)
	:Object3D(initPos)
	,speed_(speed)
	,mfAttack_chara(damage)
	,magic_size_(r)
	,move_vec_(movevec)
	,DeleteCount(count)
	,DeleteTime(time)
	,Filename(filename)
	,attack_(0)
{
	graph_handle_ = LoadGraph(filename.c_str());
	hit_collider_ = new SphereCollider(this, position_, magic_size_);
	
}

Magic::~Magic()
{
	DeleteGraph(graph_handle_);
}


/*
 * 目的（MagicのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Magic::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawBillboard3D(position_, 0.5f, 0.5f, magic_size_, 0.0f, graph_handle_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/*
 * 目的（MagicのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Magic::Update()
{
	DeleteCount++;
	Move();
	if (DeleteCount > DeleteTime)//譎る俣邨碁℃縺ｧ豸医∴繧九ｈ縺・↓縺吶ｋ
	{
		Death();
	}
}


/*
 * 目的（MagicのMove処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Magic::Move()
{
	position_ = VAdd(position_, VScale(move_vec_, speed_));
	
}


/*
 * 目的（MagicのDeath処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Magic::Death()
{
	EffectPool::GetInstance()->Play(position_, Filename, GetColorU8(255, 0, 0, 0), magic_size_, 0.1f);
	SetDeleteFlag(true);
	hit_collider_->SetDeleteFlag(true);
}
void Magic::OnEnter(Collider* collider, Collider* check)//蟾ｦ蛛ｴ.
{
	
}


/*
 * 目的（MagicのOnTrigger処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Magic::OnTrigger(Collider* collider, Collider* check)
{//蠖薙◆縺｣縺溽椪髢薙・蜃ｦ逅・
	
}


/*
 * 目的（MagicのOnExit処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Magic::OnExit(Collider* collider, Collider* check)
{
	
}