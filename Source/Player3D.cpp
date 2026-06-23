#include"Player3D.h"
#include"Model.h"
#include"ModelAnimation.h"
#include"Master.h"
#include"InputManager.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Wall.h"
#include<string>
#include <iostream>
#include <cstring> // strcmp, strncmp
//#include"Effect.h"
#include"Enemy3D.h"
#include"stage.h"


#include"Camera.h"
#include"Camera1.h"
#include"Bush.h"
#include"WeaponManager.h"
#include"Weapon.h"

#include"Shield.h"

#include"Tree.h"
#include"Scene.h"
#include"Texture.h"
#include"Debugh.h"

#include"DrawHp.h"
#include"Effect.h"
#include"DrawCircle1.h"

#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include"InfClass.h"
#include"HaveMoneyClass.h"

// 鬮ｯ蜈ｷ・ｽ・ｻ驛｢譎｢・ｽ・ｻ髴大､ｲ・ｽ・｡鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴乗・・ｽ・ｹ隴∵ｻ・ｱｪE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E髯ｷ阮ｦEE郢晢ｽｻE鬯ｨ・ｾ陋ｹ繝ｻE鬩搾ｽｵE髯ｷ・ｷE郢晢ｽｻ霑｢證ｦ・ｽ・ｸE髣包ｽｵ隴擾ｽｴ郢晢ｽｻ鬯ｮ・ｫE郢晢ｽｻE鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E郢晢ｽｻ陝ｶ譎・鴬郢晢ｽｻE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｿE驛｢譎｢・ｽ・ｻ髯具ｽｹE驛｢譎｢・ｽ・ｧ鬩幢ｽ｢隴弱・隴・搨・ｰ鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴主・讓滄Δ譎｢・ｽ・ｻfalse驛｢譎｢・ｽ・ｻ髣費ｿｽ陋ｹ繝ｻE郢晢ｽｻE鬯ｨ・ｾ陋ｹ繝ｻE鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ驕ｶ逡窪E驛｢譎｢・ｽ・ｻ郢晢ｽｻE髣費ｽｨ陞滂ｽｲEE驛｢譎｢・ｽ・ｻ
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool isSeparateAnim)
	:Object3D(initPos)
	, mfAttack(0)//鬩幢ｽ｢隰ｨ螟慚碑ｭ趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE
	, mfAttackjump(5)
	, mfAttackSlide(7)

	,OnJumpCollider(false)//鬩搾ｽｵE鬮ｦ・ｮ陷ｷ・ｶ郢晢ｽｻ鬮ｴ謇假ｽｽ・ｸ郢晢ｽｻE鬯ｯ・ｮE鬮ｦ・ｮ陷ｷ・ｮ郢晢ｽｻ鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E郢晢ｽｻ陜｣・､EE郢晢ｽｻE鬩搾ｽｵE髣比ｼ夲ｽｽ・｣郢晢ｽｻ郢晢ｽｻ
	, mbInvisible(false)//鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻ驕ｯ・ｶE鬯ｯ・ｩE髯晢ｿｽ髮懶ｽ｣EE驛｢譎｢・ｽ・ｻ郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ髢ｧ・ｲEE驛｢譎｢・ｽ・ｻ驕ｶ逡・
	, mbjump(false)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(speed)//speed
	, JUMP_POWER(jumppower)
	, mfHp(hp)//hp
	, mfMaxHp(hp)//maxhp
	, mfDashSpeed(speed * 2)
	, NewShieldFast(false)//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴取得・ｽ・ｳE郢晢ｽｻ陝ｶ譎擾ｽ｢髮｣・ｽ・ｻ髦ｮ蜒樣ｩ搾ｽｵE髯ｷ・ｷE郢晢ｽｻ霑｢證ｦ・ｽ・ｸE髣包ｽｵ隴擾ｽｶ郢晢ｽｻ鬩搾ｽｵE驛｢譎｢・ｽ・ｻE繝ｻ・ｰ
	, mfSize(60.0f)//鬩搾ｽｵE驛｢・ｧ郢晢ｽｻ髯橸ｽｺ鬩幢ｽ｢E鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ驛｢譎｢・ｽ・ｻ
	, Pause(false)//鬩幢ｽ｢隴弱・驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬯ｨ・ｾ陋ｹ繝ｻE鬯ｯ・ｮE郢晢ｽｻE
	, ChangeCamera(0)//鬯ｮ・ｫ驕ｨ繧托ｽｽ・ｹ雋・ｪ髮ｷ鬮ｯ讓奇ｽｺ・ｽ陋ｻ・､髯晢ｽｲE
	, DashGuage(100.0f)//
	, EneSerchCount(0)
	, Gpush(false)//鬮ｯ貊ゑｽｽ・｢驛｢譎｢・ｽ・ｻ郢晢ｽｻE郢晢ｽｻE鬮ｫ・ｰ陜｣遏ｩﾎ忍髯句ｹ｢・ｽ・ｵ郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ隶捺慣・ｽ・ｸE驛｢譎｢・ｽ・ｻ
	, GpushCount(2)//鬮ｯ貊ゑｽｽ・｢驛｢譎｢・ｽ・ｻ郢晢ｽｻE郢晢ｽｻE鬮ｫ・ｰ陜｣遏ｩ縺勇郢晢ｽｻE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ
	, EffectUflag(false)
	, EffectUCount(0)
	, mfRideOldHp(0)
	, mfjumpPower(150.0f)
	, mfAttackSlideSpeed(20.0f)

	, AttackSlideCount(0)
	, AttackJumpCount(0)
	,AttackCount(0)

	, ChangeStateCount(0)
	, mbjumpDown(false)
	, HighPositionFlag(false)
	, AttackHitJudgmentflag(false)
	, mbAttackSlideSerch(false)
	, mfEvasionSpeed(20.0f)
	,mbStageOutFlag(true)
	//, mnLose(false)
	//, mnWin(false)
{
	mpEffectU = new EffectU();
	mpEffectU->Load();


	SetTag(Object3D::Tag3D_Player3D);
	mpBuffManager = new BuffManager();
	mpWeaponManager = new WeaponManager();
	mpEquipmentManager = new EquipmentManager();
	mpShortInventory = new ShortInventory();
	mpLevelUp = new LevelUp();
	mpModel = new Model(filename, initPos, isSeparateAnim);
	mpHaveMoney = new HaveMoneyClass(30000);
	mpModel->AddAttachment("Resource/3D/Sabel.mv1", "wp");
	mpModel->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	mpModel->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	mpModel->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Attack_3.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKSLIDE, "Resource/Model/Attack.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/Model/Jump Attack.mv1");
	mpModel->AddAnimation(ANIMATION_SLIDE, "Resource/Model/kaihi.mv1");

	Master::mpCamera->Initialize();

	float HpRatio = (float)mfHp / mfMaxHp;
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;
	{//鬮ｯ蜈ｷ・ｽ・ｻ髫ｴ谿ｺEEE驛｢譎｢・ｽ・ｻ郢晢ｽｻ郢晢ｽｻ
		Weapon::WeaponDate* weaponInf;
		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 10;
		weaponInf->filename = "Resource/3D/Sabel.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon1;
		weaponInf->name = "weapon1";
		mpWeaponManager->AddWeapon(weaponInf);
		mpWeaponManager->ChangeWeapon(weaponInf);

		mfNormalAttack = weaponInf->damage;//鬮ｫ・ｴ陝�・｢鬮ｯ蜈ｷ・ｽ・ｻ髫ｴ謫ｾ・ｽ・ｴ驕ｶ鬆托ｽ･・｢陝�謌奇ｽｭ谿ｺEEE驛｢譎｢・ｽ・ｻ郢晢ｽｻ陷･謫ｾ・ｽ・ｸE郢晢ｽｻE鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻ鬯ｲ蛛・ｽｽ・ｨ鬩搾ｽｵE郢晢ｽｻE鬮ｯ蜈ｷ・ｽ・ｻ髫ｴ谿ｺEEE鬯ｮ・｢驛｢譎｢・ｽ・ｻ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ郢晢ｽｻ陝ｶ譎剰ｷ晞辧蜍滂ｽｨ・ｯ鬲假ｽｬ
		mfAttack = weaponInf->damage;//鬮ｫ・ｴ陝�・｢鬮ｯ蜈ｷ・ｽ・ｻ髫ｴ謫ｾ・ｽ・ｴ驕ｶ鬆托ｽ･・｢陝�謌奇ｽｭ谿ｺEEE驛｢譎｢・ｽ・ｻ郢晢ｽｻ陷･謫ｾ・ｽ・ｸE郢晢ｽｻE鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻ鬯ｲ蛛・ｽｽ・ｨ鬩搾ｽｵE郢晢ｽｻE鬮ｯ蜈ｷ・ｽ・ｻ髫ｴ谿ｺEEE鬯ｮ・｢驛｢譎｢・ｽ・ｻ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ郢晢ｽｻ陝ｶ譎剰ｷ晞辧蜍滂ｽｨ・ｯ鬲假ｽｬ
	}




	MaxDashGauge = DashGuage;

	
	 // 鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隰ｨ螟慚碑ｭ趣ｽ｢EE鬯ｨ・ｾ陟・屮・ｽ・ｻ髦ｮ蜒・
	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 60.0f);
	mpAttackSlideCollider = new SphereCollider(this, mvPosition, 200.0f);
	mpSerchEnemyCollider = new SphereCollider(this, VAdd(mvPosition, VGet(0.0f, 120.0f, 0.0f)), 500.0f);
	mpAttackJumpCollider = new SphereCollider(this, VAdd(mvPosition, VGet(0.0f, 120.0f, 0.0f)), 300.0f);
	
	
	
	AttackState = Attack_Normal;
	mpItemManager = Master::mpItemManager;
}

