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
	mfAttack = 3;//‚±‚ÌUŒ‚‚Ìƒ_ƒ[ƒW
	Master::mpSoundManager->PlaySE(SoundManager::SE_FIRE);
}
Magic_Ene::~Magic_Ene()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
}
void Magic_Ene::Draw()
{

}

void Magic_Ene::Update()
{
	DeleteCount++;
	Move();
	mpHitCollider->mvPosition = mvPosition;//“–‚½‚è”»’è‚ÌˆÚ“®
	if (DeleteCount > DeleteTime)//ŽžŠÔŒo‰ß‚ÅÁ‚¦‚é‚æ‚¤‚É‚·‚é
	{
		Death();
	}
	
	new Effect(mvPosition, Filename, GetColorU8(255, 0, 0, 0), mfMagicSize, 0.001f);
}


void Magic_Ene::OnEnter(Collider* collider, Collider* check)//¶‘¤.
{
	
	
	
	

}

void Magic_Ene::OnTrigger(Collider* collider, Collider* check)
{//“–‚½‚Á‚½uŠÔ‚Ìˆ—
	
	if (collider == mpHitCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = dynamic_cast<Player3D*>(check->mpParentObject);
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