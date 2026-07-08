#include"Magic.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Effect.h"
#include "EffectPool.h"

Magic::Magic(std::string filename, VECTOR initPos, float r, float damage, float speed,VECTOR movevec, int count, int time)
	:Object3D(initPos)
	,speed_(speed)
	,mfAttack_chara(damage)
	,mfMagicSize(r)
	,moveVec(movevec)
	,DeleteCount(count)
	,DeleteTime(time)
	,Filename(filename)
	,attack_(0)
{
	mnGraphHandle = LoadGraph(filename.c_str());
	mpHitCollider = new SphereCollider(this, position_, mfMagicSize);
	
}

Magic::~Magic()
{
	DeleteGraph(mnGraphHandle);
}

void Magic::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawBillboard3D(position_, 0.5f, 0.5f, mfMagicSize, 0.0f, mnGraphHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Magic::Update()
{
	DeleteCount++;
	Move();
	if (DeleteCount > DeleteTime)//時間経過で消えるよぁE��する
	{
		Death();
	}
}

void Magic::Move()
{
	position_ = VAdd(position_, VScale(moveVec, speed_));
	
}

void Magic::Death()
{
	EffectPool::GetInstance()->Play(position_, Filename, GetColorU8(255, 0, 0, 0), mfMagicSize, 0.1f);
	SetDeleteFlag(true);
	mpHitCollider->SetDeleteFlag(true);
}
void Magic::OnEnter(Collider* collider, Collider* check)//左側.
{
	
}

void Magic::OnTrigger(Collider* collider, Collider* check)
{//当たった瞬間�E処琁E
	
}

void Magic::OnExit(Collider* collider, Collider* check)
{
	
}