Player3D::~Player3D()
{
	delete mpModel;
	delete mpShortInventory;
	CollDelete();
}

void Player3D::Update()
{
	if (mnInvincibleTimer > 0) {
		mnInvincibleTimer--;
	}

	mpShortInventory->Update();
	if (Master::ShopClassOn || Master::StatShopClassOn)return;
	///////////////////////////////////////////////
	

	if (false)
	{
		
		Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽE鬮ｫ・ｴE髫ｲ讖ｸ・ｽ・ｺ髮趣ｽｬE
	}
		
	
	if (Master::PauseOn == false)//鬩幢ｽ｢隴弱・驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬮｣蛹・ｽｽ・ｳ郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ・ｷ陝�雜｣・ｽ・ｼ隴夲ｽｿE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ髢ｧ・ｲEE驛｢譎｢・ｽ・ｻ驕ｶ莨∬ｱｪEE髯ｷ・ｷE郢晢ｽｻ郢晢ｽｻ
	{
		if (mpTarget != nullptr)
		{
			bool isTargetValid = false;
			auto mpEneList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
			for (int i = 0; i < mpEneList.size(); i++)
			{
				if (mpTarget == mpEneList.at(i) && !mpTarget->IsDeleteFlag())
				{
					isTargetValid = true;
					break;
				}
			}
			if (!isTargetValid)
			{
				mpTarget = nullptr;
			}
		}
		Test();
		ManagerUpdate();
		if (InputManager::CheckDownKey(KEY_INPUT_Q))
		{
			ChangeView();
		}
		////////////////////////鬯ｮ・ｯE髫ｰ逍ｲ・ｺ・ｯE////////////////
		SelectAttack();
		Result();
		ResetNUETRAL();
		//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴弱・隲帷阜鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ
		CollPositionUpdate();
		Evasion();
		//鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ
		SelectAttack();
		//鬯ｩ蜍溪・EE鬮ｯ・ｷ隶手ｴ具ｽｾ蟶ｷ・ｹ譎｢・ｽ・ｻ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ
		MoveEx();
		//鬮ｯ諛・ｻｸE郢晢ｽｻEE郢晢ｽｻE鬮ｯ・ｷ郢晢ｽｻEE鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ
		RotationByMove();
		EnemySerch();

		if (mpLevelUp->GetLevelUp())
		{ 
			Master::mpSoundManager->PlaySE(SoundManager::SE_LEVELUP);
			mfHp = GetAllStatusState(Object3D::Status_Hp);//hp鬩幢ｽ｢E鬮ｮ蛹ｺ・ｧ・ｭ郢晢ｽｻ鬮ｯ諛・ｻｸEE郢晢ｽｻ郢晢ｽｻEE髯晢ｽｶ陷ｻ・ｻE郢晢ｽｻ
		}
		mpModel->Update();
	}

}

void Player3D::ResetNUETRAL()
{
	AnimationState now = mpModel->GetNowState();
	if (now == ANIMATION_NEUTRAL)
	{
		if (mpModel->GetIsSeparate() == true)
		{
			mpModel->mpSeparateAnimation->SetAnimationCount(0.5f);
		}
		else
		{
			mpModel->mpAnimation->SetAnimationCount(0.5f);
		}


		FirstNearVec = 0;//鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻ郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ驕ｶ髮・ｿｽ・､EE鬯ｮ・ｦE驕ｶ鬆托ｽ･・｢EE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬯ｮ・ｴ髢ｧ・ｴ髴趣ｽｨ髯橸ｽｻE鬩幢ｽ｢E髯ｷ・ｻ髣鯉ｽｨEE郢晢ｽｻE鬩幢ｽ｢E髣包ｽｵ隴擾ｽｴ遯ｶ・ｳ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴主・讓溽ｹ晢ｽｻ陜｣・､E隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｨ
		mpTarget = nullptr;//鬯ｨ・ｾE郢晢ｽｻE鬮ｫ・ｶ霓｣蛟｡蜃ｽ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｨ鬩幢ｽ｢E髯句ｹ｢・ｽ・ｵE隲帙えE郢晢ｽｻE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｨ

	}
}

void Player3D::ManagerUpdate()
{
	mpBuffManager->DeleteList();
	mpBuffManager->Update();
	mpLevelUp->Update();
	
}

