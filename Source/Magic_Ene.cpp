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
	attack_ = 3;//縺薙・謾ｻ謦・・繝繝｡繝ｼ繧ｸ
	Master::sound_manager_->PlaySE(SoundManager::SE_FIRE);
}
Magic_Ene::~Magic_Ene()
{
	// Base class Magic::~Magic() will delete model_
}

void Magic_Ene::Update()
{
	DeleteCount++;
	Move();
	hit_collider_->position_ = position_;//蠖薙◆繧雁愛螳壹・遘ｻ蜍・
	if (DeleteCount > DeleteTime)//譎る俣邨碁℃縺ｧ豸医∴繧九ｈ縺・↓縺吶ｋ
	{
		Death();
	}
}


void Magic_Ene::OnEnter(Collider* collider, Collider* check)//蟾ｦ蛛ｴ.
{
	
	
	
	

}

void Magic_Ene::OnTrigger(Collider* collider, Collider* check)
{//蠖薙◆縺｣縺溽椪髢薙・蜃ｦ逅・
	
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

void Magic_Ene::OnExit(Collider* collider, Collider* check)
{
	
	

}