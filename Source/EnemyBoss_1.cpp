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
	mfjumpPower = 150.0f;
	HighPositionFlag = false;
	mbjumpDown = false;
	mbjump = false;
	OnJumpCollider = false;
	mAttackType = 0;
	mAttack1ComboCount = 0;
	mnChance = 30;//驛｢・ｧ繝ｻ・｢驛｢・ｧ繝ｻ・､驛｢譏ｴ繝ｻ・主�､・ｸ・ｺ繝ｻ・ｮ驛｢譎擾ｽｳ・ｨ・取ｺｽ・ｹ譏ｴ繝ｻ郢晢ｽｻ鬩墓慣・ｽ・ｺ鬩阪・繝ｻ
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	
	//驛｢譎｢・ｽ・｢驛｢譏ｴ繝ｻ・取刮・ｸ・ｺ繝ｻ・ｮ鬨ｾ蠅難ｽｻ阮吶・
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

			if (mpModel->GetNowState() == ANIMATION_ATTACK && mAttackType == 2)
			{
				if (mfjumpPower >= mvPosition.y && !HighPositionFlag)
				{
					mvPosition = VAdd(mvPosition, VGet(0.0f, 5.0f, 0.0f));
				}
				if (mfjumpPower <= mvPosition.y)
				{
					HighPositionFlag = true;
					mbjumpDown = true;
				}
				if (HighPositionFlag)
				{
					mvPosition = VAdd(mvPosition, VGet(0.0f, mfjumpPower, 0.0f));
					mfjumpPower -= 1.0f;
				}

				if (mvPosition.y <= VinitPos.y)
				{
					OnJumpCollider = true;
					mvPosition.y = VinitPos.y;
				}
			}
			else
			{
				OnJumpCollider = false;
				HighPositionFlag = false;
				mbjump = false;
				mbjumpDown = false;
			}

			mpModel->Update();
			//mpDH->Update();//drawHp驍ｵ・ｺ繝ｻ・ｮ驛｢・ｧ繝ｻ・｢驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢譎冗樟繝ｻ螳壽Τ繝ｻ・ｼ驍ｵ・ｺ繝ｻ・ｶ
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
			
			// Center
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
			// Left 30 degrees
			VECTOR leftGo = VTransform(GoPosition, MGetRotY(-30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, leftGo, 0, 150);
			// Right 30 degrees
			VECTOR rightGo = VTransform(GoPosition, MGetRotY(30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, rightGo, 0, 150);
		}
		else if (mAttackType == 2)
		{
			mpModel->ChangeAnimation(ANIMATION_ATTACK);
			mpModel->SetLoop(false);
			mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
			mfjumpPower = 400.0f;
			HighPositionFlag = false;
			mbjumpDown = false;
			mbjump = true;
			OnJumpCollider = false;
		}
	}

	if (mAttackType == 0 && mAttack1ComboCount > 0)
	{
		if (now == ANIMATION_NEUTRAL || now == ANIMATION_RUN)
		{
			mpModel->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			mpModel->SetLoop(false);
			mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			// Single magic attack for the 3-hit combo
			new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
			
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
{//髯溷・萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ雋・ｽｽ隶・ｪ鬯ｮ・｢髦ｮ蜷ｶ繝ｻ髯ｷ繝ｻ・ｽ・ｦ鬨ｾ繝ｻ繝ｻ
	if (mfHp <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = mpModel->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpJumpAttackCoiider && check->mpParentObject->GetTag() == Tag3D_Player3D)
		{//mpModel驍ｵ・ｺ繝ｻ・ｮ鬨ｾ・｡繝ｻ・ｪ髯ｷ・ｿ繝ｻ・ｷ驛｢・ｧ郢ｧ繝ｻ譌ｺ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ郢晢ｽｻ繝ｻ迢暦ｽｸ・ｲ遶丞､ｲ・ｿ・ｽ驍ｵ・ｺ繝ｻ・ｩ驍ｵ・ｺ髦ｮ蜷ｶ繝ｻif髫ｴ竏壹・遶頑･｢諤ｦ繝ｻ・･驛｢・ｧ陝ｲ・ｨ遶企・・ｸ・ｺ郢晢ｽｻ
			Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
				{

					pPlayer->Damage(mfAttack);
					AttackHitJudgmentflag = true;//髯溷・萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ雋・∞・ｽ閧ｲ・ｹ譎｢・ｽ・ｼ
				}
			}

		}
	}
}

void EnemyBoss_1::DeathEnemy()
{
	isDead = true;
	mpModel->ChangeAnimation(ANIMATION_DYING);
	//驛｢譎｢・ｽ・ｫ驛｢譎｢・ｽ・ｼ驛｢譎丞ｹｲ郢晢ｽｻ驍ｵ・ｺ髴域喚髮ｷ驍ｵ・ｺ繝ｻ・ｪ驍ｵ・ｺ郢晢ｽｻ
	mpModel->SetLoop(false);
	//驛｢譎｢・ｽ・｢驛｢譎｢・ｽ・ｼ驛｢・ｧ繝ｻ・ｷ驛｢譎｢・ｽ・ｧ驛｢譎｢・ｽ・ｳ髯溷供・ｾ蠕後・髯溯ｼ斐・繝ｻ・ｩ雋・ｽｩ逧ｮ・ｹ譎｢・ｽ・ｼ驛｢・ｧ繝ｻ・ｷ驛｢譎｢・ｽ・ｧ驛｢譎｢・ｽ・ｳ驍ｵ・ｺ繝ｻ・ｫ髫ｰ魃会ｽｽ・ｻ驍ｵ・ｺ郢晢ｽｻ
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