void Player3D::Draw()
{
	if (mfHp <= 0)return;
	if (Master::ShopClassOn || Master::StatShopClassOn)return;
	if (Master::PauseOn == false)///驛｢譎・ｺ｢郢晢ｽｻ驛｢・ｧE鬨ｾ蛹・ｽｽ・ｻ鬯ｮ・ｱE驍ｵ・ｺE驍ｵ・ｺE驍ｵ・ｺE驍ｵ・ｺ郢晢ｽｻ遶企・・ｹ・ｧ髢ｾ・･E隴弱・驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬯ｨ・ｾ陋ｹ繝ｻE鬯ｯ・ｮE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ驕ｶ髮・ｿｽ・､EE驛｢譎｢・ｽ・ｻ
	{
		
		bar();
		
	}
	////////////////////////鬮｣蛹・ｽｽ・ｳ髫ｰ譴ｧ豎哘E郢晢ｽｻE鬯ｩ蜍溪・EE鬯ｮ・ｫ驕ｨ繧托ｽｽ・ｹ雋・ｪ髮ｷ鬩搾ｽｵE郢晢ｽｻE鬮ｫ・ｴ陟托ｽｱEE繝ｻ・ｻ鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲE隶捺慣・ｽ・ｹE郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE髴托ｽｹ陞滂ｽｲEE髣包ｽｵ隴擾ｽｶ隴・ｽ｡鬩幢ｽ｢E髣包ｽｵ隴趣ｽ｢E髢ｧ・ｲEE驛｢譎｢・ｽ・ｻ驕ｶ莨∬ｱｪEE髯ｷ・ｷE郢晢ｽｻ郢晢ｽｻ//////////////
	if (Master::mpCamera->GetCamera3() == true)
	{
		//鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻE隴主承E郢晢ｽｻE鬮ｫ・ｰE髯ｷﾂ隲､蟒｢
		mpModel->Draw();
	}

	if (Master::mpDebug->Getdebug() == true)
	{
		//鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢隴弱・驛｢譎｢・ｽ・｣鬩幢ｽ｢E郢晢ｽｻE鬯ｮ・ｯE郢晢ｽｻE鬯ｩ遨ゑｽｼ螟ｲ・ｽ・ｽE鬮ｫ・ｴ郢晢ｽｻEE鬮ｮ謇具ｽｼ蠢・
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
		DrawFormatString(100, 300, GetColor(255, 255, 255), "鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻ鬯ｲ蛛・ｽｽ・ｨ:%f", GetAllStatusState(Object3D::Status_Attack));
		DrawFormatString(100, 400, GetColor(255, 255, 255), "鬯ｯ・ｮE郢晢ｽｻE鬮ｯ貅ｷ譯・E鬮ｯ・ｷ陝ｲ・ｨ郢晢ｽｻ%f", mpEquipmentManager->GetDamage());
		DrawFormatString(100, 500, GetColor(255, 255, 255), "s鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴弱・驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ%f", GetAllStatusState(Object3D::Status_Speed));
		DrawFormatString(100, 350, GetColor(255, 255, 255), "鬯ｩ謳ｾ・ｽ・ｨ鬩戊┳EE髴難ｽ｣陋滂ｽｪ邵ｺ谿ｺE:%d", mpLevelUp->GetXp());
		DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f       Z:%f", mvPosition.x, mvPosition.y, mvPosition.z);
	}
	//鬮ｮ谿ｺEE鬮ｯ諛・ｽｶ・｣EE鬩幢ｽ｢E鬮ｮ蛹ｺ・ｨ螂・ｽｽ・｡遶擾ｽｫEEE鬩幢ｽ｢E髯具ｽｹE驕ｶ蛹・ｽｽ・ｧ鬩搾ｽｵE郢晢ｽｻE鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ郢晢ｽｻE驛｢譎｢・ｽ・ｻ
	DrawSphere3D(
		mpModel->GetAttachmentPosition(),
		30.0f,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

//鬯ｩ蜍溪・EE鬮ｯ・ｷ隶手ｴ具ｽｾ蟶ｷ・ｹ譎｢・ｽ・ｻ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ驍ｵ・ｺ陝ｶ・ｷE隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ驛｢譎｢・ｽ・ｻ
void Player3D::MoveEx()
{
	AnimationState state = mpModel->GetNowState();
	if (state==ANIMATION_ATTACKJUMP||state == ANIMATION_ATTACK || state == ANIMATION_JUMP_OUT || state == ANIMATION_SLIDE||state==ANIMATION_ATTACKSLIDE||Master::ShopClassOn||Master::StatShopClassOn)
	{
		return;
	}//鬮ｴ螟ｧ・､・ｲEE鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬮｣蛹・ｽｽ・ｳ郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢・ｧ郢晢ｽｻE隶呵ｶ｣・ｽ・ｸE郢晢ｽｻE鬮｣蜴・ｽｽ・ｴ鬮ｴ驛・ｽｲ・ｻE郢ｧ莨夲ｽｽ・ｸE髯ｷ莨夲ｽｽ・ｱ驕ｶ逡窪E驛｢譎｢・ｽ・ｻ

	moveVec = VGet(0.0f, 0.0f, 0.0f);//鬯ｩ蜍溪・EE鬮ｯ・ｷ隶主･・ｽｽ・｢霓｣蛛・ｽｽ・ｩE鬮ｯ・ｷE驛｢譎｢・ｽ・ｻ
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮｣蛹・ｽｽ・ｳEEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸E
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻE鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸E
	//VECTOR DownMoveVector = VGet(0.0f, 0.0f, 0.0f);//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮｣蛹・ｽｽ・ｳ髯具ｽｹE髯敖E鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸E
	//VECTOR RightMoveVector = VGet(0.0f, 0.0f, 0.0f);//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｯ・ｷE郢晢ｽｻE鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸E


	//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驕ｯ・ｶE鬩搾ｽｵE髣包ｽｵ隴趣ｽ｢E髯具ｽｾ・ゑｽｧ郢晢ｽｻE鬮ｯ・ｷ陝�雜｣・ｽ・ｼ隴∬・鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸隴主承E髯ｷ・ｻ髣鯉ｽｨEE驛｢・ｧ郢晢ｽｻE遶擾ｽｫEE驛｢譎｢・ｽ・ｻ
	{
		//鬮｣蛹・ｽｽ・ｳEEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驕ｶ蜑ｰ縺忍E郢晢ｽｻE鬯ｩ蜍溪・EE鬮ｯ・ｷ陝�雜｣・ｽ・ｼ隴∬・鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸隴主承E郢晢ｽｻE鬩搾ｽｵE驕ｶ荳橸ｽ｣・ｹ遯ｶ・ｳ鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬯ｮ・ｫ驕ｨ繧托ｽｽ・ｹ雋・ｪ髮ｷ鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣E繝ｻ・ｰ鬩幢ｽ｢E鬮ｫ・ｰ遶擾ｽｬE陟包ｽ｡E陷ｷ・ｶ郢晢ｽｻ鬩幢ｽ｢E髯ｷ・ｻ陜捺ｪｸ鬩搾ｽｵE驛｢譎｢・ｽ・ｻ髫ｨ・ｳ郢晢ｽｻEE驛｢・ｧ郢晢ｽｻ郢晢ｽｻ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE髯ｷ・ｷE郢晢ｽｻ郢晢ｽｻ
		UpMoveVector = VSub(Master::mpCamera->GetlookAtPosition(), Master::mpCamera->GetPosition());

		//鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻE鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驕ｶ蜑ｰ縺忍E郢晢ｽｻE鬯ｩ蜍溪・EE鬮ｯ・ｷ陝�雜｣・ｽ・ｼ隴∬・鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸隴主承E髯溷供・ｨ・ｯ邵ｺ螳茨ｽｫ謫ｾ・ｽ・ｽ郢晢ｽｻEEEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬯ｩ蜍溪・EE鬮ｯ・ｷ陝�雜｣・ｽ・ｼ隴∬・鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸隴主承E郢晢ｽｻE鬩搾ｽｵE驍ｵ・ｲ郢晢ｽｻ隲､鬮ｭE鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲE陝ｶ・ｷEE郢晢ｽｻE鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驕ｶ蜑ｰ縺忍E郢晢ｽｻE鬩幢ｽ｢隴主・蜃ｽ驍ｵ・ｺ鬩｢謳ｾ・ｽ・ｹ隴主・讓檸隴主承E郢晢ｽｻE鬮ｯ諛ｷ逕檸髯晢ｽｲE鬩搾ｽｵE郢晢ｽｻE鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髫ｰ繝ｻEE髣費ｿｽE郢晢ｽｻE髣包ｽｵ雋翫ｑ・ｽ・ｽE髫ｰ・ｳE郢晢ｽｻE驛｢譎｢・ｽ・ｻ
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));


		//鬯ｩ蜍溪・EE鬮ｯ・ｷ陝�雜｣・ｽ・ｼ隴∬・鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸隴主承E郢晢ｽｻE鬯ｩ蜍溪・EE鬮ｯ・ｷ髢ｧ・ｴ陜ｮ・ｩE鬩幢ｽ｢E鬮ｮ蛹ｺ・ｧ・ｫ郢晢ｽｻ鬮ｯ・ｷE郢晢ｽｻE鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ驕ｶ逡窪E驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驕ｶ謫ｾ・ｽ・ｵ郢晢ｽｻE郢晢ｽｻE鬯ｮ・ｫ驕ｨ繧托ｽｽ・ｸ隶抵ｽｫEE髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶE鬩搾ｽｵE鬩怜遜・ｽ・ｫ郢晢ｽｻE(鬩幢ｽ｢隴主・蜃ｽ驍ｵ・ｺ鬩｢謳ｾ・ｽ・ｹ隴主・讓檸隴主承E郢晢ｽｻE鬯ｯ・ｮ雋翫ｑ・ｽ・ｽE鬩搾ｽｵE鬮ｴ驛・ｽｲ・ｻE陋幢ｽｵ郢晢ｽｻ髣比ｼ夲ｽｽ・｣驕ｶ莨∬ｱｪEE髯ｷ・ｷE郢晢ｽｻ霑｢證ｦ・ｽ・ｸE鬮ｦ・ｮ陷ｷ・ｮ郢晢ｽｻ)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	//VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);//鬯ｩ蜍溪・EE鬮ｯ・ｷ隶主･・ｽｽ・｢霓｣蛛・ｽｽ・ｩE鬮ｯ・ｷE驛｢譎｢・ｽ・ｻ

	if (CheckHitKey(KEY_INPUT_A))//鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻE鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驕ｶ蜑ｰ縺忍E郢晢ｽｻE鬯ｩ蜍溪・EE鬯ｯ・ｨE驛｢譎｢・ｽ・ｻ
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_D))//鬮ｯ・ｷE郢晢ｽｻE鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驕ｶ蜑ｰ縺忍E郢晢ｽｻE鬯ｩ蜍溪・EE鬯ｯ・ｨE驛｢譎｢・ｽ・ｻ
	{
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));

	}
	if (CheckHitKey(KEY_INPUT_W))//鬮ｯ讒ｭ・薦E鬮ｫ・ｴ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驕ｶ蜑ｰ縺忍E郢晢ｽｻE鬯ｩ蜍溪・EE鬯ｯ・ｨE驛｢譎｢・ｽ・ｻ
	{
		moveVec = VAdd(moveVec, UpMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_S))//鬮ｫ・ｰ郢晢ｽｻ陷夲ｽｱ髴趣ｽｯ隰悟･・ｽｽ・ｭ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驕ｶ蜑ｰ縺忍E郢晢ｽｻE鬯ｩ蜍溪・EE鬯ｯ・ｨE驛｢譎｢・ｽ・ｻ
	{
		moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));

	}
	//鬯ｩ蜍溪・EE鬮ｯ・ｷ陝�雜｣・ｽ・ｼ髮具ｽｻEE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ驍・私・ｽ・ｿE郢晢ｽｻE鬮ｫ・ｲE髣包ｽｵ隴擾ｽｴ陜｣・､EE驛｢・ｧ郢晢ｽｻE隶呵ｶ｣・ｽ・ｸE郢晢ｽｻE
	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{

		//鬯ｩ蜍溪・EE鬮ｯ・ｷ隶主･・ｽｽ・｢霓｣蛛・ｽｽ・ｩE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣郢晢ｽｻ陞ｳ螟ｲ・ｽ・ｱ郢晢ｽｻEE鬯ｮ・ｫ驕ｨ繧托ｽｽ・ｸ隶抵ｽｫEE髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶE鬩搾ｽｵE鬩怜遜・ｽ・ｫ郢晢ｽｻE
		moveVec = VNorm(moveVec);

		//鬮ｫ・ｴ郢晢ｽｻEE鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ郢晢ｽｻ隶壺扱邯憺垳蜩櫓E郢晢ｽｻE鬩幢ｽ｢E髯句ｹ｢・ｽ・ｵ驍ｵ・ｺ隴惹ｼ夲ｽｽ・ｹ隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｨ
		mfTargetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;//鬩搾ｽｵE鬮ｦ・ｮ陷ｻ・ｻE郢晢ｽｻEE郢晢ｽｻE鬮｣蠢孔E驍ｵ・ｺ陷證ｦ・ｽ・ｹ隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢E郢晢ｽｻE鬩包ｽｯE鬮ｴ蝓溷繭E鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣郢晢ｽｻ隶捺慣・ｽ・ｸE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ驍・私・ｽ・ｭ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣郢晢ｽｻ陞ｳ螢ｽﾂ谿ｺE鬩搾ｽｵE髯ｷ・ｷEE驛｢・ｧ郢晢ｽｻEE髫ｴ蜿門ｾ勇E驛｢譎｢・ｽ・ｻ
	}
	//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴取ｨ雁袖髯晢ｽｷ隰碁�托ｽｲ繧雁ｵｯ隴擾ｽｴ郢晢ｽｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴乗・・ｽ・ｹ隴∵ｻ・ｱｪEE髯区ｻゑｽｽ・･郢晢ｽｻ郢晢ｽｻEE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢・ｧ郢晢ｽｻE隶呵ｶ｣・ｽ・ｸE郢晢ｽｻE
	if (state != ANIMATION_JUMP_IN || state != ANIMATION_JUMP_LOOP)
	{
		if (isMove)
		{
			mpModel->ChangeAnimation(ANIMATION_RUN);
		}
		else
		{
			mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
		}
	}



	//鬯ｩ蜍溪・EE鬮ｯ・ｷ陝�雜｣・ｽ・ｼ髮具ｽｻE郢晢ｽｻEE髯晢ｽｶ陷ｻ・ｻE郢晢ｽｻ
	mvOldPosition = mvPosition;//鬮ｯ・ｷ髫ｨ・ｬE鬮ｮ繝ｻ鬪ｰ蜈ｷ・ｽ・ｸE郢晢ｽｻE鬮ｯ貅ｯ・ｶ・｣EE鬮ｫ・ｶ霓｣蛟｡蜃ｽ郢晢ｽｻ陞ｳ螢ｼ蛻戲鬮ｫ・ｴ鬲・ｼ夲ｽｽ・ｽE鬮｣蜴・ｽｽ・ｫ髫ｴ谿ｺE


	mvPosition = VAdd(mvPosition, VScale(moveVec, GetAllStatusState(Object3D::Status_Speed)));
	////////////////////////////////////////////////////////



		/////鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E郢晢ｽｻ陜｣・､EE髯ｷ・ｷE郢晢ｽｻ郢晢ｽｻ///////////
	hitPos = VGet(0.0f, 0.0f, 0.0f);
	bool isHit = false;
	auto obj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Stage);
	for (int i = 0; i < obj.size(); i++)
	{
		Stage* pStage = dynamic_cast<Stage*>(obj.at(i));
		if (pStage != nullptr)
		{
			
			if (pStage != nullptr)
			{
				//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲE隶捺慣・ｽ・ｹE郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驍ｵ・ｺ隴惹ｼ夲ｽｽ・ｹ隴趣ｽ｢EE鬩搾ｽｵE髫ｰ逍ｲ・ｻ繧托ｽｽ・ｽE鬮ｦ・ｮ陷ｷ・ｮ髯橸ｽｺ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ驍・戟謐礼ｹ晢ｽｻE鬮ｯ・ｷE驛｢譎｢・ｽ・ｻ
				if (pStage->CheckHit_Capsule(VAdd(mvPosition, VGet(0.0f, -150.0f, 0.0f)), VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
				{
					//鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ霑｢證ｦ・ｽ・ｸE郢晢ｽｻE鬩搾ｽｵE驛｢・ｧ郢晢ｽｻE陷･・ｲEE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｫ・ｰ隴会ｽｦEE鬯ｮ・ｫ隴会ｽｦEE鬮ｴ髮｣・ｽ・､郢晢ｽｻE鬩幢ｽ｢E髯ｷ・ｻ髣鯉ｽｨEE驛｢・ｧ郢晢ｽｻE遶擾ｽｫEE驛｢譎｢・ｽ・ｻ
					hitPos = pStage->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, 1000.0f, 0.0f)),//鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲE隶捺慣・ｽ・ｹE郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬯ｮ・｢E髫ｴ蜿門ｾ勇E鬮ｦ・ｮ陷ｷ・ｮ髯橸ｽｺ鬩幢ｽ｢E驛｢譎｢・ｽ・ｻ鬮ｯ讓奇ｽｺ・ｷEE驛｢譎｢・ｽ・ｻ)鬩搾ｽｵE郢晢ｽｻE
						VAdd(mvPosition, VGet(0.0f, -1000.0f, 0.0f))//鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲE隶捺慣・ｽ・ｹE郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｯ謌ｲEE郢晢ｽｻE鬮｣蛹・ｽｽ・ｳ髣包ｽｵ隴擾ｽｶ隴鯉ｽｺ鬩搾ｽｵEEE鬯倩ｲｻ・ｽ・ｹE髯懶ｽ｣E郢晢ｽｻE髯橸ｽ｢E驛｢譎｢・ｽ・ｻ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶE鬮ｫ・ｰ隰費ｽｶ郢晢ｽｻ郢晢ｽｻE驛｢譎｢・ｽ・ｻ
					);

					//鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE郢晢ｽｻE鬩搾ｽｵEE髫ｲ・｢E髯橸ｽ｢E郢晢ｽｻ陞ｳ螢ｽ笊る垈雋ｻ・ｽ・ｶE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE鬩怜遜・ｽ・ｫ郢晢ｽｻE
					isHit = true;
				}
			}
		}
	}

	if (isHit)
	{
		//鬮ｯ諛ｶ・ｽ・ｨ郢晢ｽｻE鬯ｯ・ｮE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｮ蜿冶・EE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｮ谿ｺEE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ驕ｯ・ｶE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ驍・私・ｽ・ｿE郢晢ｽｻE鬮ｫ・ｲE髣包ｽｵ隴擾ｽｶ郢晢ｽｻ鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶE鬩搾ｽｵE驍ｵ・ｲ郢晢ｽｻE髮懶ｽ｣EE鬮ｫ・ｶ霓｣蛟｡蜃ｽ郢晢ｽｻ陜｣・､EE郢晢ｽｻE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ・ｷE髯具ｽｹE郢晢ｽｻ陷證ｦ・ｽ・ｸE髯晢ｽｶ陷ｻ・ｻE郢晢ｽｻ
		mvPosition.y = hitPos.y;
	}
	if (isHit == false)
	{
		//鬯ｮ・ｫ髯ｬ諛域｡ｶ鬯ｮ・ｫE鬮ｯ諛茨ｽｺ讚・
		mvPosition.y += -8.0f;//鬯ｮ・｣隲幢ｽｶEE鬮｣蛹・ｽｽ・ｳ髣包ｽｵ隴擾ｽｶ郢晢ｽｻ鬩幢ｽ｢E鬩墓得・ｽ・ｩEE郢晢ｽｻE郢晢ｽｻE
		if (mvPosition.y <= 0.0f || mvPosition.y <= hitPos.y)
		{
			mvPosition.y = hitPos.y;
		}
	}

	/////////////////////////鬮ｯ讖ｸ・ｽ・｢驕ｶ荳橸ｽ｣・ｹ郢晢ｽｻ鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ驛｢譎｢・ｽ・ｻ///////////////////////////
	bool hitwall = false;
	bool hitwalls = false;
	auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
	if (!walls.empty())
	{
		// 鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ郢晢ｽｻ驍・戟・､・ｶ驕ｶ荳橸ｽ､・ｲE陝ｶ譎乗套郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢E驛｢譎｢・ｽ・ｻ
		// hint: 鬮ｴ謇假ｽｽ・ｴ郢晢ｽｻE鬮ｴ謇假ｽｽ・･郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ・ｷ郢晢ｽｻEE鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ驍ｵ・ｲ陜｣・､EE郢晢ｽｻE1鬮ｫ・ｴE髯橸ｽ｢E驛｢譎｢・ｽ・ｻ鬮ｯ讖ｸ・ｽ・｢驕ｶ荳橸ｽ､・ｲEE鬩搾ｽｵE髯具ｽｹE髫ｲ・､陷･蠑ｱ笙驛｢・ｧ霑壼虫・｣・ｭ鬩搾ｽｵE郢晢ｽｻE鬮ｯ蜈ｷ・ｽ・ｻ郢晢ｽｻE鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E郢晢ｽｻ郢晢ｽｻEE髯溷供・ｨ・ｯ郢晢ｽｻ鬩搾ｽｵE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ鬮ｫ・ｴE髯樊ｻゑｽｽ・ｲ郢晢ｽｻE髣費ｿｽ陋ｹ繝ｻE郢晢ｽｻE鬮｣蛹・ｽｽ・ｳ髯槭・EE髯晢ｽｲE驕ｶ鬆托ｽ･・｢E陷・ｶ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ髫ｨ・ｳ郢晢ｽｻ隰ｦ・ｻ郢晢ｽｻE鬮ｯ・ｷE髯具ｽｹE驛｢譎｢・ｽ・ｻ鬮ｯ・ｷ郢晢ｽｻEE鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ郢晢ｽｻ陝ｶ譎∫ｮ秘Δ譎｢・ｽ・ｻ驕ｶ謫ｾ・ｽ・ｴ鬩幢ｽ｢E驛｢譎｢・ｽ・ｻ
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				// 鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲE隶捺慣・ｽ・ｹE郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E鬮ｮ蛹ｺ・ｨ螂・ｽｽ・｡遶擾ｽｫEEE鬩幢ｽ｢E髯具ｽｹE驕ｶ蛹・ｽｽ・ｧ鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驍ｵ・ｺ隴惹ｼ夲ｽｽ・ｹ隴趣ｽ｢EE鬮ｯ諛ｷ蟋摘隴擾ｽｴ郢晢ｽｻ鬮ｯ蜈ｷ・ｽ・ｻ郢晢ｽｻE鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E驕ｶ髮・ｿｽ・､EE驕ｶ荵怜款EE驕ｶ荳橸ｽ｣・ｹ郢晢ｽｻ鬮｣蛹・ｽｽ・ｳE郢晢ｽｻE鬮ｮ蛹ｺ・ｩ・ｸEE郢晢ｽｻE鬩幢ｽ｢隴弱・E隲帙えE郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E郢晢ｽｻ陝ｶ譎剰ｷ晞辧蜍滂ｽｨ・ｯ鬲假ｽｬ
				if (HitCheck_Capsule_Triangle(
					mvPosition,
					VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
					80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						mvPosition,
						VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					int WallFontSize = GetFontSize();
					SetFontSize(40);
// removed
// removed
					// 鬮ｯ讖ｸ・ｽ・｢驕ｶ荳橸ｽ｣・ｺ郢晢ｽｻ鬮ｮ蜿冶・EE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬯ｮ・ｯE髯滓坩・ｯ莨夲ｽｽ・ｿE鬩幢ｽ｢E髯具ｽｹE驕ｶ蛹・ｽｽ・ｧ鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢隴主・蜃ｽ驍ｵ・ｺ鬩｢謳ｾ・ｽ・ｹ隴主・讓檸隴主承E鬮ｮ蛹ｺ・ｧ・ｫ陟募ｮ｣霎ｧ髴亥ｿ・
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 鬮ｯ讖ｸ・ｽ・｢驕ｶ謫ｾ・ｽ・ｵ郢晢ｽｻE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸E
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 鬯ｩ蜍溪・EE鬮ｯ・ｷ隶主･・ｽｽ・｢霓｣蛛・ｽｽ・ｩE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸隴主承E郢晢ｽｻE鬮ｯ・ｷE髫ｶ谿ｺE郢晢ｽｻE鬩幢ｽ｢隴主・蜃ｽ驍ｵ・ｺ鬩｢謳ｾ・ｽ・ｹ隴主・讓檸隴主承E郢晢ｽｻE鬩搾ｽｵE驕ｶ荵怜款EE驕ｶ荳橸ｽ｣・ｹ郢晢ｽｻ鬮ｮ荳ｻ・｢鞫倡ｹ晢ｽｻE髯橸ｽ｢E驕ｶ髮・ｿｽ・､EE郢晢ｽｻE鬮ｯ・ｷ驛｢譎｢・ｽ・ｻ郢晢ｽｻE鬯ｮ・ｦE郢晢ｽｻ陞ｳ螟ｲ・ｽ・ｱ陟托ｽｱEEE遶擾ｽｫEE驛｢譎｢・ｽ・ｻ
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 鬮ｯ讖ｸ・ｽ・｢驕ｶ謫ｾ・ｽ・ｵ郢晢ｽｻE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴主・讓檸隴主承E髯橸ｽｳ陞滂ｽｲEE鬮｢・ｧE郢晢ｽｻE驛｢譎｢・ｽ・ｻ

					if (hitwall == true && hitwalls == false)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					else if (hitwalls == true)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}

	}



	mpModel->SetPosition(mvPosition);
	mpModel->SetRotation(mvRotation);

}

