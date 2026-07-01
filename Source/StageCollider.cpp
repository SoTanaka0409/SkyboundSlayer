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

	if(Master::TutorialFlag)mpGoBossCollider->mvPosition= VGet(0, 100, -6000);
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
	if (Count > 10)
	{
		auto mpPlayer = Master::mpPlayer;

		if (collider == mpGoBossCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)//normalstage‚És‚­
		{
			Player3D* player = Master::mpPlayer;
			if (player == nullptr) return;
			mpTexture2->Draw();
			if (check == player->GetCollisionCollider())
			{
				player->SetPosition(VAdd(Config::GetStageBossCenter(), VGet(500.0f, 0,-2000)));//êŠ‚ðˆÚ“®
				Master::mpSoundManager->PlaySE(SoundManager::SE_WARP);//warp‰¹‚ð–Â‚ç‚·
				/*new Effect(VGet(17000, 0, 16000), "Resource/Damage.png", GetColorU8(0, 255, 30, 0), 500.0f, 2.5f);
				new Effect(VGet(16000, 0, 17000), "Resource/Damage.png", GetColorU8(0, 255, 30, 0), 500.0f, 2.5f);*/
			}
		}
	
	}
	

}

void StageCollider::OnExit(Collider* collider, Collider* check)
{
	
}

void StageCollider::ColliderUpdate()
{
	
}