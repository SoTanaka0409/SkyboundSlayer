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
	mnChance = 50;//繧｢繧､繝・Β縺ｮ繝峨Ο繝・・遒ｺ邇・
	AttackInterval = 120;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	//繝｢繝・Ν縺ｮ逕滓・
	
	
	
	model_->SetScale(VGet(1.3f, 1.3f, 1.3f));
	model_->AddAttachment("Resource/3D/Sabel.mv1", "mixamorig:RightHand");
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Attack.mv1");


	//new DrawHp("",  //逕ｻ蜒丞錐
	//	(VAdd(position_, VGet(0.0f, 100.0f, 0.0f))),//荳ｭ蠢・ｺｧ讓・
	
	mpDebug = new Debug();
}

Enemy3D_AT::~Enemy3D_AT()
{

}

void Enemy3D_AT::Update()
{
	
	if (isDead)
	{
		DeathEnemy();
	}
	else
	{
		if (model_ != nullptr)
		{
			
			Attack();

			if (!(model_->GetNowState() == ANIMATION_ATTACK))
			{
				RotationByMove();
				Move();


			}

			model_->Update();
			//mpDH->Update();//drawHp縺ｮ繧｢繝・・繝・・繝医ｒ蜻ｼ縺ｶ
			CollPositionUpdate();

		}
	}
	
	
	
}

void Enemy3D_AT::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
		
	}
	if (Master::mpDebug->Getdebug() == true && Master::mpEnemySerch->getSerch() == false)
	{
		DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
	}
	if (Master::mpEnemySerch->getSerch() == true)
	{
		DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 150.0f, 0.0f)),
			mfSize * 5,
			8,
			GetColor(255, 0, 0),
			GetColor(255, 0, 0),
			false
		);
	}
	

}









