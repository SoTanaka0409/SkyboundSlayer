#include"Magic.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Effect.h"

Magic::Magic(std::string filename, VECTOR initPos, float r, float damage, float speed,VECTOR movevec, int count, int time)
	:Object3D(initPos)
	,mfSpeed(speed)
	,mfAttack_chara(damage)
	,mfMagicSize(r)
	,moveVec(movevec)
	,DeleteCount(count)
	,DeleteTime(time)
	,Filename(filename)
	,mfAttack(0)
{
	mnGraphHandle = LoadGraph(filename.c_str());
	mpHitCollider = new SphereCollider(this, mvPosition, mfMagicSize);
	
}

Magic::~Magic()
{
	DeleteGraph(mnGraphHandle);
}

void Magic::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawBillboard3D(mvPosition, 0.5f, 0.5f, mfMagicSize, 0.0f, mnGraphHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Magic::Update()
{
	DeleteCount++;
	Move();
	if (DeleteCount > DeleteTime)//時間経過で消えるようにする
	{
		Death();
	}
}

void Magic::Move()
{
	mvPosition = VAdd(mvPosition, VScale(moveVec, mfSpeed));
	
}

void Magic::Death()
{
	new Effect(mvPosition, Filename, GetColorU8(255, 0, 0, 0), mfMagicSize, 0.1f);
	SetDeleteFlag(true);
	mpHitCollider->SetDeleteFlag(true);
}
void Magic::OnEnter(Collider* collider, Collider* check)//左側.
{
	
}

void Magic::OnTrigger(Collider* collider, Collider* check)
{//当たった瞬間の処理
	
}

void Magic::OnExit(Collider* collider, Collider* check)
{
	
}