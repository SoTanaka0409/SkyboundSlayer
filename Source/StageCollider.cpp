#include"StageCollider.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Master.h"
#include"ObjectManager.h"
#include"SceneManager.h"
#include"Effect.h"


StageCollider::StageCollider()
	:Object3D(VGet(0,1000,0))
	,Time(50)
	,Count(0)
{
	
	mpGoBossCollider = new SphereCollider(this, VGet(6000,100,6000), 100);
	mpGoStageCollider= new SphereCollider(this, VGet(16500,0, 16500), 100);
	

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
	if (mpGoStageCollider != nullptr)
	{
		mpGoStageCollider->SetDeleteFlag(true);
	}

	SetDeleteFlag(true);
}

void StageCollider::Draw()
{
	
	
}

void StageCollider::Update()
{
	Count++;
	if (Count >Time)
	{
		new Effect(mpGoBossCollider->mvPosition, "Resource/Damage.png", GetColorU8(255, 0, 30, 0), 120.0f, 0.5f);
		new Effect(mpGoStageCollider->mvPosition, "Resource/Damage.png", GetColorU8(255, 0, 30, 0), 120.0f, 0.5f);
		Count = 0;
	}

}

void StageCollider::OnEnter(Collider* collider, Collider* check)
{
	if (Count > 10)
	{
		
		/*auto mpEne = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Enemy3D);
		if (collider == mpSafeBossCollider || collider == mpSafeNormalCollider && check->mpParentObject->GetTag() == Tag3D_Enemy3D)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(check->mpParentObject);
			if (enemy == nullptr)return;
			if (check == enemy->GetEnemycoll())
			{
				enemy->SetVPosition(VGet(rand() % 3000 +1500, 100, rand() % 3000 + 1500));
			
			}

		}*/
		

		if (collider == mpGoBossCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)//normalstageに行く
		{

			
		}
		if (collider == mpGoStageCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)//bossstageに行く
		{
			
		}
	}
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	if (collider == mpGoBossCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)//normalstageに行く
	{
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr)return;
		if(check==player->Get500Collider())
		{
			DrawBox(400, 200, 600, 250, GetColor(0, 0, 0), true);
			DrawFormatString(450, 220, GetColor(255, 255, 255), "BossStageへ");
			
		}
		

	}
	if (collider == mpGoStageCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)//bossstageに行く
	{
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr)return;
		if (check == player->Get500Collider())
		{
			DrawBox(400, 200, 600, 250, GetColor(0, 0, 0), true);
			DrawFormatString(450, 220, GetColor(255, 255, 255), "normalStageへ");
		}
	}

}

void StageCollider::OnTrigger(Collider* collider, Collider* check)
{
	if (Count > 10)
	{
		auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);

		if (collider == mpGoBossCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)//normalstageに行く
		{
			Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
			if (player == nullptr) return;
			mpTexture2->Draw();
			if (check == player->GetCollisionCollider())
			{
				player->SetPosition(VAdd(mpGoStageCollider->mvPosition, VGet(500.0f, 0, 0)));//場所を移動
				Master::mpSoundManager->PlaySE(SoundManager::SE_WARP);//warp音を鳴らす
				new Effect(VGet(17000, 0, 16000), "Resource/Damage.png", GetColorU8(0, 255, 30, 0), 500.0f, 2.5f);
				new Effect(VGet(16000, 0, 17000), "Resource/Damage.png", GetColorU8(0, 255, 30, 0), 500.0f, 2.5f);
			}
		}
		if (collider == mpGoStageCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)//bossstageに行く
		{
			Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
			if (player == nullptr) return;
			mpTexture->Draw();
			if (check == player->GetCollisionCollider())
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_WARP);//warp音を鳴らす
				player->SetPosition(VAdd(mpGoBossCollider->mvPosition, VGet(500.0f, 0, 0)));//場所を移動
				
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