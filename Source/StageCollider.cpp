#include"StageCollider.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Master.h"
#include"ObjectManager.h"
#include"SceneManager.h"
#include"Effect.h"
#include"Config.h"

StageCollider::StageCollider()
	:Object3D(VGet(0,1000,0))
	,Time(50)
	,Count(0)
{
	
	mpGoBossCollider = new SphereCollider(this,VAdd( Config::GetStageCenter(),VGet(0,0,-2000)), 100);
	
	

	mpTexture = new Texture("Resource/2D/BossLogo.png",VGet(400,400,0),true);
	mpTexture2 = new Texture("Resource/2D/NormalLogo.png", VGet(400, 400, 0), true);

}

StageCollider::~StageCollider()
{

	if (mpGoBossCollider != nullptr)
	{
		mpGoBossCollider->SetDeleteFlag(true);
	}
	

	SetDeleteFlag(true);
}

void StageCollider::Draw()
{
	
	
}

void StageCollider::Update()
{

}

void StageCollider::OnEnter(Collider* collider, Collider* check)
{
	


}

void StageCollider::OnTrigger(Collider* collider, Collider* check)
{
	
	

		if (collider == mpGoBossCollider && check->parent_object_->GetTag() == Tag3D_Player3D)//normalstageに行く
		{
			
			Player3D* player = Master::mpPlayer;
			if (player == nullptr) return;
			mpTexture2->Draw();
			if (check == player->GetCollisionCollider())
			{
				player->SetPosition(VAdd(Config::GetStageBossCenter(), VGet(500.0f, 0,-2000)));//場所を移勁E
				Master::mpSoundManager->PlaySE(SoundManager::SE_WARP);//warp音を鳴らす
				/*new Effect(VGet(17000, 0, 16000), "Resource/2d/Damage.png", GetColorU8(0, 255, 30, 0), 500.0f, 2.5f);
				new Effect(VGet(16000, 0, 17000), "Resource/2d/Damage.png", GetColorU8(0, 255, 30, 0), 500.0f, 2.5f);*/
			}
		}
	
	
	

}

void StageCollider::OnExit(Collider* collider, Collider* check)
{
	
}

void StageCollider::ColliderUpdate()
{
	
}
