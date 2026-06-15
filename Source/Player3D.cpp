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

// 髯具ｽｻ郢晢ｽｻ霑夲ｽ｡驛｢E繝ｻE驛｢譏懶ｽｹ譁滄豪E譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驛｢E陷薦EE繝ｻE鬨ｾ蛹・E驍ｵE陷ｷE繝ｻ迢暦ｽｸE闕ｵ譏ｴ繝ｻ鬮ｫE繝ｻE髯橸ｽｳ陞｢E繝ｻ蟶晄≧繝ｻE髯ｷ莨夲ｽｿE郢晢ｽｻ陋ｹE郢晢ｽｧ驛｢譎・譁青ｰ驛｢譎｢EE驛｢譎冗樟郢晢ｽｻfalse郢晢ｽｻ闔�蛹・E繝ｻE鬨ｾ蛹・E驍ｵE陷会ｽｱ遶界EE郢晢ｽｻ繝ｻE闔ｨ螟ｲEE郢晢ｽｻ
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool isSeparateAnim)
	:Object3D(initPos)
	, mfAttack(0)//驛｢謨夜Δ譎｢EE驛｢譎｢EE驛｢E繝ｻE
	, mfAttackjump(5)
	, mfAttackSlide(7)

	,OnJumpCollider(false)//驍ｵE髦ｮ蜷ｶ繝ｻ髴托ｽｸ繝ｻE鬯ｮE髦ｮ蜷ｮ繝ｻ髯檸萓ｭ隨ｳ繝ｻEE鬮ｮ竏ｵ繝ｻ髯橸ｽｳ陞｢E繝ｻ蝣､EE繝ｻE驍ｵE闔会ｽ｣繝ｻ繝ｻ
	, mbInvisible(false)//髫ｰE繝ｻE髫ｰE郢晢ｽｻ遯ｶE鬯ｩE陝�雜｣EE郢晢ｽｻ繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ繝ｻ閧ｲEE郢晢ｽｻ遶界
	, mbjump(false)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(speed)//speed
	, JUMP_POWER(jumppower)
	, mfHp(hp)//hp
	, mfMaxHp(hp)//maxhp
	, mfDashSpeed(speed * 2)
	, NewShieldFast(false)//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驛｢譎擾ｽｳE繝ｻ蟶晏｢難ｽｻ阮僞驍ｵE陷ｷE繝ｻ迢暦ｽｸE闕ｵ譏ｶ繝ｻ驍ｵE郢晢ｽｻE・ｰ
	, mfSize(60.0f)//驍ｵE郢ｧ繝ｻ陞ｺ驛｢E鬮ｮ竏ｵ繝ｻ髯橸ｽｳ郢晢ｽｻ
	, Pause(false)//驛｢譎・郢晢ｽｻ驛｢E繝ｻE鬨ｾ蛹・E鬯ｮE繝ｻE
	, ChangeCamera(0)//鬮ｫ遨ゑｽｹ貅倪雷髯樊ｺｽ蛻､陝ｲE
	, DashGuage(100.0f)//
	, EneSerchCount(0)
	, Gpush(false)//髯滂ｽ｢郢晢ｽｻ繝ｻE繝ｻE髫ｰ蝣矩ΔE陋幢ｽｵ繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ繝ｻ讓抵ｽｸE郢晢ｽｻ
	, GpushCount(2)//髯滂ｽ｢郢晢ｽｻ繝ｻE繝ｻE髫ｰ蝣矩しE繝ｻE驛｢E繝ｻE驛｢E繝ｻE驛｢譎｢EE驛｢譏ｴ繝ｻ
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
	mpModel->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Slash.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKSLIDE, "Resource/Model/Attack.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/Model/Jump Attack.mv1");
	mpModel->AddAnimation(ANIMATION_SLIDE, "Resource/Model/kaihi.mv1");

	Master::mpCamera->Initialize();

	float HpRatio = (float)mfHp / mfMaxHp;
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;
	{//髯具ｽｻ隴殺EEE郢晢ｽｻ繝ｻ繝ｻ
		Weapon::WeaponDate* weaponInf;
		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 1000;
		weaponInf->filename = "Resource/3D/Sabel.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon1;
		weaponInf->name = "weapon1";
		mpWeaponManager->AddWeapon(weaponInf);
		mpWeaponManager->ChangeWeapon(weaponInf);

		mfNormalAttack = weaponInf->damage;//髫ｴ蟠｢髯具ｽｻ隴擾ｽｴ遶頑･｢蟠戊ｭ殺EEE郢晢ｽｻ繝ｻ蜥擾ｽｸE繝ｻE髫ｰE繝ｻE髫ｰE郢晢ｽｻ鬲假ｽｨ驍ｵE繝ｻE髯具ｽｻ隴殺EEE鬮｢郢晢ｽｻ鬨ｾ繝ｻ繝ｻ繝ｻ蟶晏距陟募ｨｯ魘ｬ
		mfAttack = weaponInf->damage;//髫ｴ蟠｢髯具ｽｻ隴擾ｽｴ遶頑･｢蟠戊ｭ殺EEE郢晢ｽｻ繝ｻ蜥擾ｽｸE繝ｻE髫ｰE繝ｻE髫ｰE郢晢ｽｻ鬲假ｽｨ驍ｵE繝ｻE髯具ｽｻ隴殺EEE鬮｢郢晢ｽｻ鬨ｾ繝ｻ繝ｻ繝ｻ蟶晏距陟募ｨｯ魘ｬ
	}




	MaxDashGauge = DashGuage;

	
	 // 驛｢E繝ｻE驛｢譎｢EE驛｢E繝ｻE驛｢謨夜Δ譎｢EE鬨ｾ蠅難ｽｻ阮僞
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
		
		Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//髯ｷ莨夲ｽｽE髫ｴE隲橸ｽｺ雎ｬE
	}
		
	
	if (Master::PauseOn == false)//驛｢譎・郢晢ｽｻ驛｢E繝ｻE髣包ｽｳ繝ｻE驍ｵE繝ｻE髯ｷ蟠趣ｽｼ譚ｿE驍ｵE繝ｻE驍ｵE郢晢ｽｻ繝ｻ閧ｲEE郢晢ｽｻ遶企豪EE陷ｷE繝ｻ繝ｻ
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
		////////////////////////鬮ｯE隰疲ｺｯE////////////////
		SelectAttack();
		Result();
		ResetNUETRAL();
		//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驛｢譎・諛界鬨ｾ繝ｻ繝ｻ
		CollPositionUpdate();
		Evasion();
		//髫ｰE繝ｻE髫ｰE郢晢ｽｻ郢晢ｽｻ鬨ｾ繝ｻ繝ｻ
		SelectAttack();
		//鬩募∞EE髯ｷ讎贋ｾ帷ｹ晢ｽｻ鬨ｾ繝ｻ繝ｻ
		MoveEx();
		//髯懃軸E繝ｻEE繝ｻE髯ｷ繝ｻEE鬨ｾ繝ｻ繝ｻ
		RotationByMove();
		EnemySerch();

		if (mpLevelUp->GetLevelUp())
		{ 
			Master::mpSoundManager->PlaySE(SoundManager::SE_LEVELUP);
			mfHp = GetAllStatusState(Object3D::Status_Hp);//hp驛｢E髮区ｧｭ繝ｻ髯懃軸EE繝ｻ繝ｻEE陝ｶ蜻ｻE繝ｻ
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


		FirstNearVec = 0;//髫ｰE繝ｻE髫ｰE郢晢ｽｻ繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ遶雁�､EE鬮ｦE遶頑･｢EE繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE鬮ｴ閧ｴ霎ｨ陞ｻE驛｢E陷ｻ闌ｨEE繝ｻE驛｢E闕ｵ譏ｴ窶ｳ驛｢E繝ｻE驛｢譎｢EE驛｢譎冗樟繝ｻ蝣､E譎｢EE驛｢E繝ｻE驛｢譏ｴ繝ｻ郢晢ｽｨ
		mpTarget = nullptr;//鬨ｾE繝ｻE髫ｶ轣倡函郢晢ｽｻ驛｢E繝ｻE驛｢譎｢EE驛｢E繝ｻE驛｢譏ｴ繝ｻ郢晢ｽｨ驛｢E陋幢ｽｵE諛ウE繝ｻE驛｢譏ｴ繝ｻ郢晢ｽｨ

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
	if (Master::PauseOn == false)///郢晄亢繝ｻ郢ｧE騾包ｽｻ鬮ｱE邵ｺE邵ｺE邵ｺE邵ｺ繝ｻ竊醍ｹｧ閾･E譎・郢晢ｽｻ驛｢E繝ｻE鬨ｾ蛹・E鬯ｮE繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ遶雁�､EE郢晢ｽｻ
	{
		
		bar();
		
	}
	////////////////////////髣包ｽｳ隰梧汚EE繝ｻE鬩募∞EE鬮ｫ遨ゑｽｹ貅倪雷驍ｵE繝ｻE髫ｴ蠑ｱEE・ｻ驍ｵE繝ｻE驛｢譎丞ｹｲE讓抵ｽｹE繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE霑ｹ螟ｲEE闕ｵ譏ｶ譁｡驛｢E闕ｵ譎｢E閧ｲEE郢晢ｽｻ遶企豪EE陷ｷE繝ｻ繝ｻ//////////////
	if (Master::mpCamera->GetCamera3() == true)
	{
		//驛｢譎｢EE驛｢譏ｴ繝ｻE譎右E繝ｻE髫ｰE陷諤廢
		mpModel->Draw();
	}

	if (Master::mpDebug->Getdebug() == true)
	{
		//驍ｵE繝ｻE驛｢譎・郢晢ｽ｣驛｢E繝ｻE鬮ｯE繝ｻE鬩穂ｼ夲ｽｽE髫ｴ繝ｻEE髮手ｼ忍
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
		DrawFormatString(100, 300, GetColor(255, 255, 255), "髫ｰE繝ｻE髫ｰE郢晢ｽｻ鬲假ｽｨ:%f", GetAllStatusState(Object3D::Status_Attack));
		DrawFormatString(100, 400, GetColor(255, 255, 255), "鬯ｮE繝ｻE髯溷桁EE髯ｷ蟲ｨ繝ｻ%f", mpEquipmentManager->GetDamage());
		DrawFormatString(100, 500, GetColor(255, 255, 255), "s驛｢E繝ｻE驛｢譎・郢晢ｽｻ驛｢譏ｴ繝ｻ%f", GetAllStatusState(Object3D::Status_Speed));
		DrawFormatString(100, 350, GetColor(255, 255, 255), "鬩搾ｽｨ驕脱EE霓｣蛟ｪ縺殺E:%d", mpLevelUp->GetXp());
		DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f       Z:%f", mvPosition.x, mvPosition.y, mvPosition.z);
	}
	//髮殺EE髯懆ｶ｣EE驛｢E髮区ｨ奇ｽ｡竏ｫEEE驛｢E陋ｹE遶包ｽｧ驍ｵE繝ｻE鬨ｾ繝ｻ繝ｻ繝ｻE郢晢ｽｻ
	DrawSphere3D(
		mpModel->GetAttachmentPosition(),
		30.0f,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

//鬩募∞EE髯ｷ讎贋ｾ帷ｹ晢ｽｻ鬨ｾ繝ｻ繝ｻ邵ｺ蟶ｷE譏ｴ繝ｻ郢晢ｽｻ驛｢E繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE髯檸萓ｭ隨ｳ繝ｻEE鬮ｮ竏ｵ繝ｻ髯橸ｽｳ郢晢ｽｻ
void Player3D::MoveEx()
{
	AnimationState state = mpModel->GetNowState();
	if (state==ANIMATION_ATTACKJUMP||state == ANIMATION_ATTACK || state == ANIMATION_JUMP_OUT || state == ANIMATION_SLIDE||state==ANIMATION_ATTACKSLIDE||Master::ShopClassOn||Master::StatShopClassOn)
	{
		return;
	}//髴大､ｲEE髯橸ｽｳ陞｢E郢晢ｽｻ驛｢譎｢EE驛｢譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE髣包ｽｳ繝ｻE驍ｵE繝ｻE驍ｵE郢ｧ繝ｻE讙趣ｽｸE繝ｻE髣厄ｽｴ髴郁ｲｻE繧会ｽｸE陷会ｽｱ遶界EE郢晢ｽｻ

	moveVec = VGet(0.0f, 0.0f, 0.0f);//鬩募∞EE髯ｷ讎奇ｽ｢轣假ｽｩE髯ｷE郢晢ｽｻ
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髣包ｽｳEEE髯ｷE闔会ｽ｣郢晢ｽｻ驛｢E繝ｻE驛｢譎冗樟EE
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髯晢ｽｾ繝ｻE髫ｴ繝ｻEE髯ｷE闔会ｽ｣郢晢ｽｻ驛｢E繝ｻE驛｢譎冗樟EE
	//VECTOR DownMoveVector = VGet(0.0f, 0.0f, 0.0f);//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髣包ｽｳ陋ｹE陝E髯ｷE闔会ｽ｣郢晢ｽｻ驛｢E繝ｻE驛｢譎冗樟EE
	//VECTOR RightMoveVector = VGet(0.0f, 0.0f, 0.0f);//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髯ｷE繝ｻE髫ｴ繝ｻEE髯ｷE闔会ｽ｣郢晢ｽｻ驛｢E繝ｻE驛｢譎冗樟EE


	//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髯ｷE闔会ｽ｣遯ｶE驍ｵE闕ｵ譎｢E陋ｾﾂｧ繝ｻE髯ｷ蟠趣ｽｼ譁職驛｢E繝ｻE驛｢譎冗樟E譎右E陷ｻ闌ｨEE郢ｧ繝ｻE竏ｫEE郢晢ｽｻ
	{
		//髣包ｽｳEEE髯ｷE闔会ｽ｣遶剰ごEE繝ｻE鬩募∞EE髯ｷ蟠趣ｽｼ譁職驛｢E繝ｻE驛｢譎冗樟E譎右E繝ｻE驍ｵE遶丞｣ｹ窶ｳ驛｢譎｢EE驛｢譎｢EE鬮ｫ遨ゑｽｹ貅倪雷髫ｴ繝ｻEE髯ｷE闔会ｽ｣E・ｰ驛｢E髫ｰ竏ｬE蠕｡E蜷ｶ繝ｻ驛｢E陷ｻ蝓檸驍ｵE郢晢ｽｻ隨ｳ繝ｻEE郢ｧ繝ｻ繝ｻ驍ｵE繝ｻE驍ｵE陷ｷE繝ｻ繝ｻ
		UpMoveVector = VSub(Master::mpCamera->GetlookAtPosition(), Master::mpCamera->GetPosition());

		//髯晢ｽｾ繝ｻE髫ｴ繝ｻEE髯ｷE闔会ｽ｣遶剰ごEE繝ｻE鬩募∞EE髯ｷ蟠趣ｽｼ譁職驛｢E繝ｻE驛｢譎冗樟E譎右E陟募ｨｯ縺守ｫ擾ｽｽ繝ｻEEEE髯ｷE闔会ｽ｣郢晢ｽｻ鬩募∞EE髯ｷ蟠趣ｽｼ譁職驛｢E繝ｻE驛｢譎冗樟E譎右E繝ｻE驍ｵE邵ｲ繝ｻ諤髭E驍ｵE繝ｻE驛｢譎丞ｹｲE蟶ｷEE繝ｻE髫ｴ繝ｻEE髯ｷE闔会ｽ｣遶剰ごEE繝ｻE驛｢譎冗函邵ｺ驢搾ｽｹ譎冗樟E譎右E繝ｻE髯懷生E陝ｲE驍ｵE繝ｻE髫ｴ繝ｻEE髯ｷE隰・EE闔�E繝ｻE闕ｵ貊ゑｽｽE隰ｳE繝ｻE郢晢ｽｻ
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));


		//鬩募∞EE髯ｷ蟠趣ｽｼ譁職驛｢E繝ｻE驛｢譎冗樟E譎右E繝ｻE鬩募∞EE髯ｷ閧ｴ蝮ｩE驛｢E髮区ｧｫ繝ｻ髯ｷE繝ｻE驍ｵE陷会ｽｱ遶界EE郢晢ｽｻ郢晢ｽｻ驍ｵE繝ｻE驍ｵE遶擾ｽｵ繝ｻE繝ｻE鬮ｫ遨ゑｽｸ讒ｫEE陷会ｽｱ遯ｶE驍ｵE驗呻ｽｫ繝ｻE(驛｢譎冗函邵ｺ驢搾ｽｹ譎冗樟E譎右E繝ｻE鬯ｮ貊ゑｽｽE驍ｵE髴郁ｲｻE蛛ｵ繝ｻ闔会ｽ｣遶企豪EE陷ｷE繝ｻ迢暦ｽｸE髦ｮ蜷ｮ繝ｻ)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	//VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);//鬩募∞EE髯ｷ讎奇ｽ｢轣假ｽｩE髯ｷE郢晢ｽｻ

	if (CheckHitKey(KEY_INPUT_A))//髯晢ｽｾ繝ｻE髫ｴ繝ｻEE髯ｷE闔会ｽ｣遶剰ごEE繝ｻE鬩募∞EE鬯ｨE郢晢ｽｻ
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_D))//髯ｷE繝ｻE髫ｴ繝ｻEE髯ｷE闔会ｽ｣遶剰ごEE繝ｻE鬩募∞EE鬯ｨE郢晢ｽｻ
	{
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));

	}
	if (CheckHitKey(KEY_INPUT_W))//髯槭ｑEE髫ｴ繝ｻEE髯ｷE闔会ｽ｣遶剰ごEE繝ｻE鬩募∞EE鬯ｨE郢晢ｽｻ
	{
		moveVec = VAdd(moveVec, UpMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_S))//髫ｰ繝ｻ蜚ｱ霎ｯ謌奇ｽｭ繝ｻEE髯ｷE闔会ｽ｣遶剰ごEE繝ｻE鬩募∞EE鬯ｨE郢晢ｽｻ
	{
		moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));

	}
	//鬩募∞EE髯ｷ蟠趣ｽｼ雋ｻEE驍ｵE繝ｻE驍ｵE郢晢ｽｻ繝ｻ邇厄ｽｿE繝ｻE髫ｲE闕ｵ譏ｴ蝣､EE郢ｧ繝ｻE讙趣ｽｸE繝ｻE
	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{

		//鬩募∞EE髯ｷ讎奇ｽ｢轣假ｽｩE髯ｷE闔会ｽ｣繝ｻ螳夲ｽｱ繝ｻEE鬮ｫ遨ゑｽｸ讒ｫEE陷会ｽｱ遯ｶE驍ｵE驗呻ｽｫ繝ｻE
		moveVec = VNorm(moveVec);

		//髫ｴ繝ｻEE驍ｵE陷会ｽｱ繝ｻ讚∵綜隶哘EE繝ｻE驛｢E陋幢ｽｵ邵ｺ譎会ｽｹ譏ｴ繝ｻ郢晢ｽｨ
		mfTargetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;//驍ｵE髦ｮ蜻ｻE繝ｻEE繝ｻE髣忍EE邵ｺ蜀暦ｽｹ譎｢EE驛｢譎｢EE驛｢E繝ｻE驛｢E繝ｻE驕ｯE髴域喚E髯ｷE闔会ｽ｣繝ｻ讓抵ｽｸE繝ｻE驍ｵE郢晢ｽｻ繝ｻ邇厄ｽｭ繝ｻEE髯ｷE闔会ｽ｣繝ｻ螳壽殺E驍ｵE陷ｷEE郢ｧ繝ｻEE隴取得EE郢晢ｽｻ
	}
	//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驛｢譎樊味陝ｷ謌頑ｲり嵯譏ｴ繝ｻ驛｢E繝ｻE驛｢譏懶ｽｹ譁滄豪EE陋滂ｽ･繝ｻ繝ｻEE繝ｻE驍ｵE郢晢ｽｻ郢晢ｽｻ驍ｵE繝ｻE驍ｵE郢ｧ繝ｻE讙趣ｽｸE繝ｻE
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



	//鬩募∞EE髯ｷ蟠趣ｽｼ雋ｻE繝ｻEE陝ｶ蜻ｻE繝ｻ
	mvOldPosition = mvPosition;//髯ｷ隨ｬE髮・骰具ｽｸE繝ｻE髯溯ｶ｣EE髫ｶ轣倡函繝ｻ螳壼初E髫ｴ魃会ｽｽE髣厄ｽｫ隴殺E


	mvPosition = VAdd(mvPosition, VScale(moveVec, GetAllStatusState(Object3D::Status_Speed)));
	////////////////////////////////////////////////////////



		/////驛｢E繝ｻE驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢E繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE髯檸萓ｭ隨ｳ繝ｻEE鬮ｮ竏ｵ繝ｻ髯橸ｽｳ陞｢E繝ｻ蝣､EE陷ｷE繝ｻ繝ｻ///////////
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
				//驛｢E繝ｻE驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢E繝ｻE驍ｵE繝ｻE驛｢譎丞ｹｲE讓抵ｽｹE繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE驛｢E繝ｻE驛｢譎丞ｹｲ邵ｺ譎会ｽｹ譎｢EE驍ｵE隰疲ｻゑｽｽE髦ｮ蜷ｮ陞ｺ驍ｵE繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ繝ｻ邇匁捗繝ｻE髯ｷE郢晢ｽｻ
				if (pStage->CheckHit_Capsule(VAdd(mvPosition, VGet(0.0f, -150.0f, 0.0f)), VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
				{
					//髯檸萓ｭ隨ｳ繝ｻEE繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ繝ｻ迢暦ｽｸE繝ｻE驍ｵE郢ｧ繝ｻE蜥ｲEE郢晢ｽｻ郢晢ｽｻ驛｢譎｢EE驛｢E繝ｻE驛｢譎｢EE驍ｵE繝ｻE驍ｵE繝ｻE髫ｰ證ｦEE鬮ｫ證ｦEE髴難ｽ､繝ｻE驛｢E陷ｻ闌ｨEE郢ｧ繝ｻE竏ｫEE郢晢ｽｻ
					hitPos = pStage->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, 1000.0f, 0.0f)),//驛｢譎丞ｹｲE讓抵ｽｹE繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE鬮｢E隴取得EE髦ｮ蜷ｮ陞ｺ驛｢E郢晢ｽｻ髯樊ｺｷEE郢晢ｽｻ)驍ｵE繝ｻE
						VAdd(mvPosition, VGet(0.0f, -1000.0f, 0.0f))//驛｢譎丞ｹｲE讓抵ｽｹE繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髯戲EE繝ｻE髣包ｽｳ闕ｵ譏ｶ譌ｺ驍ｵEEE鬘費ｽｹE陜｣E繝ｻE陞｢E郢晢ｽｻ驍ｵE繝ｻE驍ｵE陷会ｽｱ遯ｶE髫ｰ謔ｶ繝ｻ繝ｻE郢晢ｽｻ
					);

					//髯檸萓ｭ隨ｳ繝ｻEE繝ｻE驍ｵEE隲｢E陞｢E繝ｻ螳壽╂隰費ｽｶE驍ｵE繝ｻE驍ｵE驗呻ｽｫ繝ｻE
					isHit = true;
				}
			}
		}
	}

	if (isHit)
	{
		//髯懶ｽｨ繝ｻE鬯ｮE繝ｻE驍ｵE繝ｻE髮取腸EE驍ｵE繝ｻE驍ｵE繝ｻE髮殺EE驍ｵE郢晢ｽｻ遯ｶE驍ｵE郢晢ｽｻ繝ｻ邇厄ｽｿE繝ｻE髫ｲE闕ｵ譏ｶ繝ｻ驍ｵE陷会ｽｱ遯ｶE驍ｵE邵ｲ繝ｻE雜｣EE髫ｶ轣倡函繝ｻ蝣､EE繝ｻE驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢E繝ｻE驍ｵE繝ｻE髯ｷE陋ｹE繝ｻ蜀暦ｽｸE陝ｶ蜻ｻE繝ｻ
		mvPosition.y = hitPos.y;
	}
	if (isHit == false)
	{
		//鬮ｫ陬懈桶鬮ｫE髯懈ｺ櫓
		mvPosition.y += -8.0f;//鬮｣諛ｶEE髣包ｽｳ闕ｵ譏ｶ繝ｻ驛｢E驕擾ｽｩEE繝ｻE繝ｻE
		if (mvPosition.y <= 0.0f || mvPosition.y <= hitPos.y)
		{
			mvPosition.y = hitPos.y;
		}
	}

	/////////////////////////髯橸ｽ｢遶丞｣ｹ繝ｻ髯檸萓ｭ隨ｳ繝ｻEE鬮ｮ竏ｵ繝ｻ髯橸ｽｳ郢晢ｽｻ///////////////////////////
	bool hitwall = false;
	bool hitwalls = false;
	auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
	if (!walls.empty())
	{
		// 髯檸萓ｭ隨ｳ繝ｻEE繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ繝ｻ邇匁､ｶ遶丞､ｲE蟶晏擅繝ｻE驍ｵE繝ｻE驛｢E郢晢ｽｻ
		// hint: 髴托ｽｴ繝ｻE髴托ｽ･繝ｻE驍ｵE繝ｻE髯ｷ繝ｻEE鬨ｾ繝ｻ繝ｻ邵ｲ蝣､EE繝ｻE1髫ｴE陞｢E郢晢ｽｻ髯橸ｽ｢遶丞､ｲEE驍ｵE陋ｹE隲､蜥弱♀郢ｧ迚咎｣ｭ驍ｵE繝ｻE髯具ｽｻ繝ｻE髯橸ｽｳ陞｢E繝ｻ繝ｻEE陟募ｨｯ繝ｻ驍ｵE郢晢ｽｻ郢晢ｽｻ驍ｵE繝ｻE驍ｵE郢晢ｽｻ髫ｴE陞滂ｽｲ繝ｻE闔�蛹・E繝ｻE髣包ｽｳ陞・EE陝ｲE遶頑･｢E蜀ｶ萓ｭ隨ｳ繝ｻEE繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ隨ｳ繝ｻ謦ｻ繝ｻE髯ｷE陋ｹE郢晢ｽｻ髯ｷ繝ｻEE鬨ｾ繝ｻ繝ｻ繝ｻ蟶晁箔郢晢ｽｻ遶擾ｽｴ驛｢E郢晢ｽｻ
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				// 驛｢譎丞ｹｲE讓抵ｽｹE繝ｻE驛｢譎｢EE驛｢譎｢EE驛｢E髮区ｨ奇ｽ｡竏ｫEEE驛｢E陋ｹE遶包ｽｧ驍ｵE繝ｻE驛｢E繝ｻE驛｢譎丞ｹｲ邵ｺ譎会ｽｹ譎｢EE髯懷姓E譏ｴ繝ｻ髯具ｽｻ繝ｻE髯橸ｽｳ陞｢E遶雁�､EE遶乗劼EE遶丞｣ｹ繝ｻ髣包ｽｳE繝ｻE髮区ｩｸEE繝ｻE驛｢譎・E諛ウE繝ｻE驛｢譎｢EE驍ｵE繝ｻE驍ｵE繝ｻE髯檸萓ｭ隨ｳ繝ｻEE鬮ｮ竏ｵ繝ｻ髯橸ｽｳ陞｢E繝ｻ蟶晏距陟募ｨｯ魘ｬ
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
					// 髯橸ｽ｢遶丞｣ｺ繝ｻ髮取腸EE驍ｵE繝ｻE驍ｵE繝ｻE鬮ｯE陟暮ｯ会ｽｿE驛｢E陋ｹE遶包ｽｧ驍ｵE繝ｻE驛｢譎冗函邵ｺ驢搾ｽｹ譎冗樟E譎右E髮区ｧｫ蠕宣辧霈忍
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 髯橸ｽ｢遶擾ｽｵ繝ｻE繝ｻE驍ｵE郢晢ｽｻ郢晢ｽｻ驛｢E繝ｻE驛｢譎冗樟EE
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 鬩募∞EE髯ｷ讎奇ｽ｢轣假ｽｩE髯ｷE闔会ｽ｣郢晢ｽｻ驛｢E繝ｻE驛｢譎冗樟E譎右E繝ｻE髯ｷE隶殺E繝ｻE驛｢譎冗函邵ｺ驢搾ｽｹ譎冗樟E譎右E繝ｻE驍ｵE遶乗劼EE遶丞｣ｹ繝ｻ髮主｢摘繝ｻE陞｢E遶雁�､EE繝ｻE髯ｷ郢晢ｽｻ繝ｻE鬮ｦE繝ｻ螳夲ｽｱ蠑ｱEEE竏ｫEE郢晢ｽｻ
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 髯橸ｽ｢遶擾ｽｵ繝ｻE繝ｻE驍ｵE郢晢ｽｻ郢晢ｽｻ驛｢E繝ｻE驛｢譎冗樟E譎右E陞ｳ螟ｲEE髢ｧE繝ｻE郢晢ｽｻ

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
	if (mnInvincibleTimer > 0) return; // 霎滂ｽ｡隰ｨE隴弱ｋ菫｣闕ｳE邵ｺE郢晉ｹ晢ｽ｡郢晢ｽｼ郢ｧE霎滂ｽ｡陷会ｽｹ
	if (damage - mpEquipmentManager->GetDamage() <= 0) { mfHp -= 1; return; }//隴幄抄蠑ｱ縲堤ｹｧ繧茨ｽｸ蟶呻ｽ臥ｸｺ繝ｻ
	if (now == ANIMATION_SLIDE)return;
	mfHp -= (damage-mpEquipmentManager->GetDamage());//髯ｬ繝ｻE邵ｺ蜉ｱ窶ｻ邵ｺ繝ｻE郢晉ｹ晢ｽ｡郢晢ｽｼ郢ｧE陋ｻ繝ｻ繝�郢晢ｽ｡郢晢ｽｼ郢ｧE郢ｧ蜻茨ｽｸ蟶呻ｽ臥ｸｺ繝ｻ

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
		
		// 郢ｧE郢晢ｽｩ郢ｧE郢戲縺・・ｳ郢ｧE鬮｢蜿･E蛹ｺ蜃ｾ邵ｺE霎滂ｽ｡隰ｨE隴弱ｋ菫｣郢ｧ蛛ｵ縺晉ｹ戲繝ｨ (陜難ｽｺ隴幢ｽｬ30郢晁ｼ釆樒ｹ晢ｽｼ郢・+ 髴托ｽｽ陷芽ｾ滂ｽ｡隰ｨE郢晁ｼ釆樒ｹ晢ｽｼ郢・
		mnInvincibleTimer = 30 + mnUpgradeEvasionInvincibility;
	}
	if (mpModel->GetNowState() == ANIMATION_SLIDE)
	{
		// 陜玲ｨ｣竏ｩ鬨ｾ貅ｷEE繝ｻ驛・譎槫ｱｬ繝ｻ蟲ｨ竊鍋ｹｧE郢戲繝ｻ郢ｧE郢晢ｽｬ郢晢ｽｼ郢晉甥繝ｻ郢ｧ螳夲ｽｶE邵ｺ繝ｻ
		mvPosition = VAdd(mvPosition, VScale(oldmoveVec, mfEvasionSpeed + mfUpgradeEvasionSpeed));
		mpModel->SetPosition(mvPosition);
	}






}


