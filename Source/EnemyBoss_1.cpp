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
#include"SceneGame.h"
#include"GameManager.h"
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
	mnChance = 30;//鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・｢鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・､鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ繝ｻ荳ｻ・ｽ・､繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬩幢ｽ｢隴取得・ｽ・ｳ繝ｻ・ｨ繝ｻ蜿厄ｽｺ・ｽ繝ｻ・ｹ隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬯ｩ蠅捺・繝ｻ・ｽ繝ｻ・ｺ鬯ｩ髦ｪ繝ｻ郢晢ｽｻ
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	
	//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・｢鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ繝ｻ蜿門旭繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬯ｨ・ｾ陟・屮・ｽ・ｻ髦ｮ蜷ｶ繝ｻ
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	model_->AddAnimation(ANIMATION_ATTACKMAGIC, "Resource/Model/MagicAttack.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Jump Attack.mv1");

	model_->SetScale(VGet(4.0f, 4.0f, 4.0f));
	
	mpJumpAttackCoiider = new SphereCollider(this, position_, 400.0f);

	mpDebug = new Debug();

	
}

EnemyBoss_1::~EnemyBoss_1()
{

}

void EnemyBoss_1::Update()
{
	SceneGame* game = Master::mpSceneManager->GetSceneGame();
	if (game && game->mpGameManager) {
		auto phase = game->mpGameManager->GetCurrentPhase();
		if (phase == GameManager::Phase::FADE_OUT_TO_BOSS || phase == GameManager::Phase::FADE_IN_BOSS) {
			return; // フェード中はボスの操作や更新を無効化
		}
	}

	if (isDead)
	{
		DeathEnemy();
	}
	else
	{
		if (model_ != nullptr)
		{
		
			//else
			Attack();
			if (!(model_->GetNowState() == ANIMATION_ATTACK) || !(model_->GetNowState() == ANIMATION_ATTACKJUMP))
			{
				RotationByMove();
				Move();
			}

			if (model_->GetNowState() == ANIMATION_ATTACK && mAttackType == 2)
			{
				if (mfjumpPower >= position_.y && !HighPositionFlag)
				{
					position_ = VAdd(position_, VGet(0.0f, 5.0f, 0.0f));
				}
				if (mfjumpPower <= position_.y)
				{
					HighPositionFlag = true;
					mbjumpDown = true;
				}
				if (HighPositionFlag)
				{
					position_ = VAdd(position_, VGet(0.0f, mfjumpPower, 0.0f));
					mfjumpPower -= 1.0f;
				}

				if (position_.y <= VinitPos.y)
				{
					OnJumpCollider = true;
					position_.y = VinitPos.y;
				}
			}
			else
			{
				OnJumpCollider = false;
				HighPositionFlag = false;
				mbjump = false;
				mbjumpDown = false;
			}

			model_->Update();
			//mpDH->Update();//drawHp鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・｢鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴主・讓溽ｹ晢ｽｻ陞ｳ螢ｽﾎ､郢晢ｽｻ繝ｻ・ｼ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｶ
			CollPositionUpdate();
			mpJumpAttackCoiider->position_ = position_;

		}
	}
}

void EnemyBoss_1::Draw()
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

void EnemyBoss_1::Attack()
{
	AnimationState now = model_->GetNowState();

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
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			// Center
			new Magic_Ene("Resource/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
			// Left 30 degrees
			VECTOR leftGo = VTransform(GoPosition, MGetRotY(-30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, leftGo, 0, 150);
			// Right 30 degrees
			VECTOR rightGo = VTransform(GoPosition, MGetRotY(30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, rightGo, 0, 150);
		}
		else if (mAttackType == 2)
		{
			model_->ChangeAnimation(ANIMATION_ATTACK);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
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
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			// Single magic attack for the 3-hit combo
			new Magic_Ene("Resource/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
			
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
{//鬮ｯ貅ｷ繝ｻ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ髮九・・ｽ・ｽ髫ｶﾂ繝ｻ・ｪ鬯ｯ・ｮ繝ｻ・｢鬮ｦ・ｮ陷ｷ・ｶ郢晢ｽｻ鬮ｯ・ｷ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｦ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ
	if (hp_ <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpJumpAttackCoiider && check->parent_object_->GetTag() == Tag3D_Player3D)
		{//mpModel鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬯ｨ・ｾ繝ｻ・｡郢晢ｽｻ繝ｻ・ｪ鬮ｯ・ｷ繝ｻ・ｿ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢繝ｻ・ｧ驛｢・ｧ郢晢ｽｻ隴鯉ｽｺ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ霑｢證ｦ・ｽ・ｸ繝ｻ・ｲ驕ｶ荳橸ｽ､・ｲ繝ｻ・ｿ繝ｻ・ｽ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｩ鬩搾ｽｵ繝ｻ・ｺ鬮ｦ・ｮ陷ｷ・ｶ郢晢ｽｻif鬮ｫ・ｴ遶丞｣ｹ繝ｻ驕ｶ鬆托ｽ･・｢隲､・ｦ郢晢ｽｻ繝ｻ・･鬩幢ｽ｢繝ｻ・ｧ髯晢ｽｲ繝ｻ・ｨ驕ｶ莨√・繝ｻ・ｸ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
			Player3D* pPlayer = Master::mpPlayer;
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
				{

					pPlayer->Damage(attack_);
					AttackHitJudgmentflag = true;//鬮ｯ貅ｷ繝ｻ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ髮九・竏槭・・ｽ髢ｧ・ｲ繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ
				}
			}

		}
	}
}

void EnemyBoss_1::DeathEnemy()
{
	isDead = true;
	model_->ChangeAnimation(ANIMATION_DYING);
	//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｫ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驛｢譎｢・ｽ・ｻ鬩搾ｽｵ繝ｻ・ｺ鬮ｴ蝓溷繭鬮ｮ・ｷ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
	model_->SetLoop(false);
	//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・｢鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬮ｯ貅ｷ萓帙・・ｾ陟募ｾ後・鬮ｯ貅ｯ・ｼ譁舌・郢晢ｽｻ繝ｻ・ｩ髮九・・ｽ・ｩ騾ｧ・ｮ繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮ｫ・ｰ鬲・ｼ夲ｽｽ・ｽ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
	model_->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (model_->IsAnimationLoopFinish())
	{
		GiveRewards();
		Master::GameClearCount++;
		
		Delete();
		SetDeleteFlag(true);
	}

	model_->Update();
}

void EnemyBoss_1::Delete()
{
	Enemy::Delete();
	if (mpJumpAttackCoiider != nullptr)
	{
		mpJumpAttackCoiider->SetDeleteFlag(true);
		mpJumpAttackCoiider = nullptr;
	}
}