void Player3D::Damage(float damage)
{
	AnimationState now = mpModel->GetNowState();
	if (mnInvincibleTimer > 0) return; // 髴取ｻゑｽｽ・｡髫ｰ・ｨE髫ｴ蠑ｱ・玖将・｣髣包ｽｳE驍ｵ・ｺE驛｢譎会ｽｹ譎｢・ｽ・｡驛｢譎｢・ｽ・ｼ驛｢・ｧE髴取ｻゑｽｽ・｡髯ｷ莨夲ｽｽ・ｹ
	if (damage - mpEquipmentManager->GetDamage() <= 0) { mfHp -= 1; return; }//髫ｴ蟷・割陟托ｽｱ邵ｲ蝣､・ｹ・ｧ郢ｧ闌ｨ・ｽ・ｸ陝ｶ蜻ｻ・ｽ閾･・ｸ・ｺ郢晢ｽｻ
	if (now == ANIMATION_SLIDE || now == ANIMATION_ATTACKSLIDE)return;
	mfHp -= (damage-mpEquipmentManager->GetDamage());//鬮ｯ・ｬ郢晢ｽｻE驍ｵ・ｺ陷会ｽｱ遯ｶ・ｻ驍ｵ・ｺ郢晢ｽｻE驛｢譎会ｽｹ譎｢・ｽ・｡驛｢譎｢・ｽ・ｼ驛｢・ｧE髯具ｽｻ郢晢ｽｻ郢晢ｿｽ驛｢譎｢・ｽ・｡驛｢譎｢・ｽ・ｼ驛｢・ｧE驛｢・ｧ陷ｻ闌ｨ・ｽ・ｸ陝ｶ蜻ｻ・ｽ閾･・ｸ・ｺ郢晢ｽｻ

}

