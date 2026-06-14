#include"Enemy3D_AT.h"
#include"Model.h"
#include"Master.h"
#include"Player3D.h"
#include"Object3D.h"
#include"ObjectManager.h"
//
#include"Scene3D.h"
#include"SceneManager.h"
#include"Stage.h"
#include"DrawHp.h"


#include"Wall.h"
#include"Scene.h"
#include"Tree.h"
#include"Effect.h"
#include"InputManager.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"


Enemy3D_AT::Enemy3D_AT(std::string filename, VECTOR initPos, float hp, float speed,float HitSize, float Serch1, float Serch2,float Serch3,float Attack,float xp,int money,bool isSeparateAnim )
	:Enemy(filename,initPos,hp,speed,Attack,HitSize,Serch1,Serch2,Serch3,xp,money, isSeparateAnim)
	
	//,Animation(false)
{
	mnChance = 50;//アイテムのドロップ確率
	AttackInterval = 120;
	AttackCount = 0;
	//mpCollision->SetTag(Collision::Coll_Enemy3D);
	SetTag(Object3D::Tag3D_Enemy3D);
	SetTagEne(Enemy::night_stagg1);
	//SpeedDownNomber =mnSpeed-
	//モデルの生成
	
	//mpModel->AddAttachment("Resource/3D/Sabel.mv1", "mixamorig:RightHandIndex1");
	
	//mpAttackHitCollider= new SphereCollider(this, mvPosition, HitStopSerch);//
	
	//mpModel->AddAnimation(ANIMATION_ATTACK2, "Resource/Model/Attack.mv1");
	//mpModel->AddAttachment("Resource/3D/Sabel.mv1", "wp");
	mpModel->SetScale(VGet(1.3f, 1.3f, 1.3f));
	mpModel->AddAttachment("Resource/3D/Sabel.mv1", "mixamorig:RightHandIndex1");
	mpModel->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	mpModel->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	mpModel->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Attack.mv1");
	Item::ItemInformation* itemInfo;
	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 1;
	itemInfo->ID = Item::ItemID::POWER;
	itemInfo->Name = "Power";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	//new DrawHp("",  //画像名
	//	(VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f))),//中心座標
	//	VAdd(mvPosition, VGet(150.0f, 200.0f, 0.0f)),//左上座標)
	//	VAdd(mvPosition, VGet(-150.0f, 0.0f, 0.0f)));//右下座標)
	
	mpDebug = new Debug();
}

Enemy3D_AT::~Enemy3D_AT()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
	}


}

void Enemy3D_AT::Update()
{
	
	if (isDead)
	{
		DeathEnemy();
	}
	else
	{
		if (mpModel != nullptr)
		{
			
			Attack();

			if (!(mpModel->GetNowState() == ANIMATION_ATTACK))
			{
				RotationByMove();
				Move();


			}

			mpModel->Update();
			//mpDH->Update();//drawHpのアップデートを呼ぶ
			CollPositionUpdate();

		}
	}
	
	
	
}

void Enemy3D_AT::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
		
	}
	if (Master::mpDebug->Getdebug() == true && Master::mpEnemySerch->getSerch() == false)
	{
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
	}
	if (Master::mpEnemySerch->getSerch() == true)
	{
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize * 5,
			8,
			GetColor(255, 0, 0),
			GetColor(255, 0, 0),
			false
		);
	}
	

}









