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
	mnChance = 30;//郢ｧ・｢郢ｧ・､郢昴・ﾎ堤ｸｺ・ｮ郢晏ｳｨﾎ溽ｹ昴・繝ｻ驕抵ｽｺ驍・・
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	
	//郢晢ｽ｢郢昴・ﾎ晉ｸｺ・ｮ騾墓ｻ薙・
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
			//mpDH->Update();//drawHp邵ｺ・ｮ郢ｧ・｢郢昴・繝ｻ郢昴・繝ｻ郢晏現・定惱・ｼ邵ｺ・ｶ
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
{//陟冶侭笳・ｸｺ・｣邵ｺ貅ｽ讀ｪ鬮｢阮吶・陷・ｽｦ騾・・
	if (mfHp <= 0)return; auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	AnimationState now = mpModel->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpJumpAttackCoiider && check->mpParentObject->GetTag() == Tag3D_Player3D)
		{//mpModel邵ｺ・ｮ騾｡・ｪ陷ｿ・ｷ郢ｧ繧・旺邵ｺ・｣邵ｺ・ｦ邵ｺ繝ｻ・狗ｸｲ竏夲ｿ邵ｺ・ｩ邵ｺ阮吶・if隴√・竊楢怦・･郢ｧ蟲ｨ竊醍ｸｺ繝ｻ
			Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
				{

					pPlayer->Damage(mfAttack);
					AttackHitJudgmentflag = true;//陟冶侭笳・ｸｺ・｣邵ｺ貅假ｽ育ｹ晢ｽｼ
				}
			}

		}
	}
}

void EnemyBoss_1::DeathEnemy()
{
	isDead = true;
	mpModel->ChangeAnimation(ANIMATION_DYING);
	//郢晢ｽｫ郢晢ｽｼ郢晏干繝ｻ邵ｺ霈披雷邵ｺ・ｪ邵ｺ繝ｻ
	mpModel->SetLoop(false);
	//郢晢ｽ｢郢晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ陟募ｾ後・陟輔・・ｩ貅佩皮ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ・ｫ隰鯉ｽｻ邵ｺ繝ｻ
	mpModel->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (mpModel->IsAnimationLoopFinish())
	{
		GiveRewards();
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