void Player3D::Evasion()
{
	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (mpModel->GetIsSeparate())mpModel->mpSeparateAnimation->SetAnimationCount(1.2f);
		else mpModel->mpAnimation->SetAnimationCount(1.2f);
		mpModel->ChangeAnimation(ANIMATION_SLIDE);
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		
		// 驛｢・ｧE驛｢譎｢・ｽ・ｩ驛｢・ｧE驛｢謌ｲ邵ｺ繝ｻ繝ｻ・ｳ驛｢・ｧE鬯ｮ・｢陷ｿ・･E陋ｹ・ｺ陷・ｽｾ驍ｵ・ｺE髴取ｻゑｽｽ・｡髫ｰ・ｨE髫ｴ蠑ｱ・玖将・｣驛｢・ｧ陋幢ｽｵ邵ｺ譎会ｽｹ謌ｲ郢晢ｽｨ (髯憺屮・ｽ・ｺ髫ｴ蟷｢・ｽ・ｬ30驛｢譎・ｽｼ驥・ｨ抵ｽｹ譎｢・ｽ・ｼ驛｢繝ｻ+ 鬮ｴ謇假ｽｽ・ｽ髯ｷ闃ｽ・ｾ貊ゑｽｽ・｡髫ｰ・ｨE驛｢譎・ｽｼ驥・ｨ抵ｽｹ譎｢・ｽ・ｼ驛｢繝ｻ
		mnInvincibleTimer = 30 + mnUpgradeEvasionInvincibility;
	}
	if (mpModel->GetNowState() == ANIMATION_SLIDE)
	{
		// 髯懃軸・ｨ・｣遶擾ｽｩ鬯ｨ・ｾ雋・ｽｷEE郢晢ｽｻ鬩帙・隴取ｧｫ・ｱ・ｬ郢晢ｽｻ陝ｲ・ｨ遶企豪・ｹ・ｧE驛｢謌ｲ郢晢ｽｻ驛｢・ｧE驛｢譎｢・ｽ・ｬ驛｢譎｢・ｽ・ｼ驛｢譎臥櫨郢晢ｽｻ驛｢・ｧ陞ｳ螟ｲ・ｽ・ｶE驍ｵ・ｺ郢晢ｽｻ
		mvPosition = VAdd(mvPosition, VScale(oldmoveVec, mfEvasionSpeed + mfUpgradeEvasionSpeed));
		mpModel->SetPosition(mvPosition);
	}






}


