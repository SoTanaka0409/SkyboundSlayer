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
	mnChance = 70;//繧｢繧､繝・Β縺ｮ繝峨Ο繝・・遒ｺ邇・
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	//繝｢繝・Ν縺ｮ逕滓・
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/Model/MagicAttack.mv1");
	
	model_->SetScale(VGet(1.3f, 1.3f, 1.3f));
	//new DrawHp("",  //逕ｻ蜒丞錐
	//	(VAdd(position_, VGet(0.0f, 100.0f, 0.0f))),//荳ｭ蠢・ｺｧ讓・
	mpDebug = new Debug();
	/*Item::ItemInformation* itemInfo;
	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 1;
	itemInfo->ID = Item::ItemID::HEAL;
	itemInfo->isLog = true;
	itemInfo->Name = "heal";
	mpItemManager->AddItem(itemInfo);*/
	
}

Enemy3D::~Enemy3D()
{
}

void Enemy3D::Update()
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

void Enemy3D::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
	if (Master::mpDebug->Getdebug() == true&& Master::mpEnemySerch->getSerch() == false)
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
		AnimationState now = model_->GetNowState();
		
		if (AttackCount >= AttackInterval && isHitAttackSearch)
		{
			AttackCount = 0;
			//謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ螟画峩
			model_->ChangeAnimation(ANIMATION_ATTACK);
			//繝ｫ繝ｼ繝励・縺輔○縺ｪ縺・
			model_->SetLoop(false);
			//繝｢繝ｼ繧ｷ繝ｧ繝ｳ蠕後・蠕・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ謌ｻ縺・
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			isHitAttackSearch = false;
			new Magic_Ene("Resource/Damage.png", VAdd(position_,VGet(0.0f,100.0f,0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
		}
		if (!(now == ANIMATION_ATTACK))
		{
			AttackCount++;
			AttackHitJudgmentflag = false;//蠖薙◆繧雁愛螳壹・蠕ｩ豢ｻ
		}
	}

	
	
