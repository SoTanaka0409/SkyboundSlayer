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
	mpModel = new Model(filename, initPos, false);
	mpHitCollider = new SphereCollider(this, mvPosition, mfMagicSize);
	
}

Magic::~Magic()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
}

void Magic::Draw()
{

}

void Magic::Update()
{
	DeleteCount++;
	if (mpModel != nullptr)
	{
		Move();
	}
	if (DeleteCount > DeleteTime)//ŠÔŒo‰ß‚ÅÁ‚¦‚é‚æ‚¤‚É‚·‚é
	{
		Death();
	}
	
		
	
	new Effect(mvPosition, Filename, GetColorU8(255, 0, 0, 0), mfMagicSize, 0.1f);
}

void Magic::Move()
{
	mvPosition = VAdd(mvPosition, VScale(moveVec, mfSpeed));
	
}

void Magic::Death()
{
	SetDeleteFlag(true);
	mpHitCollider->SetDeleteFlag(true);
}
void Magic::OnEnter(Collider* collider, Collider* check)//¶‘¤.
{
	
}

void Magic::OnTrigger(Collider* collider, Collider* check)
{//“–‚½‚Á‚½uŠÔ‚Ìˆ—
	
}

void Magic::OnExit(Collider* collider, Collider* check)
{
	
}