void Player3D::RotationByMove()
{

	//////鬮｣遒大ｴ溯楜・ｦ鬯ｯ・ｮ郢晢ｽｻEE驛｢譎｢・ｽ・ｻ驕ｯ・ｶE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ郢晢ｽｻ隶難ｽ｣陜鈴ｦｴ蟇櫁ｮ厄ｽｸEE郢晢ｽｻE鬩幢ｽ｢E髯懶ｽ｣E鬮｢・ｧ郢晢ｽｻ隲､谿ｺE
	//Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag_3D_Camera)
	//鬮ｴ謇假ｽｽ・ｴ郢晢ｽｻE鬮ｯ諛ｶ・ｽ・ｨ郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ諛・ｻｸE郢晢ｽｻEE郢晢ｽｻE鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩搾ｽｵE髣包ｽｵ隴趣ｽ｢E髯具ｽｾE郢晢ｽｻE鬮ｫ・ｶ霓｣蛟｡蜃ｽ驛｢譎｢・ｽ・ｻ鬮ｯ諛・ｻｸE郢晢ｽｻEE郢晢ｽｻE鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩搾ｽｵE郢晢ｽｻE鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻE鬮ｯ蜈ｷ・ｽ・ｻ驛｢譎｢・ｽ・ｻ郢晢ｽｻ陞ｳ螟ｲ・ｽ・ｱ陟托ｽｱEEE遶擾ｽｫEE驛｢譎｢・ｽ・ｻ
	float subAngle = mfTargetAngle - mfAngle;



	//鬩搾ｽｵE驛｢・ｧ郢晢ｽｻE驍・私・ｽ・ｭ郢晢ｽｻEE鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣E繝ｻ・ｰ鬩幢ｽ｢E髯晢ｽｲE驕ｶ證ｮﾎ忍髯具ｽｹE髯敖E鬮ｯ・ｷE髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻE鬩搾ｽｵE鬯ｲ繝ｻ繝ｻ・ｽE髫ｰ繝ｻEE髯句ｦ｣EE髣皮甥驕廢E郢晢ｽｻE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽE鬮｣蛹・ｽｽ・ｳ髯槭・EE髣費ｿｽ陋ｹ繝ｻE郢晢ｽｻE鬮｣蛹・ｽｽ・ｳ髯ｷ闌ｨ・ｽ・ｷ郢晢ｽｻE髯晢ｽｲE驕ｶ莨∬ｱｪEE郢晢ｽｻE鬩幢ｽ｢E髣包ｽｵ隴趣ｽ｢E郢晢ｽｻEE郢晢ｽｻE鬩搾ｽｵE髯溷供・ｨ・ｯ郢晢ｽｻ鬩搾ｽｵE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵE髯橸ｽ｢E驕ｶ逡窪E郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE
	//鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩搾ｽｵE鬯ｲ繝ｻ繝ｻ・ｽE髫ｰ繝ｻEE髯句ｦ｣EE鬮ｯ諛会ｽｼ螟ｲ・ｽ・ｽE郢晢ｽｻE鬮｣蛹・ｽｽ・ｳ髯槭・EE髣費ｿｽ陋ｹ繝ｻE郢晢ｽｻE鬮｣蛹・ｽｽ・ｳ髯ｷ闌ｨ・ｽ・ｷ郢晢ｽｻE髯晢ｽｲE驕ｶ莨∬ｱｪEE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ髫ｨ・ｳ郢晢ｽｻEE鬮｢・ｾE髮趣ｽｬE鬮ｮ谿ｺEE鬩搾ｽｵE髯ｷ・ｷE郢晢ｽｻ郢晢ｽｻ
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//鬯ｮ・ｫ髣埼屮・ｽ・ｲ隶厄ｽｸEE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻE鬮ｯ蜈ｷ・ｽ・ｻ驛｢譎｢・ｽ・ｻ郢晢ｽｻ陞ｳ螢ｽ繝ｻEE驛｢譎｢・ｽ・ｻ驕ｶ鬘倅ｾｭ郢晢ｽｻ鬮｣髮・ｽｨ・ｯ郢晢ｽｻ鬯ｮ・ｴ陷ｿ・ｰEEE鬩搾ｽｵE髣比ｼ夲ｽｽ・｣郢晢ｽｻ郢晢ｽｻ
	if (subAngle > 0.0f)
	{
		subAngle -= ROTATE_SPEED;
		if (subAngle < 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	else if (subAngle < 0.0f)
	{
		subAngle += ROTATE_SPEED;
		if (subAngle > 0.0f)
		{
			subAngle = 0.0f;
		}
	}


	//鬮｣遒大ｴ溯楜・ｦ鬯ｯ・ｮ郢晢ｽｻEE驛｢譎｢・ｽ・ｻ驕ｯ・ｶE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ郢晢ｽｻ隶難ｽ｣陜鈴ｦｴ蟇櫁ｮ厄ｽｸEE郢晢ｽｻE鬩幢ｽ｢E髯懶ｽ｣E鬮｢・ｧ郢晢ｽｻ隲､谿ｺE
	mfAngle = mfTargetAngle - subAngle;


	//鬮ｯ諛・ｻｸE郢晢ｽｻEE郢晢ｽｻE鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩幢ｽ｢E髯橸ｽｳ陞滂ｽｲEE郢晢ｽｻE鬮ｯ讖ｸ・ｽ・ｳ驛｢譎｢・ｽ・ｻ
	mvRotation.y = mfAngle + DX_PI_F;
	//鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻE隴主承E郢晢ｽｻE鬮｣雋ｻ・ｽ・ｨ髫ｴ謫ｾ・ｽ・ｴ驕ｶ謫ｾ・ｽ・ｴ鬩幢ｽ｢E驛｢譎｢・ｽ・ｻ
	mpModel->SetRotation(mvRotation);

	////鬮ｯ諛・ｻｸE郢晢ｽｻEE郢晢ｽｻE鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩幢ｽ｢E髯橸ｽｳ陞滂ｽｲEE郢晢ｽｻE鬮ｯ讖ｸ・ｽ・ｳ驛｢譎｢・ｽ・ｻ
	////鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻE隴主承E郢晢ｽｻE鬮｣雋ｻ・ｽ・ｨ髫ｴ謫ｾ・ｽ・ｴ驕ｶ謫ｾ・ｽ・ｴ鬩幢ｽ｢E驛｢譎｢・ｽ・ｻ


}




void Player3D::Jump()
{

	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		mvPosition.y += 300.0f;
		mbjump = true;
		mfjumpPower = JUMP_POWER;

		//鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴取ｨ雁袖髯晢ｽｷ隰碁�托ｽｲ繧雁ｵｯ隴擾ｽｴE鬩幢ｽ｢隴乗・・ｽ・ｹ隴∵ｻ・ｱｪE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE髯ｷ・ｷE郢晢ｽｻ郢晢ｽｻ
		
	}

	

}

void Player3D::Result()
{


}

void Player3D::Attack()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // 鬩幢ｽ｢隴弱・E驍ｵ・ｺ髢ｧ・ｲEE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｴ謇假ｽｽ・･郢晢ｽｻE鬮ｫ・ｲE髣包ｽｵ隴趣ｽ｢E陞ｳ螢ｽ笊るｬｮ・｢郢晢ｽｻE驛｢譎｢・ｽ・ｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackCount >= AttackTime&&now!=ANIMATION_ATTACK)
	{
		AttackCount = 0;
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACK);
		//鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻE騾ｧ・ｮE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｯ讓奇ｽｺ・ｽ陋ｻ・､髯晢ｽｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驛｢譎｢・ｽ・ｻ鬩搾ｽｵE鬮ｴ蝓溷繭鬮ｮ・ｷ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ
		mpModel->SetLoop(false);
		//鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬮ｯ貅ｷ萓妣陟慕ｪｪ鬮ｯ貅ｯ・ｼ蠢咲ｹ晢ｽｻEE騾ｧ・ｮE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｫ・ｰ鬲・ｼ夲ｽｽ・ｽE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		if (mpModel->GetIsSeparate() == true)
		{
			mpModel->mpSeparateAnimation->SetAnimationCount(1.3f);
		}
		else
		{
			mpModel->mpAnimation->SetAnimationCount(0.5f);
		}
	}


	auto pObjList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	if (now == ANIMATION_ATTACK)
	{
		AttackState = Attack_Normal;
		
		if (AttackCount % 5 == 0)
		{
			for (int i = 0; i < pObjList.size(); i++)
			{
				Enemy* pEne = dynamic_cast<Enemy*>(pObjList.at(i));
				if (pEne == nullptr) continue;
				pEne->SetHitjudgment(false);
			}
		}
	}



}

void Player3D::AttackJump()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // 鬩幢ｽ｢隴弱・E驍ｵ・ｺ髢ｧ・ｲEE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｴ謇假ｽｽ・･郢晢ｽｻE鬮ｫ・ｲE髣包ｽｵ隴趣ｽ｢E陞ｳ螢ｽ笊るｬｮ・｢郢晢ｽｻE驛｢譎｢・ｽ・ｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackJumpCount >= AttackJumpTime&&!mbjump)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_JUMP);
		mbjump = true;
		AttackJumpCount = 0;
		mfjumpPower = JUMP_POWER;
		//鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻE騾ｧ・ｮE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｯ讓奇ｽｺ・ｽ陋ｻ・､髯晢ｽｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACKJUMP);
		//鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驛｢譎｢・ｽ・ｻ鬩搾ｽｵE鬮ｴ蝓溷繭鬮ｮ・ｷ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		//鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬮ｯ貅ｷ萓妣陟慕ｪｪ鬮ｯ貅ｯ・ｼ蠢咲ｹ晢ｽｻEE騾ｧ・ｮE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｫ・ｰ鬲・ｼ夲ｽｽ・ｽE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ
		
		if (mpModel->GetIsSeparate() == true)
		{
			mpModel->mpSeparateAnimation->SetAnimationCount(1.0f);
		}
		else
		{
			mpModel->mpAnimation->SetAnimationCount(1.0f);
		}
	}


	auto pObjList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	if (now == ANIMATION_ATTACKJUMP)
	{
		AttackState = Attack_Jump;
		if (mfjumpPower >= mvPosition.y && !HighPositionFlag)
		{
			mvPosition = VAdd(mvPosition, VGet(0.0f, 5.0f, 0.0f));
		}
		if(mfjumpPower <= mvPosition.y)
		{
			HighPositionFlag = true;
			
			mbjumpDown = true;

		}
		if (HighPositionFlag)
		{
			mvPosition = VAdd(mvPosition, VGet(0.0f, mfjumpPower, 0.0f));
			mfjumpPower -= 1.0f;
		}
		


		if (mvPosition.y <= hitPos.y)
		{
			OnJumpCollider = true;
			mvPosition.y = hitPos.y;//Y鬮ｯ貅ｯ・ｶ・｣EE鬮ｫ・ｶ霓｣蛟｡蜃ｽ郢晢ｽｻ郢晢ｽｻ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶE鬩搾ｽｵE鬩怜遜・ｽ・ｫ郢晢ｽｻE
			
		}
		

	}
	else
	{
		OnJumpCollider = false;
		HighPositionFlag = false;
		mbjump = false;
		mbjumpDown = false;
	}
}

void Player3D::AttackSlide()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // 鬩幢ｽ｢隴弱・E驍ｵ・ｺ髢ｧ・ｲEE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬮ｴ謇假ｽｽ・･郢晢ｽｻE鬮ｫ・ｲE髣包ｽｵ隴趣ｽ｢E陞ｳ螢ｽ笊るｬｮ・｢郢晢ｽｻE驛｢譎｢・ｽ・ｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackSlideCount >= AttackSlideTime)
	{
		if (mpTarget == nullptr)return;//鬮ｫ・ｰE郢晢ｽｻE鬩幢ｽ｢E髯ｷ・ｻ鬩包ｽｺ隶鯉ｽｳ鬯ｩ蠅灘ｾ勇E鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE鬯ｮ・ｦE驕ｶ逡窪E郢晢ｽｻE鬩搾ｽｵEEE髫ｧ・ｽetrurn
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		if (mpModel->GetIsSeparate())mpModel->mpSeparateAnimation->SetAnimationCount(1.2f);
		else mpModel->mpAnimation->SetAnimationCount(1.2f);
		AttackSlideCount = 0;
		GoPosition = (VSub(mpTarget->GetPosition(), mvPosition));
		TargetPosition = VScale(GoPosition, 2.5f / 30.0f);

		//鬮ｫ・ｰE郢晢ｽｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻE騾ｧ・ｮE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｯ讓奇ｽｺ・ｽ陋ｻ・､髯晢ｽｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACKSLIDE);
		
		//鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驛｢譎｢・ｽ・ｻ鬩搾ｽｵE鬮ｴ蝓溷繭鬮ｮ・ｷ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ
		mpModel->SetLoop(false);
		//鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬮ｯ貅ｷ萓妣陟慕ｪｪ鬮ｯ貅ｯ・ｼ蠢咲ｹ晢ｽｻEE騾ｧ・ｮE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE鬩搾ｽｵE郢晢ｽｻE鬮ｫ・ｰ鬲・ｼ夲ｽｽ・ｽE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);

	}
	

	
	if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide)
	{
		new Effect(VAdd(mvPosition, VGet(0.0f, 0.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 255, 30, 20), 50.0f, 0.03f);
		GoPosition = VNorm(GoPosition);
		mfTargetAngle = atan2f(GoPosition.x, GoPosition.z);


		////////////////鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴守甥諢幃刹・ｻE鬮ｫ・ｰE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬮ｯ貅ｯ・ｶ・｣EE鬮ｫ・ｶ霓｣蛟ｩ・｡・ｷ郢晢ｽｻE郢晢ｽｻE鬮ｯ・ｷ鬮ｦ・ｪ郢晢ｽｻ///////////////
		if (AttackSlideCount < 30)
		{
			mvPosition = VAdd(mvPosition, TargetPosition);
			Master::mpCamera->AddHorizontalAngle(90.0f / 30.0f);
		}
		mpModel->SetPosition(mvPosition);
	}

	



}




