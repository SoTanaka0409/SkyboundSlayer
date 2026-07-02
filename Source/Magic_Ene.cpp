#include"Magic_Ene.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Effect.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"

Magic_Ene::Magic_Ene(std::string filename, VECTOR initPos, float r, float damage, float speed, VECTOR movevec, int count, int time)
	:Magic(filename,initPos,r,damage,speed,movevec,count,time)
{
	mfAttack = 3;//この攻撃のダメージ
	Master::mpSoundManager->PlaySE(SoundManager::SE_FIRE);
}
Magic_Ene::~Magic_Ene()
{
	// Base class Magic::~Magic() will delete mpModel
}

void Magic_Ene::Update()
{
	DeleteCount++;
	Move();
	mpHitCollider->mvPosition = mvPosition;//当たり判定の移動
	if (DeleteCount > DeleteTime)//時間経過で消えるようにする
	{
		Death();
	}
}


void Magic_Ene::OnEnter(Collider* collider, Collider* check)//左側.
{
	
	
	
	

}

void Magic_Ene::OnTrigger(Collider* collider, Collider* check)
{//当たった瞬間の処理
	
	if (collider == mpHitCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = check->mpParentObject->CastTo<Player3D>();
		if (check == pPlayer->GetCollisionCollider())
		{
			pPlayer->Damage(mfAttack_chara + mfAttack);
			Death();
		}
	}


}

void Magic_Ene::OnExit(Collider* collider, Collider* check)
{
	
	

}