void Player3D::RotationByMove()
{

	//////髣碑崟螳ｦ鬯ｮ繝ｻEE郢晢ｽｻ遯ｶE驍ｵE繝ｻE驍ｵE陷会ｽｱ繝ｻ讓｣蝗馴寞讖ｸEE繝ｻE驛｢E陜｣E髢ｧ繝ｻ諤殺E
	//Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag_3D_Camera)
	//髴托ｽｴ繝ｻE髯懶ｽｨ繝ｻE驍ｵE繝ｻE髯懃軸E繝ｻEE繝ｻE髯区ｻゑｽｽE驍ｵE闕ｵ譎｢E陋ｾE繝ｻE髫ｶ轣倡函郢晢ｽｻ髯懃軸E繝ｻEE繝ｻE髯区ｻゑｽｽE驍ｵE繝ｻE髯晢ｽｾ繝ｻE髯具ｽｻ郢晢ｽｻ繝ｻ螳夲ｽｱ蠑ｱEEE竏ｫEE郢晢ｽｻ
	float subAngle = mfTargetAngle - mfAngle;



	//驍ｵE郢ｧ繝ｻE邇厄ｽｭ繝ｻEE髯ｷE闔会ｽ｣E・ｰ驛｢E陝ｲE遶暮ΔE陋ｹE陝E髯ｷE闔会ｽ｣郢晢ｽｻ髯晢ｽｾ繝ｻE驍ｵE鬲・・ｽE隰・EE陋妣EE闔牙遜EE繝ｻE髣比ｼ夲ｽｽE髣包ｽｳ陞・EE闔�蛹・E繝ｻE髣包ｽｳ陷茨ｽｷ繝ｻE陝ｲE遶企豪EE繝ｻE驛｢E闕ｵ譎｢E繝ｻEE繝ｻE驍ｵE陟募ｨｯ繝ｻ驍ｵE郢晢ｽｻ郢晢ｽｻ驍ｵE陞｢E遶界EE繝ｻE驍ｵE繝ｻE
	//髯晢ｽｾ繝ｻE驍ｵE繝ｻE髯区ｻゑｽｽE驍ｵE鬲・・ｽE隰・EE陋妣EE髯應ｼ夲ｽｽE繝ｻE髣包ｽｳ陞・EE闔�蛹・E繝ｻE髣包ｽｳ陷茨ｽｷ繝ｻE陝ｲE遶企豪EE繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE驍ｵE郢晢ｽｻ隨ｳ繝ｻEE髢ｾE雎ｬE髮殺EE驍ｵE陷ｷE繝ｻ繝ｻ
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//鬮ｫ闍難ｽｲ讖ｸEE繝ｻE驍ｵE繝ｻE髯晢ｽｾ繝ｻE髯具ｽｻ郢晢ｽｻ繝ｻ螳壽・EE郢晢ｽｻ遶願侭繝ｻ髣雁ｨｯ繝ｻ鬮ｴ蜿ｰEEE驍ｵE闔会ｽ｣繝ｻ繝ｻ
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


	//髣碑崟螳ｦ鬯ｮ繝ｻEE郢晢ｽｻ遯ｶE驍ｵE繝ｻE驍ｵE陷会ｽｱ繝ｻ讓｣蝗馴寞讖ｸEE繝ｻE驛｢E陜｣E髢ｧ繝ｻ諤殺E
	mfAngle = mfTargetAngle - subAngle;


	//髯懃軸E繝ｻEE繝ｻE髯区ｻゑｽｽE驛｢E陞ｳ螟ｲEE繝ｻE髯橸ｽｳ郢晢ｽｻ
	mvRotation.y = mfAngle + DX_PI_F;
	//驛｢譎｢EE驛｢譏ｴ繝ｻE譎右E繝ｻE髣費ｽｨ隴擾ｽｴ遶擾ｽｴ驛｢E郢晢ｽｻ
	mpModel->SetRotation(mvRotation);

	////髯懃軸E繝ｻEE繝ｻE髯区ｻゑｽｽE驛｢E陞ｳ螟ｲEE繝ｻE髯橸ｽｳ郢晢ｽｻ
	////驛｢譎｢EE驛｢譏ｴ繝ｻE譎右E繝ｻE髣費ｽｨ隴擾ｽｴ遶擾ｽｴ驛｢E郢晢ｽｻ


}