void Player3D::Shot()
{


}
void Player3D::bar()
{
	float HpRatio = mfHp / GetAllStatusState(Object3D::Status_Hp);


	mfHp = mfHp < 0 ? 0 : mfHp; //HP鬩搾ｽｵE驛｢譎｢・ｽ・ｻ鬮ｫ・ｴ陝ｷ・｢EE鬮ｮ逡ｿ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵEEE郢晢ｽｻ鬩幢ｽ｢E鬮ｮ蛹ｺ・ｧ・ｭ郢晢ｽｻ鬩幢ｽ｢E髯滓坩・ｯ莨夲ｽｽ・ｽ郢晢ｽｻ
	mfHp = mfHp > GetAllStatusState(Object3D::Status_Hp) ? GetAllStatusState(Object3D::Status_Hp) : mfHp; //HP鬩搾ｽｵE髫ｴ・ｴE髫ｲ・､陷ｻ・ｵ隴ｽ蠢孔鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩幢ｽ｢E髯橸ｽｳ陞滂ｽｲEE驛｢譎｢・ｽ・ｻ驕ｶ謫ｾ・ｽ・ｴ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ髫ｨ・ｳ郢晢ｽｻEE髯晢ｽｲEE驕ｶ謫ｾ・ｽ・ｵ髫ｲ・､陷ｻ・ｵ隴ｽ蠢孔鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩幢ｽ｢E鬮ｮ蛹ｺ・ｧ・ｭ郢晢ｽｻ鬩幢ｽ｢E髯滓坩・ｯ莨夲ｽｽ・ｽ郢晢ｽｻ
	unsigned int Color;
	unsigned int Color2;
	Color = GetColor(255, 255, 255);
	Color2 = GetColor(0, 0, 0);
	DrawBox(pos.x, pos.y, pos.x + size.x, pos.y + size.y, Color2, true);
	DrawBox(pos.x, pos.y, pos.x + size.x, pos.y + size.y, GetColor(255, 255, 255), true);
	DrawBox(pos.x + 1, pos.y + 1,
		pos.x + size.x * HpRatio - 1,
		pos.y + size.y - 1,
		GetColor(0, 255, 0), true);

	float DashRatio = DashGuage / MaxDashGauge;

	DashGuage = DashGuage < 0 ? 0 : DashGuage; //HP鬩搾ｽｵE驛｢譎｢・ｽ・ｻ鬮ｫ・ｴ陝ｷ・｢EE鬮ｮ逡ｿ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵEEE郢晢ｽｻ鬩幢ｽ｢E鬮ｮ蛹ｺ・ｧ・ｭ郢晢ｽｻ鬩幢ｽ｢E髯滓坩・ｯ莨夲ｽｽ・ｽ郢晢ｽｻ
	DashGuage = DashGuage > MaxDashGauge ? MaxDashGauge : DashGuage; //HP鬩搾ｽｵE髫ｴ・ｴE髫ｲ・､陷ｻ・ｵ隴ｽ蠢孔鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩幢ｽ｢E髯橸ｽｳ陞滂ｽｲEE驛｢譎｢・ｽ・ｻ驕ｶ謫ｾ・ｽ・ｴ鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE驛｢譎｢・ｽ・ｻ髫ｨ・ｳ郢晢ｽｻEE髯晢ｽｲEE驕ｶ謫ｾ・ｽ・ｵ髫ｲ・､陷ｻ・ｵ隴ｽ蠢孔鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽE鬩幢ｽ｢E鬮ｮ蛹ｺ・ｧ・ｭ郢晢ｽｻ鬩幢ｽ｢E髯滓坩・ｯ莨夲ｽｽ・ｽ郢晢ｽｻ
	unsigned int Color3;
	unsigned int Color4;
	Color3 = GetColor(255, 255, 255);
	Color4 = GetColor(0, 0, 0);
	DrawBox(pos2.x, pos2.y, pos2.x + size2.x, pos2.y + size2.y, Color3, true);
	DrawBox(pos2.x + 1, pos2.y + 1,
		pos2.x + size2.x * DashRatio - 1,
		pos2.y + size2.y - 1,
		GetColor(255, 255, 0), true);

	// Slide Attack Cooldown UI
	float SlideRatio = (float)AttackSlideCount / AttackSlideTime;
	SlideRatio = SlideRatio > 1.0f ? 1.0f : SlideRatio;
	VECTOR uiSlidePos = { 30, 820 };
	VECTOR uiSlideSize = { 140, 20 };
	DrawBox(uiSlidePos.x, uiSlidePos.y, uiSlidePos.x + uiSlideSize.x, uiSlidePos.y + uiSlideSize.y, GetColor(0, 0, 0), true);
	DrawBox(uiSlidePos.x + 1, uiSlidePos.y + 1, uiSlidePos.x + uiSlideSize.x * SlideRatio - 1, uiSlidePos.y + uiSlideSize.y - 1, GetColor(0, 255, 255), true);
	DrawFormatString(uiSlidePos.x + 5, uiSlidePos.y + 2, GetColor(255, 255, 255), "SLIDE");

	// Jump Attack Cooldown UI
	float JumpRatio = (float)AttackJumpCount / AttackJumpTime;
	JumpRatio = JumpRatio > 1.0f ? 1.0f : JumpRatio;
	VECTOR uiJumpPos = { 180, 820 };
	VECTOR uiJumpSize = { 150, 20 };
	DrawBox(uiJumpPos.x, uiJumpPos.y, uiJumpPos.x + uiJumpSize.x, uiJumpPos.y + uiJumpSize.y, GetColor(0, 0, 0), true);
	DrawBox(uiJumpPos.x + 1, uiJumpPos.y + 1, uiJumpPos.x + uiJumpSize.x * JumpRatio - 1, uiJumpPos.y + uiJumpSize.y - 1, GetColor(255, 0, 255), true);
	DrawFormatString(uiJumpPos.x + 5, uiJumpPos.y + 2, GetColor(255, 255, 255), "JUMP");

	mpLevelUp->Draw();
	mpShortInventory->Draw();
}

void Player3D::ChangeView()
{

	ChangeCamera++;
	if (ChangeCamera == 0)
	{
		Master::mpCamera->SetCamera1(true);
		Master::mpCamera->SetCamera3(false);
	}
	if (ChangeCamera == 1)
	{
		Master::mpCamera->SetCamera1(false);
		Master::mpCamera->SetCamera3(true);
		ChangeCamera = -1;
	}

}



void Player3D::EnemySerch()
{

}

