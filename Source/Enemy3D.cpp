#include"Enemy3D.h"
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
#include"Magic_Ene.h"

Enemy3D::Enemy3D(std::string filename, VECTOR initPos, float hp,float speed,float HitSize,float Serch1,float Serch2,float Serch3,float xp,int money, bool isSeparateAnim)
	:Enemy(filename,initPos,  hp, speed,2,HitSize,Serch1,Serch2,Serch3,xp,money, isSeparateAnim)
	
	
	
	//,Animation(false)
{
	mbMagic = true;
	mnChance = 70;//アイテムのドロップ確率
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	SetTagEne(Enemy::archerl_stage1);
	
	//SpeedDownNomber =mnSpeed-
	//モデルの生成
	mpModel->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	mpModel->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	mpModel->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACK, "Resource/Model/MagicAttack.mv1");
	
	mpModel->SetScale(VGet(1.3f, 1.3f, 1.3f));
	//new DrawHp("",  //画像名
	//	(VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f))),//中心座標
	//	VAdd(mvPosition, VGet(150.0f, 200.0f, 0.0f)),//左上座標)
	//	VAdd(mvPosition, VGet(-150.0f, 0.0f, 0.0f)));//右下座標)
	//mpCollision->SetTag(Collision::Coll_Enemy3D);
	mpDebug = new Debug();
	Item::ItemInformation* itemInfo;
	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 1;
	itemInfo->ID = Item::ItemID::HEAL;
	itemInfo->isLog = true;
	itemInfo->Name = "heal";
	mpItemManager->AddItem(itemInfo);
	
	//mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize, 0.0f)), mfSize);
	//mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 30.0f);
	//mpSerchCollider = new SphereCollider(this, mvPosition, HitSerch);//敵を探知する円
	//mpAttackCollider = new SphereCollider(this, mvPosition, HitAttackSerch);//敵を攻撃し始める円
	//mpStopCollider = new SphereCollider(this, mvPosition, HitStopSerch);//敵との距離を保つ円d
}

Enemy3D::~Enemy3D()
{
	Delete();
	SetDeleteFlag(true);
}

void Enemy3D::Update()
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

void Enemy3D::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}
	if (Master::mpDebug->Getdebug() == true&& Master::mpEnemySerch->getSerch() == false)
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
			mfSize*5,
			8,
			GetColor(255, 0, 0),
			GetColor(255, 0, 0),
			false
		);
	}
	
	
}



	void Enemy3D::Attack()
	{
		AnimationState now = mpModel->GetNowState();
		
		//f ()プレイヤーに近づいた時と書く
		if (AttackCount >= AttackInterval && isHitAttackSearch)
		{
			AttackCount = 0;
			//攻撃モーションに変更
			mpModel->ChangeAnimation(ANIMATION_ATTACK);
			//ループはさせない
			mpModel->SetLoop(false);
			//モーション後は待機モーションに戻す
			mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
			//mpModel->SetRotation(mvRotation);
			isHitAttackSearch = false;
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition,VGet(0.0f,100.0f,0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
		}
		if (!(now == ANIMATION_ATTACK))
		{
			AttackCount++;
			AttackHitJudgmentflag = false;//当たり判定の復活
		}
	}

	
	