void Player3D::Jump()
{

	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		mvPosition.y += 300.0f;
		mbjump = true;
		mfjumpPower = JUMP_POWER;

		//驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驛｢譎樊味陝ｷ謌頑ｲり嵯譏ｴE驛｢譏懶ｽｹ譁滄豪E譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE驍ｵE陷ｷE繝ｻ繝ｻ
		
	}

	

}

void Player3D::Result()
{


}

void Player3D::Attack()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // 驛｢譎・E邵ｺ閧ｲEE繝ｻE驍ｵE繝ｻE髴托ｽ･繝ｻE髫ｲE闕ｵ譎｢E螳壽╂鬮｢繝ｻE郢晢ｽｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackCount >= AttackTime&&now!=ANIMATION_ATTACK)
	{
		AttackCount = 0;
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACK);
		//髫ｰE繝ｻE髫ｰE郢晢ｽｻE逧ｮE譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髯樊ｺｽ蛻､陝ｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//驛｢譎｢EE驛｢譎｢EE驛｢譎丞ｹｲ郢晢ｽｻ驍ｵE髴域喚髮ｷ驍ｵE繝ｻE驍ｵE郢晢ｽｻ
		mpModel->SetLoop(false);
		//驛｢譎｢EE驛｢譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE髯溷供E蠕窪髯溯ｼ忍繝ｻEE逧ｮE譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髫ｰ魃会ｽｽE驍ｵE郢晢ｽｻ
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
		

	}



}