void Player3D::Test()
{
	if (InputManager::CheckDownKey(KEY_INPUT_2))
	{
		// 鬮｣蜴・ｽｽ・ｴ鬮ｴ驛・ｽｲ・ｻE陞ｳ螟ｲ・ｽ・ｫE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢E驛｢・ｧ郢晢ｽｻEE驛｢譎｢・ｽ・ｻ髫ｨ讚馴椨謇具ｽｮ謚ｵ・ｽ・ｫ郢晢ｽｻE郢晢ｽｻE鬯ｮ・ｦE郢晢ｽｻE ---------------- //
		Item::ItemInformation* itemInfo;
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::HEAL;
		itemInfo->Name = "蝗槫ｾｩ阮ｬ";
		Master::mpItemManager->AddItem(itemInfo);

		// --------------------------------------- //

		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 1;
		itemInfo->ID = Item::ItemID::HIGHHEAL;
		itemInfo->Name = "highheal";
		Master::mpItemManager->AddItem(itemInfo);


		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::POWER;
		itemInfo->Name = "Power";
		Master::mpItemManager->AddItem(itemInfo);

		// 鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻE繝ｻ・ｦ
		// 鬮ｫ・ｲE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵEEE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻE陜｣・､EE髣包ｽｵ隴趣ｽ｢E髴托ｼｵemInformation鬩幢ｽ｢E鬮ｮ蛹ｺ・ｧ・ｫ陟募ｮ｣霎ｧ陷肴ｺｷ・ｹ・ｲ髫ｨ讚摘E驛｢譎｢・ｽ・ｻ
		// 鬩搾ｽｵE髫ｴ謫ｾ・ｽ・ｴ郢晢ｽｻ隶呵ｶ｣・ｽ・ｹE髫ｴ・ｽ陜ｸ・ｾemManager鬩搾ｽｵE郢晢ｽｻE鬮ｮ荵晢ｽ薦E鬩搾ｽｵE髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶE鬩搾ｽｵE驛｢・ｧ郢晢ｽｻEE鬩幢ｽ｢E驛｢譎｢・ｽ・ｻ

		Weapon::WeaponDate* weaponInf;
		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 5;
		weaponInf->filename = "Resource/3D/Sabel.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon3;
		weaponInf->name = "weapon3";
		mpWeaponManager->AddWeapon(weaponInf);


		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 7;
		weaponInf->filename = "Resource/3D/Hero.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon2;
		weaponInf->name = "weapon2";
		mpWeaponManager->AddWeapon(weaponInf);

	}
	if (InputManager::CheckDownKey(KEY_INPUT_3))
	{
		// 鬮｣蜴・ｽｽ・ｴ鬮ｴ驛・ｽｲ・ｻE陞ｳ螟ｲ・ｽ・ｫE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩搾ｽｵE郢晢ｽｻE鬩幢ｽ｢E驛｢・ｧ郢晢ｽｻEE驛｢譎｢・ｽ・ｻ髫ｨ讚馴椨謇具ｽｮ謚ｵ・ｽ・ｫ郢晢ｽｻE郢晢ｽｻE鬯ｮ・ｦE郢晢ｽｻE ---------------- //
		Item::ItemInformation* itemInfo;
		
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 6;
		itemInfo->ID = Item::ItemID::SPEED;
		itemInfo->Name = "speed";
		Master::mpItemManager->AddItem(itemInfo);
		mpHaveMoney->AddMoney(1000);
		mpLevelUp->AddXp(20);

	}
	if (InputManager::CheckDownKey(KEY_INPUT_4))
	{
		mfHp -= 30;
	}


}
void Player3D::OnEnter(Collider* collider, Collider* check)
{
	

	if (collider == mpSerchEnemyCollider && check->mpParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
	{
		

		auto pEne = dynamic_cast<Enemy*>(check->mpParentObject);
		if (pEne == nullptr) return;
		VECTOR mnEneDistance = VSub(pEne->GetPosition(), mvPosition);
		if (check == pEne->GetEnemycoll())
		{
			mbAttackSlideSerch = true;
			float VECSize = VSize(mnEneDistance);
			FirstNearVec++;
			if (FirstNearVec == 1)
			{
				VECSizeFirst = VECSize;
			}

			if (VECSizeFirst <= VECSize)
			{
				VECSizeFirst = VECSize;
				mpTarget = pEne;

			}
		}
	}
	// 
	// 
	// 
	// 
	// && check->mpParentObject->GetTag() == Object3D::Tag3D_Player3D)


	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Obj)
	{
		
	}

}

void Player3D::OnTrigger(Collider* collider, Collider* check)
{

	AnimationState now = mpModel->GetNowState();

	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpAttachCollider && check->mpParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
		{
			Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
			if (pEne == nullptr) return;
			if (check == pEne->GetEnemycoll())
			{
				
				// 鬮ｫEE郢晢ｽｻE鬮ｫEE驛｢譎｢EEE騾ｧEE隴趣ｽ｢EE鬩幢ｽ｢E郢晢ｽｻE鬩幢ｽ｢隴趣ｽ｢EE鬩幢ｽ｢隴趣ｽ｢EE&&Attack1
				if (now == ANIMATION_ATTACK && AttackState == Attack_Normal && !mbjump && !pEne->GetHitjudgment())
				{
					
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));
					// 鬯ｨE陋ｹ繝ｻE鬯ｯEE郢晢ｽｻE鬮ｫEE郢晢ｽｻE鬩幢ｽ｢E髯滓坩E莨夲ｽｽE郢晢ｽｻEE髯晢ｽｶ陷ｻEE郢晢ｽｻ
					Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
					new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 0, 30, 0), 30.0f, 0.1f);
				}


			}

		}
	}
	if (collider == mpAttackJumpCollider && check->mpParentObject->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
		if (pEne == nullptr) return;
		if (check == pEne->GetEnemycoll())
		{
			if (now == ANIMATION_ATTACKJUMP && AttackState == Attack_Jump && !pEne->GetHitjudgment())
			{
				pEne->SetHitjudgment(true);
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + mfAttackjump);

				// 鬯ｨ・ｾ陋ｹ繝ｻE鬯ｯ・ｮE郢晢ｽｻE鬮ｫ・ｰE郢晢ｽｻE鬩幢ｽ｢E髯滓坩・ｯ莨夲ｽｽ・ｼ郢晢ｽｻEE髯晢ｽｶ陷ｻ・ｻE郢晢ｽｻ
				Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
				new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(0, 255, 30, 0), 30.0f, 0.1f);
			}
		}
	}

	if (collider == mpAttackSlideCollider && check->mpParentObject->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
		if (pEne == nullptr) return;
		if (check == pEne->GetEnemycoll())
		{
			if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide && !pEne->GetHitjudgment())
			{
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + mfAttackSlide);
				pEne->SetHitjudgment(true);
				// 鬯ｨ・ｾ陋ｹ繝ｻE鬯ｯ・ｮE郢晢ｽｻE鬮ｫ・ｰE郢晢ｽｻE鬩幢ｽ｢E髯滓坩・ｯ莨夲ｽｽ・ｼ郢晢ｽｻEE髯晢ｽｶ陷ｻ・ｻE郢晢ｽｻ
				Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
				new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(35, 0, 255, 0), 60.0f, 1.0f);
			}
		}
	}
	


}

void Player3D::OnExit(Collider* collider, Collider* check)
{
	/*if (collider==mpAttachCollider&&check->mpParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
	{
		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);

	}*/
}

void Player3D::SelectAttack()
{
	AttackCountClass();
	AnimationState now = mpModel->GetNowState();
	if (InputManager::CheckDownKey(KEY_INPUT_E) && now != ANIMATION_ATTACK)
	{
		ChangeStateCount++;
		if (ChangeStateCount > 2)
		{
			ChangeStateCount = 0;
		}

	}

	switch (ChangeStateCount)
	{
	case 0:

		AttackState = Attack_Normal;
		Attack();
		break;
	case 1:

		AttackState = Attack_Slide;
		AttackSlide();
		break;
	case 2:
		AttackState = Attack_Jump;
		AttackJump();
	default:
		break;
	}
	if (Master::mpDebug)
	{
		DrawFormatString(300, 300, GetColor(255, 255, 255), "%d", ChangeStateCount);
	}
	if (!(now==ANIMATION_ATTACK)&& !(now == ANIMATION_ATTACKJUMP)&& !(now == ANIMATION_ATTACKSLIDE))//鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E驛｢譎｢・ｽ・ｻ鬮ｯ貅ｷ譯・E鬮ｮ雜｣・ｽ・｢郢晢ｽｻE
	{
		auto mpEne = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < mpEne.size(); i++)
		{
			Enemy* pEne = dynamic_cast<Enemy*>(mpEne.at(i));
			if (pEne == nullptr) continue;
			pEne->SetHitjudgment(false);
		}
	}

}

void Player3D::AttackCountClass()
{


	AttackCount++;
	AttackSlideCount++;
	AttackJumpCount++;


}
void Player3D::CollPositionUpdate()
{
	AnimationState now = mpModel->GetNowState();
	// 鬮ｯ讙ｸ關難ｽｭ髫ｨ・ｳ郢晢ｽｻEE鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢E髯晢ｽｲE鬮ｫ・ｴ郢晢ｽｻEE
	mpCapsuleCollider->mvPosition = mvPosition;
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));

	mpAttachCollider->mvPosition = VGet(1000, 10000, 1000);
	mpSerchEnemyCollider->mvPosition = mvPosition;
	mpAttackSlideCollider->mvPosition = VGet(1000, 10000, 1000);
	mpAttackJumpCollider->mvPosition = VGet(1000, 10000, 1000);
	if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide)
	{
		mpAttackSlideCollider->mvPosition = mvPosition;
	}
	else if (now == ANIMATION_ATTACKJUMP && AttackState == Attack_Jump&& OnJumpCollider)
	{
		mpAttackJumpCollider->mvPosition = mvPosition;
	}
	else if (now == ANIMATION_ATTACK)
	{
		mpAttachCollider->mvPosition = mpModel->GetAttachmentPosition();
	}

}

float Player3D::GetAllStatusState(Object3D::StatusState state)
{
	if (mpLevelUp == nullptr || mpBuffManager == nullptr || mpWeaponManager == nullptr)return 0;

	if (state == Status_Attack)
	{
		return mpWeaponManager->GetDamage() + mpBuffManager->GetBuff(state)+mpLevelUp->GetAttack() + mfUpgradeAttack;
	}
	if (state == Status_Speed)
	{
		return mfSpeed + mpBuffManager->GetBuff(state)+mpLevelUp->GetSpeed() + mfUpgradeSpeed;
	}
	if (state == Status_Hp)
	{
		return mfMaxHp + mpLevelUp->GetHp() + mfUpgradeMaxHp;
	}


}

void Player3D::CollDelete()
{
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->SetDeleteFlag(true);
	}
	if (mpAttackJumpCollider != nullptr)
	{
		mpAttackJumpCollider->SetDeleteFlag(true);
	}
	if (mpAttackSlideCollider != nullptr)
	{
		mpAttackSlideCollider->SetDeleteFlag(true);
	}
	if ( mpSerchEnemyCollider!= nullptr)
	{
		mpSerchEnemyCollider->SetDeleteFlag(true);
	}
}
