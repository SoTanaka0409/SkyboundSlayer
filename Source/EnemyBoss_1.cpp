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
	mnChance = 30;//鬯ｩ蟷｢・ｽ・｢郢晢ｽｻ繝ｻ・ｧ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・｢鬯ｩ蟷｢・ｽ・｢郢晢ｽｻ繝ｻ・ｧ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・､鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ郢晢ｽｻ闕ｳ・ｻ繝ｻ・ｽ繝ｻ・､郢晢ｽｻ繝ｻ・ｸ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｮ鬯ｩ蟷｢・ｽ・｢髫ｴ蜿門ｾ励・・ｽ繝ｻ・ｳ郢晢ｽｻ繝ｻ・ｨ郢晢ｽｻ陷ｿ蜴・ｽｽ・ｺ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｹ髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｩ陟・瑳繝ｻ郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｺ鬯ｯ・ｩ鬮ｦ・ｪ郢晢ｽｻ驛｢譎｢・ｽ・ｻ
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	
	//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・｢鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ郢晢ｽｻ陷ｿ髢譌ｭ郢晢ｽｻ繝ｻ・ｸ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｮ鬯ｯ・ｨ繝ｻ・ｾ髯溘・螻ｮ繝ｻ・ｽ繝ｻ・ｻ鬮ｦ・ｮ陷ｷ・ｶ郢晢ｽｻ
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
			return; // 繝輔ぉ繝ｼ繝我ｸｭ縺ｯ繝懊せ縺ｮ謫堺ｽ懊ｄ譖ｴ譁ｰ繧堤┌蜉ｹ蛹・
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
				if (!HighPositionFlag)
				{
					position_ = VAdd(position_, VGet(0.0f, 15.0f, 0.0f));
					if (position_.y >= VinitPos.y + mfjumpPower)
					{
						HighPositionFlag = true;
						mbjumpDown = true;
					}
				}
				else
				{
					position_ = VAdd(position_, VGet(0.0f, -25.0f, 0.0f));
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
			//mpDH->Update();//drawHp鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｮ鬯ｩ蟷｢・ｽ・｢郢晢ｽｻ繝ｻ・ｧ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・｢鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｺｽ・ｹ譎｢・ｽ・ｻ髯橸ｽｳ陞｢・ｽ・趣ｽ､驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｼ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｶ
			CollPositionUpdate();
			mpJumpAttackCoiider->position_ = position_;
			
			// 繝懊せ縺後・繧ｹ繧ｨ繝ｪ繧｢繧医ｊ菴弱￥縺ｪ縺｣縺溷ｴ蜷茨ｼ郁誠縺｡縺溷ｴ蜷茨ｼ峨・繝懊せ繧ｹ繝・・繧ｸ縺ｮ鬮倥＆縺ｫ謌ｻ縺・
			if (position_.y < VinitPos.y)
			{
				position_.y = VinitPos.y;
			}

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
{//鬯ｮ・ｯ雋・ｽｷ郢晢ｽｻ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｸ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・｣鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ鬮ｮ荵昴・繝ｻ・ｽ繝ｻ・ｽ鬮ｫ・ｶ・つ郢晢ｽｻ繝ｻ・ｪ鬯ｯ・ｯ繝ｻ・ｮ郢晢ｽｻ繝ｻ・｢鬯ｮ・ｦ繝ｻ・ｮ髯ｷ・ｷ繝ｻ・ｶ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ繝ｻ・ｷ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｦ鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
	if (hp_ <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpJumpAttackCoiider && check->parent_object_->GetTag() == Tag3D_Player3D)
		{//mpModel鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｮ鬯ｯ・ｨ繝ｻ・ｾ郢晢ｽｻ繝ｻ・｡驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｪ鬯ｮ・ｯ繝ｻ・ｷ郢晢ｽｻ繝ｻ・ｿ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｷ鬯ｩ蟷｢・ｽ・｢郢晢ｽｻ繝ｻ・ｧ鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ髫ｴ魃会ｽｽ・ｺ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・｣鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｦ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ髴托ｽ｢隴会ｽｦ繝ｻ・ｽ繝ｻ・ｸ郢晢ｽｻ繝ｻ・ｲ鬩包ｽｶ闕ｳ讖ｸ・ｽ・､繝ｻ・ｲ郢晢ｽｻ繝ｻ・ｿ郢晢ｽｻ繝ｻ・ｽ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｩ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ鬯ｮ・ｦ繝ｻ・ｮ髯ｷ・ｷ繝ｻ・ｶ驛｢譎｢・ｽ・ｻif鬯ｮ・ｫ繝ｻ・ｴ驕ｶ荳橸ｽ｣・ｹ郢晢ｽｻ鬩包ｽｶ鬯・汚・ｽ・･繝ｻ・｢髫ｲ・､繝ｻ・ｦ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・･鬯ｩ蟷｢・ｽ・｢郢晢ｽｻ繝ｻ・ｧ鬮ｯ譎｢・ｽ・ｲ郢晢ｽｻ繝ｻ・ｨ鬩包ｽｶ闔ｨ竏壹・郢晢ｽｻ繝ｻ・ｸ郢晢ｽｻ繝ｻ・ｺ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
			Player3D* pPlayer = Master::mpPlayer;
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
				{

					pPlayer->Damage(attack_);
					AttackHitJudgmentflag = true;//鬯ｮ・ｯ雋・ｽｷ郢晢ｽｻ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｸ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・｣鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ鬮ｮ荵昴・遶乗ｧｭ繝ｻ繝ｻ・ｽ鬮｢・ｧ繝ｻ・ｲ郢晢ｽｻ繝ｻ・ｹ髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｼ
				}
			}

		}
	}
}

void EnemyBoss_1::DeathEnemy()
{
	isDead = true;
	model_->ChangeAnimation(ANIMATION_DYING);
	//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｫ鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｼ鬯ｩ蟷｢・ｽ・｢髫ｴ諠ｹ・ｸ讖ｸ・ｽ・ｹ繝ｻ・ｲ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ鬯ｮ・ｴ陜捺ｺｷ郢ｭ鬯ｮ・ｮ繝ｻ・ｷ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｪ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	model_->SetLoop(false);
	//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・｢鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｼ鬯ｩ蟷｢・ｽ・｢郢晢ｽｻ繝ｻ・ｧ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｷ鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｧ鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｳ鬯ｮ・ｯ雋・ｽｷ關灘ｸ吶・繝ｻ・ｾ髯溷供・ｾ蠕後・鬯ｮ・ｯ雋・ｽｯ繝ｻ・ｼ隴∬・繝ｻ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｩ鬮ｮ荵昴・繝ｻ・ｽ繝ｻ・ｩ鬨ｾ・ｧ繝ｻ・ｮ郢晢ｽｻ繝ｻ・ｹ髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｼ鬯ｩ蟷｢・ｽ・｢郢晢ｽｻ繝ｻ・ｧ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｷ鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｧ鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢郢晢ｽｻ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｳ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｫ鬯ｮ・ｫ繝ｻ・ｰ鬯ｲ繝ｻ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｽ郢晢ｽｻ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｺ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
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