void Player3D::AttackJump()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // 驛｢譎・E邵ｺ閧ｲEE繝ｻE驍ｵE繝ｻE髴托ｽ･繝ｻE髫ｲE闕ｵ譎｢E螳壽╂鬮｢繝ｻE郢晢ｽｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackJumpCount >= AttackJumpTime&&!mbjump)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_JUMP);
		mbjump = true;
		AttackJumpCount = 0;
		mfjumpPower = JUMP_POWER;
		//髫ｰE繝ｻE髫ｰE郢晢ｽｻE逧ｮE譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髯樊ｺｽ蛻､陝ｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACKJUMP);
		//驛｢譎｢EE驛｢譎｢EE驛｢譎丞ｹｲ郢晢ｽｻ驍ｵE髴域喚髮ｷ驍ｵE繝ｻE驍ｵE郢晢ｽｻ
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		//驛｢譎｢EE驛｢譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE髯溷供E蠕窪髯溯ｼ忍繝ｻEE逧ｮE譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髫ｰ魃会ｽｽE驍ｵE郢晢ｽｻ
		
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
			mvPosition.y = hitPos.y;//Y髯溯ｶ｣EE髫ｶ轣倡函繝ｻ繝ｻ驍ｵE繝ｻE驍ｵE陷会ｽｱ遯ｶE驍ｵE驗呻ｽｫ繝ｻE
			
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
	int mouseInput = GetMouseInput(); // 驛｢譎・E邵ｺ閧ｲEE繝ｻE驍ｵE繝ｻE髴托ｽ･繝ｻE髫ｲE闕ｵ譎｢E螳壽╂鬮｢繝ｻE郢晢ｽｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackSlideCount >= AttackSlideTime)
	{
		if (mpTarget == nullptr)return;//髫ｰE繝ｻE驛｢E陷ｻ驕ｺ讌ｳ鬩墓得EE驍ｵE繝ｻE驍ｵE鬮ｦE遶界EE繝ｻE驍ｵEEE隧�etrurn
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		if (mpModel->GetIsSeparate())mpModel->mpSeparateAnimation->SetAnimationCount(1.2f);
		else mpModel->mpAnimation->SetAnimationCount(1.2f);
		AttackSlideCount = 0;
		GoPosition = (VSub(mpTarget->GetPosition(), mvPosition));
		TargetPosition = VScale(GoPosition, 0.5f);

		//髫ｰE繝ｻE髫ｰE郢晢ｽｻE逧ｮE譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髯樊ｺｽ蛻､陝ｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACKSLIDE);
		//驛｢譎｢EE驛｢譎｢EE驛｢譎丞ｹｲ郢晢ｽｻ驍ｵE髴域喚髮ｷ驍ｵE繝ｻE驍ｵE郢晢ｽｻ
		mpModel->SetLoop(false);
		//驛｢譎｢EE驛｢譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE髯溷供E蠕窪髯溯ｼ忍繝ｻEE逧ｮE譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE驍ｵE繝ｻE髫ｰ魃会ｽｽE驍ｵE郢晢ｽｻ
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);

	}
	

	
	if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide)
	{
		new Effect(VAdd(mvPosition, VGet(0.0f, 0.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 255, 30, 20), 50.0f, 0.03f);
		GoPosition = VNorm(GoPosition);
		mfTargetAngle = atan2f(GoPosition.x, GoPosition.z);


		////////////////驛｢E繝ｻE驛｢譎｢EE驛｢E繝ｻE驛｢譎牙愛陋ｻE髫ｰE郢晢ｽｻ郢晢ｽｻ髯溯ｶ｣EE髫ｶ轣倩｡ｷ繝ｻE繝ｻE髯ｷ髦ｪ繝ｻ///////////////
		if (AttackSlideCount < 5)
		{
			mvPosition = VAdd(mvPosition, TargetPosition);
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


	mfHp = mfHp < 0 ? 0 : mfHp; //HP驍ｵE郢晢ｽｻ髫ｴ蟷｢EE髮畿驍ｵE繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE驍ｵEEE繝ｻ驛｢E髮区ｧｭ繝ｻ驛｢E陟暮ｯ会ｽｽ繝ｻ
	mfHp = mfHp > GetAllStatusState(Object3D::Status_Hp) ? GetAllStatusState(Object3D::Status_Hp) : mfHp; //HP驍ｵE隴ｴE隲､蜻ｵ譽忍E髯区ｻゑｽｽE驛｢E陞ｳ螟ｲEE郢晢ｽｻ遶擾ｽｴ驍ｵE繝ｻE驍ｵE郢晢ｽｻ隨ｳ繝ｻEE陝ｲEE遶擾ｽｵ隲､蜻ｵ譽忍E髯区ｻゑｽｽE驛｢E髮区ｧｭ繝ｻ驛｢E陟暮ｯ会ｽｽ繝ｻ
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

	DashGuage = DashGuage < 0 ? 0 : DashGuage; //HP驍ｵE郢晢ｽｻ髫ｴ蟷｢EE髮畿驍ｵE繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE驍ｵEEE繝ｻ驛｢E髮区ｧｭ繝ｻ驛｢E陟暮ｯ会ｽｽ繝ｻ
	DashGuage = DashGuage > MaxDashGauge ? MaxDashGauge : DashGuage; //HP驍ｵE隴ｴE隲､蜻ｵ譽忍E髯区ｻゑｽｽE驛｢E陞ｳ螟ｲEE郢晢ｽｻ遶擾ｽｴ驍ｵE繝ｻE驍ｵE郢晢ｽｻ隨ｳ繝ｻEE陝ｲEE遶擾ｽｵ隲､蜻ｵ譽忍E髯区ｻゑｽｽE驛｢E髮区ｧｭ繝ｻ驛｢E陟暮ｯ会ｽｽ繝ｻ
	unsigned int Color3;
	unsigned int Color4;
	Color3 = GetColor(255, 255, 255);
	Color4 = GetColor(0, 0, 0);
	DrawBox(pos2.x, pos2.y, pos2.x + size2.x, pos2.y + size2.y, Color3, true);
	DrawBox(pos2.x + 1, pos2.y + 1,
		pos2.x + size2.x * DashRatio - 1,
		pos2.y + size2.y - 1,
		GetColor(255, 255, 0), true);




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
		// 髣厄ｽｴ髴郁ｲｻE螳夲ｽｫE繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE驛｢E郢ｧ繝ｻEE郢晢ｽｻ隨櫓鞫手ｮ抵ｽｫ繝ｻE繝ｻE鬮ｦE繝ｻE ---------------- //
		Item::ItemInformation* itemInfo;
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::HEAL;
		itemInfo->Name = "回復薬";
		mpItemManager->AddItem(itemInfo);

		// --------------------------------------- //

		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 1;
		itemInfo->ID = Item::ItemID::HIGHHEAL;
		itemInfo->Name = "highheal";
		mpItemManager->AddItem(itemInfo);


		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::POWER;
		itemInfo->Name = "Power";
		mpItemManager->AddItem(itemInfo);

		// 鬨ｾ繝ｻ繝ｻE・ｦ
		// 髫ｲE繝ｻE驍ｵE繝ｻE驍ｵEEE驛｢E繝ｻE驛｢譏ｴ繝ｻE蝣､EE闕ｵ譎｢E霑ＵemInformation驛｢E髮区ｧｫ蠕宣辧蜍溷ｹｲ隨櫓EE郢晢ｽｻ
		// 驍ｵE隴擾ｽｴ繝ｻ讙趣ｽｹE隴ｽ蝸ｾemManager驍ｵE繝ｻE髮九ｑEE驍ｵE陷会ｽｱ遯ｶE驍ｵE郢ｧ繝ｻEE驛｢E郢晢ｽｻ

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
		// 髣厄ｽｴ髴郁ｲｻE螳夲ｽｫE繝ｻE驍ｵE繝ｻE驍ｵE繝ｻE驛｢E郢ｧ繝ｻEE郢晢ｽｻ隨櫓鞫手ｮ抵ｽｫ繝ｻE繝ｻE鬮ｦE繝ｻE ---------------- //
		Item::ItemInformation* itemInfo;
		
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 6;
		itemInfo->ID = Item::ItemID::SPEED;
		itemInfo->Name = "speed";
		mpItemManager->AddItem(itemInfo);
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
				
				// 髫ｰE繝ｻE髫ｰE郢晢ｽｻE逧ｮE譎｢EE驛｢E繝ｻE驛｢譎｢EE驛｢譎｢EE&&Attack1
				if (now == ANIMATION_ATTACK && AttackState == Attack_Normal && !mbjump && !pEne->GetHitjudgment())
				{
					pEne->SetHitjudgment(true);
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));
					AttackHitJudgmentflag = true;
					// 鬨ｾ蛹・E鬯ｮE繝ｻE髫ｰE繝ｻE驛｢E陟暮ｯ会ｽｼ繝ｻEE陝ｶ蜻ｻE繝ｻ
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

				// 鬨ｾ蛹・E鬯ｮE繝ｻE髫ｰE繝ｻE驛｢E陟暮ｯ会ｽｼ繝ｻEE陝ｶ蜻ｻE繝ｻ
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
				// 鬨ｾ蛹・E鬯ｮE繝ｻE髫ｰE繝ｻE驛｢E陟暮ｯ会ｽｼ繝ｻEE陝ｶ蜻ｻE繝ｻ
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
	if (!(now==ANIMATION_ATTACK)&& !(now == ANIMATION_ATTACKJUMP)&& !(now == ANIMATION_ATTACKSLIDE))//髯檸萓ｭ隨ｳ繝ｻEE鬮ｮ竏ｵ繝ｻ髯橸ｽｳ陞｢E郢晢ｽｻ髯溷桁EE髮趣ｽ｢繝ｻE
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
	// 髯檸萓ｭ隨ｳ繝ｻEE鬮ｮ竏ｵ繝ｻ髯橸ｽｳ陞｢E陝ｲE髫ｴ繝ｻEE
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
