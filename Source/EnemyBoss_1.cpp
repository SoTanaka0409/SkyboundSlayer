#include"EnemyBoss_1.h"
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

EnemyBoss_1::EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, float xp,int money, bool isSeparateAnim)
	:Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3, xp,money, isSeparateAnim)



	//,Animation(false)
{
	mbMagic = true;
	mAttackType = 0;
	mAttack1ComboCount = 0;
	mnChance = 30;//繧｢繧､繝・Β縺ｮ繝峨Ο繝・・遒ｺ邇・
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	
	//繝｢繝・Ν縺ｮ逕滓・
	mpModel->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	mpModel->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	mpModel->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKMAGIC, "Resource/Model/MagicAttack.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Jump Attack.mv1");

	mpModel->SetScale(VGet(4.0f, 4.0f, 4.0f));
	
	mpJumpAttackCoiider = new SphereCollider(this, mvPosition, 400.0f);

	mpDebug = new Debug();

	
}

EnemyBoss_1::~EnemyBoss_1()
{

}

void EnemyBoss_1::Update()
{
	if (isDead)
	{
		DeathEnemy();
	}
	else
	{
		if (mpModel != nullptr)
		{
		
			//else
			Attack();
			if (!(mpModel->GetNowState() == ANIMATION_ATTACK) || !(mpModel->GetNowState() == ANIMATION_ATTACKJUMP))
			{
				RotationByMove();
				Move();


			}

			mpModel->Update();
			//mpDH->Update();//drawHp縺ｮ繧｢繝・・繝・・繝医ｒ蜻ｼ縺ｶ
			CollPositionUpdate();
			mpJumpAttackCoiider->mvPosition = mvPosition;

		}
	}
}

void EnemyBoss_1::Draw()
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

void EnemyBoss_1::Attack()
{
	AnimationState now = mpModel->GetNowState();

	if (AttackCount >= AttackInterval && isHitAttackSearch)
	{
		AttackCount = 0;
		isHitAttackSearch = false;
		
		// Decide random attack type (0: 3x Magic, 1: Single Magic, 2: Jump Attack)
		mAttackType = GetRand(2);
		
		if (mAttackType == 0)
		{
			mAttack1ComboCount = 3;
		}
		else if (mAttackType == 1)
		{
			mpModel->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			mpModel->SetLoop(false);
			mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, VScale(GoPosition,sinf(30)), 0, 150);
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, VScale(GoPosition, cosf(30)), 0, 150);
		}
		else if (mAttackType == 2)
		{
			mpModel->ChangeAnimation(ANIMATION_ATTACK);
			mpModel->SetLoop(false);
			mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		}
	}

	if (mAttackType == 0 && mAttack1ComboCount > 0)
	{
		if (now == ANIMATION_NEUTRAL || now == ANIMATION_RUN)
		{
			mpModel->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			mpModel->SetLoop(false);
			mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, VScale(GoPosition,sinf(30)), 0, 150);
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, VScale(GoPosition, cosf(30)), 0, 150);
			
			mAttack1ComboCount--;
		}
	}

	if (!(now == ANIMATION_ATTACKMAGIC) && !(now == ANIMATION_ATTACK))
	{
		if (mAttack1ComboCount <= 0)
		{
			AttackCount++;
		}
		AttackHitJudgmentflag = false;
	}
}
void EnemyBoss_1::OnTrigger(Collider* collider, Collider* check)
{//蠖薙◆縺｣縺溽椪髢薙・蜃ｦ逅・
	if (mfHp <= 0)return; auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	AnimationState now = mpModel->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpJumpAttackCoiider && check->mpParentObject->GetTag() == Tag3D_Player3D)
		{//mpModel縺ｮ逡ｪ蜿ｷ繧ゅ≠縺｣縺ｦ縺・ｋ縲√￠縺ｩ縺薙・if譁・↓蜈･繧峨↑縺・
			Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
				{

					pPlayer->Damage(mfAttack);
					AttackHitJudgmentflag = true;//蠖薙◆縺｣縺溘ｈ繝ｼ
				}
			}

		}
	}
}

void EnemyBoss_1::DeathEnemy()
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	isDead = true;
	mpModel->ChangeAnimation(ANIMATION_DYING);
	//繝ｫ繝ｼ繝励・縺輔○縺ｪ縺・
	mpModel->SetLoop(false);
	//繝｢繝ｼ繧ｷ繝ｧ繝ｳ蠕後・蠕・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ謌ｻ縺・
	mpModel->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (mpModel->IsAnimationLoopFinish())
	{
		if (mbItem && GetRand(100) < mnChance) { Item::ItemInformation* info = new Item::ItemInformation(); info->ID = Item::HEAL; info->Count = 1; info->Name = "HEAL"; Master::mpItemManager->AddItem(info); }
		if (player != nullptr) {
			player->mpHaveMoney->AddMoney(mfHaveMoney);
			player->mpLevelUp->AddXp(mfHaveXp);
		}
		Master::GameClearCount++;
		
		Delete();
		SetDeleteFlag(true);
	}

	mpModel->Draw();
	mpModel->Update();


}

void EnemyBoss_1::Delete()
{
	Enemy::Delete();
	if (mpJumpAttackCoiider != nullptr)
	{
		mpJumpAttackCoiider->SetDeleteFlag(true);
	}
}

