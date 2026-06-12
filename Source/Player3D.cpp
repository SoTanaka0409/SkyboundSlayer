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

// é™‹ï½»ç¹ï½»è¿šï½¡éƒ¢E§ãƒ»E¢éƒ¢æ˜œï½¹æ–Ÿé‹E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³éƒ¢E§èœ‘ãEE½E½ãƒ»E¿é¨¾åŒE½½E¨é‚µEºèœ·E¶ãƒ»ç‹—ï½¸Eºèµæ˜´ãƒ»é««Eªãƒ»E­é™ï½³è¢E¹ãƒ»å¸æ†ãƒ»E½é™·ä¼šï½¿E½ç¹ï½»è›¹E»ç¹ï½§éƒ¢æ™E½¼æ–Â°éƒ¢æ™¢E½E«éƒ¢æ™ç¾ç¹ï½»falseç¹ï½»è åŒE½½E½ãƒ»E¿é¨¾åŒE½½E¨é‚µEºèœ‰ï½±ç«ŠéEE¸Eºç¹ï½»ãƒ»E¼è¨å¤²E½E¼ç¹ï½»
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool isSeparateAnim)
	:Object3D(initPos)
	, mfAttack(0)//éƒ¢æ•–Â€éƒ¢æ™¢E½E¡éƒ¢æ™¢E½E¼éƒ¢E§ãƒ»E¸
	, mfAttackjump(5)
	, mfAttackSlide(7)

	,OnJumpCollider(false)//é‚µEºé˜®å¶ãƒ»éœ‘ï½¸ãƒ»E¬é¬®E¢é˜®å®ãƒ»é™ŸåEä¾­ç¬³ãƒ»E¹E§é«®âˆµãƒ»é™ï½³è¢E¹ãƒ»å ¤E¸Eºãƒ»E¤é‚µEºè‰ï½£ãƒ»ãƒ»
	, mbInvisible(false)//éš°E¾ãƒ»E»éš°E¦ç¹ï½»çª¶E²é¬©E¥èŸ è¶£E½E¤ç¹ï½»ãƒ»E½é‚µEºãƒ»Eªé‚µEºç¹ï½»ãƒ»è‚²E¸Eºç¹ï½»ç«ŠãE
	, mbjump(false)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(speed)//speed
	, JUMP_POWER(jumppower)
	, mfHp(hp)//hp
	, mfMaxHp(hp)//maxhp
	, mfDashSpeed(speed * 2)
	, NewShieldFast(false)//éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E¼éƒ¢æ™¢E½E«éƒ¢æ™ï½³E¨ãƒ»å¸Â€å¢“ï½»è–™ãEé‚µEºèœ·E¶ãƒ»ç‹—ï½¸Eºèµæ˜¶ãƒ»é‚µEºç¹ï½»E‚ï½°
	, mfSize(60.0f)//é‚µEºç¹§ãƒ»èºéƒ¢E§é«®âˆµãƒ»é™ï½³ç¹ï½»
	, Pause(false)//éƒ¢æ™Eº¢ç¹ï½»éƒ¢E§ãƒ»Eºé¨¾åŒE½½E»é¬®E±ãƒ»E¢
	, ChangeCamera(0)//é««ç©‚ï½¹æº˜â—‹é™æº½åˆ¤èŸ²E©
	, DashGuage(100.0f)//
	, EneSerchCount(0)
	, Gpush(false)//é™Ÿï½¢ç¹ï½»ãƒ»E®ãƒ»Eºéš°å ‹Â€éƒ¢E§è››ï½µãƒ»E½é‚µEºãƒ»E¦é‚µEºç¹ï½»ãƒ»æ¨’ï½¸Eºç¹ï½»
	, GpushCount(2)//é™Ÿï½¢ç¹ï½»ãƒ»E®ãƒ»Eºéš°å ‹Â€é‚µEºãƒ»E®éƒ¢E§ãƒ»E«éƒ¢E§ãƒ»E¦éƒ¢æ™¢E½E³éƒ¢æ˜´ãƒ»
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
	mpInventory = new Inventory(Object3D::Tag3D_Player3D);
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
	//mpModel->AddAnimation(ANIMATION_SLIDE, "Resource/Model/Slide.mv1");
	mpModel->AddAnimation(ANIMATION_SLIDE, "Resource/Model/kaihi.mv1");

	Master::mpCamera->Initialize();
	//mpCamerafa = Master::mpCamerafa;

	float HpRatio = (float)mfHp / mfMaxHp;
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;
	{//é™‹ï½»è­ãEEE«¯E¬ç¹ï½»ãƒ»ãƒ»
		Weapon::WeaponDate* weaponInf;
		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 1000;
		weaponInf->filename = "Resource/3D/Sabel.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon1;
		weaponInf->name = "weapon1";
		mpWeaponManager->AddWeapon(weaponInf);
		mpWeaponManager->ChangeWeapon(weaponInf);

		mfNormalAttack = weaponInf->damage;//éš´å´¢Â€é™‹ï½»è­ï½´ç«Šæ¥¢å´•è­ãEEE«¯E¬ç¹ï½»ãƒ»å’ï½¸Eºãƒ»E®éš°E¾ãƒ»E»éš°E¦ç¹ï½»é­˜ï½¨é‚µEºãƒ»E®é™‹ï½»è­ãEEE™‹E¹é«¢Â€ç¹ï½»é¨¾ãƒ»ãƒ»ãƒ»å¸å‹—è •å¨¯é´¬
		mfAttack = weaponInf->damage;//éš´å´¢Â€é™‹ï½»è­ï½´ç«Šæ¥¢å´•è­ãEEE«¯E¬ç¹ï½»ãƒ»å’ï½¸Eºãƒ»E®éš°E¾ãƒ»E»éš°E¦ç¹ï½»é­˜ï½¨é‚µEºãƒ»E®é™‹ï½»è­ãEEE™‹E¹é«¢Â€ç¹ï½»é¨¾ãƒ»ãƒ»ãƒ»å¸å‹—è •å¨¯é´¬
	}




	MaxDashGauge = DashGuage;

	//mpTarget = new Enemy3D("", VGet(20000, 0, 0), 0, 0, 0, 0, 0);
	
	//mpCamera->ResetCameraPlayer(true);
	 // éƒ¢E§ãƒ»E³éƒ¢æ™¢E½E©éƒ¢E§ãƒ»E¤éƒ¢æ•–Â€éƒ¢æ™¢E½E¼é¨¾å¢“ï½»è–™ãE
	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 60.0f);
	mpAttackSlideCollider = new SphereCollider(this, mvPosition, 200.0f);
	mpSerchEnemyCollider = new SphereCollider(this, VAdd(mvPosition, VGet(0.0f, 120.0f, 0.0f)), 500.0f);
	mpAttackJumpCollider = new SphereCollider(this, VAdd(mvPosition, VGet(0.0f, 120.0f, 0.0f)), 300.0f);
	
	
	//WeaponHitPos = VAdd(mpModel->GetAttachmentPosition(), VGet(0.0f, 50.0f, 0.0f));
	
	AttackState = Attack_Normal;
	mpItemManager = Master::mpItemManager;
}

Player3D::~Player3D()
{
	delete mpModel;
	CollDelete();
}

void Player3D::Update()
{
	if (mnInvincibleTimer > 0) {
		mnInvincibleTimer--;
	}

	mpInventory->Update();//inventoryç¸ºE¯é«¢ä¹ï¿ ç¸ºE¦ç¹§ã‚E¼E¸ºè–™âE
	if (Master::ShopClassOn || Master::InventoryClasOn || Master::StatShopClassOn)return;
	///////////////////////////////////////////////
	

	if (InputManager::CheckDownKey(KEY_INPUT_O)&&!Master::NearShopOn)
	{
		Master::InventoryClasOn = true;
		Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//é™·ä¼šï½½E¹éš´E«è«ï½ºè±¬E¹
	}
		
	
	if (Master::PauseOn == false)//éƒ¢æ™Eº¢ç¹ï½»éƒ¢E§ãƒ»Eºé—•ï½³ãƒ»E­é‚µEºãƒ»E¯é™·å´ï½¼æ¿E°é‚µEºãƒ»Eªé‚µEºç¹ï½»ãƒ»è‚²E¸Eºç¹ï½»ç«Šé‹E¸Eºèœ·E¶ãƒ»ãƒ»
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
		////////////////////////é«¯E¦è¬”æº¯åŠE////////////////
		SelectAttack();
		Result();
		ResetNUETRAL();
		//éƒ¢E§ãƒ»E¸éƒ¢æ™¢E½E£éƒ¢æ™¢E½E³éƒ¢æ™E€æ‡ŠãEé¨¾ãƒ»ãƒ»
		CollPositionUpdate();
		Evasion();
		//éš°E¾ãƒ»E»éš°E¦ç¹ï½»ç¹ï½»é¨¾ãƒ»ãƒ»
		SelectAttack();
		//é©•å‡E½E»é™·æ¦Šä¾›ç¹ï½»é¨¾ãƒ»ãƒ»
		MoveEx();
		//é™œç²E«ãƒ»E½E»ãƒ»E¢é™·ãƒ»E½E¦é¨¾ãƒ»ãƒ»
		RotationByMove();
		EnemySerch();

		//Interval++;
		//if (Interval >= 10)//é¬©E«ç¹ï½»ãƒ»å®šï½¬E¦ç¹ï½»ç¬E½½éƒ¢E§ãƒ»E¤éƒ¢æ™¢E½E³éƒ¢E§ãƒ»E¿éƒ¢æ™¢E½E¼éƒ¢æ™ãEEãE
		//{
		//	Shot();
		//	Interval = 0;
		//}
		if (mpLevelUp->GetLevelUp())
		{ 
			Master::mpSoundManager->PlaySE(SoundManager::SE_LEVELUP);
			mfHp = GetAllStatusState(Object3D::Status_Hp);//hpéƒ¢E§é›‹æ§­ãƒ»é™œç²E§E­ãƒ»ãƒ»E¸EºèŸ¶å‘»E½ãƒ»
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


		FirstNearVec = 0;//éš°E¾ãƒ»E»éš°E¦ç¹ï½»ãƒ»E½é‚µEºãƒ»E¦é‚µEºãƒ»Eªé‚µEºç¹ï½»ç«Šå ¤E¸Eºé«¦Eªç«Šæ¥¢E¬E¨ãƒ»Eµé‚µEºãƒ»E¨é‚µEºãƒ»E®é«´è‚´è¾¨è»E¬éƒ¢E§èœ»èŒ¨E½E¸ãƒ»E¬éƒ¢E§èµæ˜´â€³éƒ¢E§ãƒ»E¦éƒ¢æ™¢E½E³éƒ¢æ™ç¾ãƒ»å ¤E¹æ™¢E½Eªéƒ¢E§ãƒ»E»éƒ¢æ˜´ãƒ»ç¹ï½¨
		mpTarget = nullptr;//é¨¾E¶ãƒ»E®éš¶ç˜ç”Ÿç¹ï½»éƒ¢E§ãƒ»E¿éƒ¢æ™¢E½E¼éƒ¢E§ãƒ»E²éƒ¢æ˜´ãƒ»ç¹ï½¨éƒ¢E§è››ï½µEæ‡ƒE¹E§ãƒ»E»éƒ¢æ˜´ãƒ»ç¹ï½¨

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
	if (Master::ShopClassOn || Master::InventoryClasOn || Master::StatShopClassOn)return;
	if (Master::PauseOn == false)///ç¹æ˜´ãƒ»ç¹§Eºé€•ï½»é«±E¢ç¸ºE§ç¸ºE¯ç¸ºEªç¸ºãƒ»â†‘ç¹§è‡¥E¹æ™Eº¢ç¹ï½»éƒ¢E§ãƒ»Eºé¨¾åŒE½½E»é¬®E±ãƒ»E¢é‚µEºãƒ»E§é‚µEºãƒ»E¯é‚µEºãƒ»Eªé‚µEºç¹ï½»ç«Šå ¤E¸Eºç¹ï½»
	{
		
		bar();
		
	}
	////////////////////////é—•ï½³è¬Œæ‰˜E½Eºãƒ»Eºé©•å‡E½E°é««ç©‚ï½¹æº˜â—‹é‚µEºãƒ»E®éš´å¼±EE¹ï½»é‚µEºãƒ»E¿éƒ¢æ™å¹²Eæ¨’ï½¹E§ãƒ»E¤éƒ¢æ™¢E½E¤éƒ¢æ™¢E½E¼é‚µEºè¿¹å¤²E½E¦èµæ˜¶æ–¡éƒ¢E§èµæ™¢E½è‚²E¸Eºç¹ï½»ç«Šé‹E¸Eºèœ·E¶ãƒ»ãƒ»//////////////
	if (Master::mpCamera->GetCamera3() == true)
	{
		//éƒ¢æ™¢E½E¢éƒ¢æ˜´ãƒ»Eæ™‰E¸Eºãƒ»E®éš°E½èœ€æ€œæE
		mpModel->Draw();
	}

	if (Master::mpDebug->Getdebug() == true)
	{
		//é‚µEºãƒ»E§éƒ¢æ™ãEç¹ï½£éƒ¢E§ãƒ»E¯é«¯E¦ãƒ»E¨é©•ä¼šï½½Eºéš´ãƒ»E½E¹é›è¼”ãE
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
		DrawFormatString(100, 300, GetColor(255, 255, 255), "éš°E¾ãƒ»E»éš°E¦ç¹ï½»é­˜ï½¨:%f", GetAllStatusState(Object3D::Status_Attack));
		DrawFormatString(100, 400, GetColor(255, 255, 255), "é¬®E¦ãƒ»E²é™ŸåŒ…E½E¡é™·å³¨ãƒ»%f", mpEquipmentManager->GetDamage());
		DrawFormatString(100, 500, GetColor(255, 255, 255), "séƒ¢E§ãƒ»E¹éƒ¢æ™E±’ç¹ï½»éƒ¢æ˜´ãƒ»%f", GetAllStatusState(Object3D::Status_Speed));
		DrawFormatString(100, 350, GetColor(255, 255, 255), "é©ï½¨é’ãEE½E¨è½£å€ªããEE¤:%d", mpLevelUp->GetXp());
		DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f       Z:%f", mvPosition.x, mvPosition.y, mvPosition.z);
	}
	//é›ãEE½E¦é™œè¶£E½E¨éƒ¢E§é›‹æ¨Šï½¡âˆ«E¹E§ç«E½¬éƒ¢E§è›¹E»ç«•ï½§é‚µEºãƒ»Eªé¨¾ãƒ»ãƒ»ãƒ»E½ç¹ï½»
	DrawSphere3D(
		mpModel->GetAttachmentPosition(),
		30.0f,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

//é©•å‡E½E»é™·æ¦Šä¾›ç¹ï½»é¨¾ãƒ»ãƒ»ç¸ºå¸·E¹æ˜´ãƒ»ç¹ï½»éƒ¢E§ãƒ»E¸é‚µEºãƒ»E¨é‚µEºãƒ»E®é™ŸåEä¾­ç¬³ãƒ»E¹E§é«®âˆµãƒ»é™ï½³ç¹ï½»
void Player3D::MoveEx()
{
	AnimationState state = mpModel->GetNowState();
	if (state==ANIMATION_ATTACKJUMP||state == ANIMATION_ATTACK || state == ANIMATION_JUMP_OUT || state == ANIMATION_SLIDE||state==ANIMATION_ATTACKSLIDE||Master::InventoryClasOn||Master::ShopClassOn||Master::StatShopClassOn)
	{
		return;
	}//éœ‘å¤²E½E¹é™ï½³è¢E¹ç¹ï½»éƒ¢æ™¢E½E¢éƒ¢æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é—•ï½³ãƒ»E­é‚µEºãƒ»E§é‚µEºç¹§ãƒ»E½æªï½¸Eºãƒ»E°é—–ï½´éœˆè²»E½ã‚‰ï½¸Eºèœ‰ï½±ç«ŠéEE¸Eºç¹ï½»

	moveVec = VGet(0.0f, 0.0f, 0.0f);//é©•å‡E½E»é™·æ¦Šï½¢ç˜ï½©E¿é™·E·ç¹ï½»
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);//éƒ¢E§ãƒ»E«éƒ¢æ™¢E½E¡éƒ¢æ™¢E½E©é‚µEºãƒ»E®é—•ï½³é¬E¤§E©E¿é™·E·è‰ï½£ç¹ï½»éƒ¢E§ãƒ»E¯éƒ¢æ™ç¾EãE
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);//éƒ¢E§ãƒ»E«éƒ¢æ™¢E½E¡éƒ¢æ™¢E½E©é‚µEºãƒ»E®é™ï½¾ãƒ»E¦éš´ãƒ»E½E¹é™·E·è‰ï½£ç¹ï½»éƒ¢E§ãƒ»E¯éƒ¢æ™ç¾EãE
	//VECTOR DownMoveVector = VGet(0.0f, 0.0f, 0.0f);//éƒ¢E§ãƒ»E«éƒ¢æ™¢E½E¡éƒ¢æ™¢E½E©é‚µEºãƒ»E®é—•ï½³è›¹EºèŸ€E¿é™·E·è‰ï½£ç¹ï½»éƒ¢E§ãƒ»E¯éƒ¢æ™ç¾EãE
	//VECTOR RightMoveVector = VGet(0.0f, 0.0f, 0.0f);//éƒ¢E§ãƒ»E«éƒ¢æ™¢E½E¡éƒ¢æ™¢E½E©é‚µEºãƒ»E®é™·E¿ãƒ»E³éš´ãƒ»E½E¹é™·E·è‰ï½£ç¹ï½»éƒ¢E§ãƒ»E¯éƒ¢æ™ç¾EãE


	//éƒ¢E§ãƒ»E«éƒ¢æ™¢E½E¡éƒ¢æ™¢E½E©é‚µEºãƒ»E®é™·E·è‰ï½£çª¶E³é‚µEºèµæ™¢E½è›¾Â§ãƒ»E»é™·å´ï½¼æ–ãEéƒ¢E§ãƒ»E¯éƒ¢æ™ç¾Eæ™‰E¹E§èœ»èŒ¨E½E±ç¹§ãƒ»E½âˆ«E¹E§ç¹ï½»
	{
		//é—•ï½³é¬E¤§E©E¿é™·E·è‰ï½£ç«è‚²E¸Eºãƒ»E®é©•å‡E½E»é™·å´ï½¼æ–ãEéƒ¢E§ãƒ»E¯éƒ¢æ™ç¾Eæ™‰E¸Eºãƒ»E¯é‚µE²ç«å£¹â€³éƒ¢æ™¢E½E¡éƒ¢æ™¢E½E©é««ç©‚ï½¹æº˜â—‹éš´ãƒ»E½E¹é™·E·è‰ï½£E‚ï½°éƒ¢E§éš°âˆ¬E¬å¾¡E»å¶ãƒ»éƒ¢E§èœ»åŸŸï¿½E¢é‚µEºç¹ï½»ç¬³ãƒ»E¹E§ç¹§ãƒ»ãƒ»é‚µEºãƒ»E¨é‚µEºèœ·E¶ãƒ»ãƒ»
		UpMoveVector = VSub(Master::mpCamera->GetlookAtPosition(), Master::mpCamera->GetPosition());
		//UpMoveVector.y = 0.0f;

		//é™ï½¾ãƒ»E¦éš´ãƒ»E½E¹é™·E·è‰ï½£ç«è‚²E¸Eºãƒ»E®é©•å‡E½E»é™·å´ï½¼æ–ãEéƒ¢E§ãƒ»E¯éƒ¢æ™ç¾Eæ™‰E¸Eºè •å¨¯ãç«ï½½ãƒ»E¸é¬E¤§E©E¿é™·E·è‰ï½£ç¹ï½»é©•å‡E½E»é™·å´ï½¼æ–ãEéƒ¢E§ãƒ»E¯éƒ¢æ™ç¾Eæ™‰E¸Eºãƒ»E¨é‚µE²ç¸²ãƒ»æ€•ãEE¸é‚µEºãƒ»E®éƒ¢æ™å¹²Eå¸·E¹E§ãƒ»E¹éš´ãƒ»E½E¹é™·E·è‰ï½£ç«è‚²E¸Eºãƒ»E®éƒ¢æ™ç”Ÿç¸ºé†ï½¹æ™ç¾Eæ™‰E¸Eºãƒ»E«é™œå¶E‰èŸ²E©é‚µEºãƒ»Eªéš´ãƒ»E½E¹é™·E·è¬E‡E½E¼è E¥ãƒ»E¤èµæ»‚ï½½E©è¬³E¾ãƒ»E¼ç¹ï½»
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));
		//leftMoveVector.y = 0.0f;


		//é©•å‡E½E»é™·å´ï½¼æ–ãEéƒ¢E§ãƒ»E¯éƒ¢æ™ç¾Eæ™‰E¸Eºãƒ»E¯é©•å‡E½E»é™·è‚´å©çºE½¼éƒ¢E§é›‹æ§«ãƒ»é™·E»ãƒ»E³é‚µEºèœ‰ï½±ç«ŠéEE¸Eºç¹ï½»ç¹ï½»é‚µEºãƒ»E§é‚µE²ç«ï½µãƒ»E­ãƒ»E£é««ç©‚ï½¸æ§«å¯E‚µEºèœ‰ï½±çª¶E»é‚µEºé„™ï½«ãƒ»E¥(éƒ¢æ™ç”Ÿç¸ºé†ï½¹æ™ç¾Eæ™‰E¸Eºãƒ»E®é¬®æ»‚ï½½E·é‚µEºéœˆè²»E½åµãƒ»è‰ï½£ç«Šé‹E¸Eºèœ·E¶ãƒ»ç‹—ï½¸Eºé˜®å®ãƒ»)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	//VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);//é©•å‡E½E»é™·æ¦Šï½¢ç˜ï½©E¿é™·E·ç¹ï½»

	if (CheckHitKey(KEY_INPUT_A))//é™ï½¾ãƒ»E¦éš´ãƒ»E½E¹é™·E·è‰ï½£ç«è‚²E¸Eºãƒ»E®é©•å‡E½E»é¬¨E¾ç¹ï½»
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_D))//é™·E¿ãƒ»E³éš´ãƒ»E½E¹é™·E·è‰ï½£ç«è‚²E¸Eºãƒ»E®é©•å‡E½E»é¬¨E¾ç¹ï½»
	{
		//veVec.x = 1.0f;
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
		//moveVec = VSub(moveVec, leftMoveVector);

	}
	if (CheckHitKey(KEY_INPUT_W))//é™ã‚‘E½E¥éš´ãƒ»E½E¹é™·E·è‰ï½£ç«è‚²E¸Eºãƒ»E®é©•å‡E½E»é¬¨E¾ç¹ï½»
	{
		//veVec.z = 1.0;
		moveVec = VAdd(moveVec, UpMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_S))//éš°ãƒ»å”±è¾¯æˆŠï½­ãƒ»E½E¹é™·E·è‰ï½£ç«è‚²E¸Eºãƒ»E®é©•å‡E½E»é¬¨E¾ç¹ï½»
	{
		//moveVec.z = -1.0;
		moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));
		//moveVec = VSub(moveVec, UpMoveVector);

	}
	//é©•å‡E½E»é™·å´ï½¼è²»E¼E½é‚µEºãƒ»E¦é‚µEºç¹ï½»ãƒ»ç–ï½¿E¥ãƒ»E¶éš²E·èµæ˜´Â€å ¤E¸Eºç¹§ãƒ»E½æªï½¸Eºãƒ»E°
	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{
		//mpModel->ChangeAnimation(ModelAnimation::ANIMATION_RUN);

		//é©•å‡E½E»é™·æ¦Šï½¢ç˜ï½©E¿é™·E·è‰ï½£ãƒ»å®šï½±ãƒ»E½E£é««ç©‚ï½¸æ§«å¯E‚µEºèœ‰ï½±çª¶E»é‚µEºé„™ï½«ãƒ»E¥
		moveVec = VNorm(moveVec);

		//éš´ãƒ»E½E°é‚µEºèœ‰ï½±ãƒ»æ«æ‘è®šãEE½E»ãƒ»E¢éƒ¢E§è››ï½µç¸ºæ™‰ï½¹æ˜´ãƒ»ç¹ï½¨
		mfTargetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;//é‚µEºé˜®å‘»E¼ãƒ»E¸Eºãƒ»E§é—”èEE¿E«ç¸ºå†—ï½¹æ™¢E½E£éƒ¢æ™¢E½E©éƒ¢E§ãƒ»E¯éƒ¢E§ãƒ»E¿é¯E¶éœˆæŠ«Â€E²é™·E·è‰ï½£ãƒ»æ¨’ï½¸Eºãƒ»E¦é‚µEºç¹ï½»ãƒ»ç–ï½­ãƒ»E½E¹é™·E·è‰ï½£ãƒ»å®šæ€ãEEºé‚µEºèœ·E¶ç«E½¬ç¹§ãƒ»E½E¿è­æ“¾E½E­ç¹ï½»
	}
	//éƒ¢E§ãƒ»E¸éƒ¢æ™¢E½E£éƒ¢æ™¢E½E³éƒ¢æ™æ–¡èŸ·æˆŠæ²‚èµæ˜´ãƒ»éƒ¢E§ãƒ»E¢éƒ¢æ˜œï½¹æ–Ÿé‹E¸Eºè›Ÿï½¥ãƒ»ãƒ»E¸Eºãƒ»Eªé‚µEºç¹ï½»ç¹ï½»é‚µEºãƒ»E§é‚µEºç¹§ãƒ»E½æªï½¸Eºãƒ»E°
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



	//é©•å‡E½E»é™·å´ï½¼è²»E¼ãƒ»E¸EºèŸ¶å‘»E½ãƒ»
	mvOldPosition = mvPosition;//é™·ç¬¬E¦é›E½±é‹ï½¸Eºãƒ»E®é™Ÿè¶£E½E§éš¶ç˜ç”Ÿãƒ»å®šå‰ç«E½¬éš´é¯‰ï½½E¦é—–ï½«è­ãEäºE


	mvPosition = VAdd(mvPosition, VScale(moveVec, GetAllStatusState(Object3D::Status_Speed)));
	////////////////////////////////////////////////////////



		/////éƒ¢E§ãƒ»E¹éƒ¢æ˜´ãƒ»ç¹ï½»éƒ¢E§ãƒ»E¸é‚µEºãƒ»E¨é‚µEºãƒ»E®é™ŸåEä¾­ç¬³ãƒ»E¹E§é«®âˆµãƒ»é™ï½³è¢E¹ãƒ»å ¤E¸Eºèœ·E¶ãƒ»ãƒ»///////////
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
				//éƒ¢E§ãƒ»E¹éƒ¢æ˜´ãƒ»ç¹ï½»éƒ¢E§ãƒ»E¸é‚µEºãƒ»E¨éƒ¢æ™å¹²Eæ¨’ï½¹E§ãƒ»E¤éƒ¢æ™¢E½E¤éƒ¢æ™¢E½E¼é‚µEºãƒ»E®éƒ¢E§ãƒ»E«éƒ¢æ™å¹²ç¸ºæ™‰ï½¹æ™¢E½E«é‚µEºè¬”æ»‚ï½½E½é˜®å®èºé‚µEºãƒ»E£é‚µEºãƒ»E¦é‚µEºç¹ï½»ãƒ»ç–æ’»ãƒ»E´é™·E·ç¹ï½»
				if (pStage->CheckHit_Capsule(VAdd(mvPosition, VGet(0.0f, -150.0f, 0.0f)), VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
				{
					//é™ŸåEä¾­ç¬³ãƒ»E¸Eºãƒ»E£é‚µEºãƒ»E¦é‚µEºç¹ï½»ãƒ»ç‹—ï½¸Eºãƒ»E§é‚µEºç¹§ãƒ»E½å’²E¸Eºç¹ï½»ç¹ï½»éƒ¢æ™¢E½Eªéƒ¢E§ãƒ»E´éƒ¢æ™¢E½E³é‚µEºãƒ»E¨é‚µEºãƒ»E®éš°æš¦E½E¥é««æš¦E½E¦éœ“ï½¤ãƒ»E¹éƒ¢E§èœ»èŒ¨E½E±ç¹§ãƒ»E½âˆ«E¹E§ç¹ï½»
					hitPos = pStage->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, 1000.0f, 0.0f)),//éƒ¢æ™å¹²Eæ¨’ï½¹E§ãƒ»E¤éƒ¢æ™¢E½E¤éƒ¢æ™¢E½E¼é‚µEºãƒ»E®é«¢E¹è­æ“¾E½E½é˜®å®èºéƒ¢E§ç¹ï½»é™æº·E£E¼ç¹ï½»)é‚µEºãƒ»E¨
						VAdd(mvPosition, VGet(0.0f, -1000.0f, 0.0f))//éƒ¢æ™å¹²Eæ¨’ï½¹E§ãƒ»E¤éƒ¢æ™¢E½E¤éƒ¢æ™¢E½E¼é‚µEºãƒ»E®é™ãEE»E£ãƒ»E½é—•ï½³èµæ˜¶æ—ºé‚µEºè²E‡E½é¡”ï½¹E§è£E¤ãƒ»E·è¢E¼ç¹ï½»é‚µEºãƒ»E¨é‚µEºèœ‰ï½±çª¶E»éš°æ‚¶ãƒ»ãƒ»E®ç¹ï½»
					);

					//é™ŸåEä¾­ç¬³ãƒ»E¸Eºãƒ»E£é‚µEºè²E½·è«¢å¹E¥œè¢E¹ãƒ»å®šæ„¾è¬”ï½¶ç¬E½²é‚µEºãƒ»E¦é‚µEºé„™ï½«ãƒ»E¥
					isHit = true;
				}
			}
		}
	}

	if (isHit)
	{
		//é™œï½¨ãƒ»E°é¬®E±ãƒ»E¢é‚µEºãƒ»E«é›æ’°E½E¿é‚µEºãƒ»E£é‚µEºãƒ»E¦é›ãEE½E©é‚µEºç¹ï½»çª¶E»é‚µEºç¹ï½»ãƒ»ç–ï½¿E¥ãƒ»E¶éš²E·èµæ˜¶ãƒ»é‚µEºèœ‰ï½±çª¶E»é‚µE²ç¸²ãƒ»E½è¶£E½E§éš¶ç˜ç”Ÿãƒ»å ¤E¹E§ãƒ»E¹éƒ¢æ˜´ãƒ»ç¹ï½»éƒ¢E§ãƒ»E¸é‚µEºãƒ»E«é™·E·è›¹E»ãƒ»å†—ï½¸EºèŸ¶å‘»E½ãƒ»
		mvPosition.y = hitPos.y;
	}
	if (isHit == false)
	{
		//é««è£œæ‰±é««E¼é™œæº˜ãE
		mvPosition.y += -8.0f;//é«£æ‡¶E½E½é—•ï½³èµæ˜¶ãƒ»éƒ¢E§éï½©ç«E½¬è²E½·ãƒ»Eºãƒ»E¦
		if (mvPosition.y <= 0.0f || mvPosition.y <= hitPos.y)
		{
			mvPosition.y = hitPos.y;
		}
	}

	/////////////////////////é™ï½¢ç«å£¹ãƒ»é™ŸåEä¾­ç¬³ãƒ»E¹E§é«®âˆµãƒ»é™ï½³ç¹ï½»///////////////////////////
	bool hitwall = false;
	bool hitwalls = false;
	auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
	if (!walls.empty())
	{
		// é™ŸåEä¾­ç¬³ãƒ»E¸Eºãƒ»E£é‚µEºãƒ»E¦é‚µEºç¹ï½»ãƒ»ç–æ¤¶ç«å¤²E½å¸å¡ãƒ»E¿é‚µEºãƒ»E¹éƒ¢E§ç¹ï½»
		// hint: éœ‘ï½´ãƒ»E¾éœ‘ï½¥ãƒ»E¶é‚µEºãƒ»E®é™·ãƒ»E½E¦é¨¾ãƒ»ãƒ»ç¸²å ¤E¸Eºãƒ»E¯1éš´E«è¢E¹ç¹ï½»é™ï½¢ç«å¤²E¼E½é‚µEºè›¹Eºè«¤å’ãŠç¹§ç‰™é£­é‚µEºãƒ»E«é™‹ï½»ãƒ»E¤é™ï½³è¢E¹ãƒ»ãƒ»E¹E§è •å¨¯ãƒ»é‚µEºç¹ï½»ç¹ï½»é‚µEºãƒ»E§é‚µE²ç¹ï½»éš´E«èŸï½²ãƒ»E¼è åŒE½½E»ãƒ»E¥é—•ï½³è‚ãEE½E¼èŸ²E¨ç«Šæ¥¢E½å†¶ä¾­ç¬³ãƒ»E¸Eºãƒ»E£é‚µEºãƒ»E¦é‚µEºç¹ï½»ç¬³ãƒ»æ’»ãƒ»E´é™·E·è›¹E»ç¹ï½»é™·ãƒ»E½E¦é¨¾ãƒ»ãƒ»ãƒ»å¸è”“ç¹ï½»ç«ï½´éƒ¢E§ç¹ï½»
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				// éƒ¢æ™å¹²Eæ¨’ï½¹E§ãƒ»E¤éƒ¢æ™¢E½E¤éƒ¢æ™¢E½E¼éƒ¢E§é›‹æ¨Šï½¡âˆ«E¹E§ç«E½¬éƒ¢E§è›¹E»ç«•ï½§é‚µEºãƒ»Eªéƒ¢E§ãƒ»E«éƒ¢æ™å¹²ç¸ºæ™‰ï½¹æ™¢E½E«é™œå©E¹æ˜´ãƒ»é™‹ï½»ãƒ»E¤é™ï½³è¢E¹ç«Šå ¤E¸E²ç«æ™¢E½E£ç«å£¹ãƒ»é—•ï½³é—E½½ãƒ»E§é›‹æ©¸E½E½ãƒ»E¢éƒ¢æ™Eº¢Eæ‡ƒE¹E§ãƒ»E´éƒ¢æ™¢E½E³é‚µEºãƒ»E¨é‚µEºãƒ»E®é™ŸåEä¾­ç¬³ãƒ»E¹E§é«®âˆµãƒ»é™ï½³è¢E¹ãƒ»å¸å‹—è •å¨¯é´¬
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
					// é™ï½¢ç«å£ºãƒ»é›æ’°E½E¿é‚µEºãƒ»E£é‚µEºãƒ»E¦é«¯E¦è •é¯‰ï½¿E¥éƒ¢E§è›¹E»ç«•ï½§é‚µEºãƒ»Eªéƒ¢æ™ç”Ÿç¸ºé†ï½¹æ™ç¾Eæ™‰E¹E§é›‹æ§«å¾é™Ÿè¼”ãE
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // é™ï½¢ç«ï½µãƒ»E²ãƒ»E¿é‚µEºç¹ï½»ç¹ï½»éƒ¢E§ãƒ»E¯éƒ¢æ™ç¾EãE
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // é©•å‡E½E»é™·æ¦Šï½¢ç˜ï½©E¿é™·E·è‰ï½£ç¹ï½»éƒ¢E§ãƒ»E¯éƒ¢æ™ç¾Eæ™‰E¸Eºãƒ»E®é™·E¿è®å¥E½½E¯ãƒ»E¾éƒ¢æ™ç”Ÿç¸ºé†ï½¹æ™ç¾Eæ™‰E¸Eºãƒ»E¨é‚µE²ç«æ™¢E½E£ç«å£¹ãƒ»é›å¢“ãEãƒ»E·è¢E¹ç«Šå ¤E¸Eºãƒ»E®é™·â‚¬ç¹ï½»ãƒ»E©é«¦Eªãƒ»å®šï½±å¼±EEEâˆ«E¹E§ç¹ï½»
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // é™ï½¢ç«ï½µãƒ»E²ãƒ»E¿é‚µEºç¹ï½»ç¹ï½»éƒ¢E§ãƒ»E¯éƒ¢æ™ç¾Eæ™‰E¹E§è³å¤²E½E¨é–§E²ãƒ»E®ç¹ï½»

					if (hitwall == true && hitwalls == false)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
						//hitwall = false;
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
	if (mnInvincibleTimer > 0) return; // è¾Ÿï½¡è¬¨Eµè­ã‚‹ä¿£è³E­ç¸ºE¯ç¹Â€ç¹ï½¡ç¹ï½¼ç¹§E¸è¾Ÿï½¡èœ‰ï½¹
	if (damage - mpEquipmentManager->GetDamage() <= 0) { mfHp -= 1; return; }//è­›Â€è´å¼±ã€’ç¹§ã‚ˆï½¸å¸™ï½‰ç¸ºãƒ»
	if (now == ANIMATION_SLIDE)return;
	mfHp -= (damage-mpEquipmentManager->GetDamage());//é™¬ãƒ»E™ç¸ºåŠ±â€»ç¸ºãƒ»E‹ç¹Â€ç¹ï½¡ç¹ï½¼ç¹§E¸è›»ãƒ»ãƒ ç¹ï½¡ç¹ï½¼ç¹§E¸ç¹§å‘ˆï½¸å¸™ï½‰ç¸ºãƒ»

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
		
		// ç¹§E¹ç¹ï½©ç¹§E¤ç¹ãEãE¹ï½³ç¹§E°é«¢å¥E§åŒºå‡¾ç¸ºE«è¾Ÿï½¡è¬¨Eµè­ã‚‹ä¿£ç¹§åµãç¹ãEãƒ¨ (è“ï½ºè­›ï½¬30ç¹è¼”Îç¹ï½¼ç¹ï¿½ + éœ‘ï½½èœ‰ï¿½è¾Ÿï½¡è¬¨Eµç¹è¼”Îç¹ï½¼ç¹ï¿½)
		mnInvincibleTimer = 30 + mnUpgradeEvasionInvincibility;
	}
	if (mpModel->GetNowState() == ANIMATION_SLIDE)
	{
		// è—æ¨£âˆ©é¨¾æº·EºE¦ãƒ»éƒE½·æ™å±¬ãƒ»å³¨â†“ç¹§E¢ç¹ãEãƒ»ç¹§E°ç¹ï½¬ç¹ï½¼ç¹ç‰™ãƒ»ç¹§å®šï½¶E³ç¸ºãƒ»
		mvPosition = VAdd(mvPosition, VScale(oldmoveVec, mfEvasionSpeed + mfUpgradeEvasionSpeed));
		mpModel->SetPosition(mvPosition);
	}






}


void Player3D::RotationByMove()
{

	//////é—”è›¾å®¦é¬®ãƒ»E¸Eºç¹ï½»çª¶E»é‚µEºãƒ»E»é‚µEºèœ‰ï½±ãƒ»æ¨£å›“é›‹æ©¸E½Eºãƒ»E¦éƒ¢E§è£E¤é–§ãƒ»æ€ãEEº
	//Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag_3D_Camera)
	//éœ‘ï½´ãƒ»E¾é™œï½¨ãƒ»E¨é‚µEºãƒ»E®é™œç²E«ãƒ»E½E»ãƒ»E¢é™‹æ»‚ï½½E¤é‚µEºèµæ™¢E½è›¾Â€E¶ãƒ»E®éš¶ç˜ç”Ÿç¹ï½»é™œç²E«ãƒ»E½E»ãƒ»E¢é™‹æ»‚ï½½E¤é‚µEºãƒ»E®é™ï½¾ãƒ»E®é™‹ï½»ç¹ï½»ãƒ»å®šï½±å¼±EEEâˆ«E¹E§ç¹ï½»
	float subAngle = mfTargetAngle - mfAngle;



	//é‚µEºç¹§ãƒ»E½ç–ï½­ãƒ»E½E¹é™·E·è‰ï½£E‚ï½°éƒ¢E§èŸ²E¨ç«•ï¿½éƒ¢E§è›¹EºèŸ€E¿é™·E·è‰ï½£ç¹ï½»é™ï½¾ãƒ»E®é‚µEºé­E¼šï½½E¼è¬E‡E½E¼è››ãEE½E¼è‰å‘»E½Eºãƒ»E¦é—”ä¼šï½½E¥é—•ï½³è‚ãEE½E¼è åŒE½½E»ãƒ»E¥é—•ï½³èœˆï½·ãƒ»E¼èŸ²E¨ç«Šé‹E¸Eºãƒ»Eªéƒ¢E§èµæ™¢E¼ãƒ»E¸Eºãƒ»E¨é‚µEºè •å¨¯ãƒ»é‚µEºç¹ï½»ç¹ï½»é‚µEºè¢E¹ç«ŠéEE¸Eºãƒ»E®é‚µEºãƒ»E§
	//é™ï½¾ãƒ»E®é‚µEºãƒ»E®é™‹æ»‚ï½½E¤é‚µEºé­E¼šï½½E¼è¬E‡E½E¼è››ãEE½E¼é™œä¼šï½½E»ãƒ»E¥é—•ï½³è‚ãEE½E¼è åŒE½½E»ãƒ»E¥é—•ï½³èœˆï½·ãƒ»E¼èŸ²E¨ç«Šé‹E¸Eºãƒ»Eªé‚µEºãƒ»E£é‚µEºãƒ»E¦é‚µEºç¹ï½»ç¬³ãƒ»E¹E§é–¾E¥è±¬E£é›ãEE½E£é‚µEºèœ·E¶ãƒ»ãƒ»
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//é««è‹“ï½²æ©¸E½Eºãƒ»E¦é‚µEºãƒ»E®é™ï½¾ãƒ»E®é™‹ï½»ç¹ï½»ãƒ»å®šï¿½æˆŠãEE‚Â€ç¹ï½»ç«Šè–™ãƒ»é—Šå¨¯ãƒ»é«´å°E»E£ç¬E½¼é‚µEºè‰ï½£ãƒ»ãƒ»
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

	//(400, 560, GetColor(0, 0, 0), "subAngle::%f",subAngle);

	//é—”è›¾å®¦é¬®ãƒ»E¸Eºç¹ï½»çª¶E»é‚µEºãƒ»E»é‚µEºèœ‰ï½±ãƒ»æ¨£å›“é›‹æ©¸E½Eºãƒ»E¦éƒ¢E§è£E¤é–§ãƒ»æ€ãEEº
	mfAngle = mfTargetAngle - subAngle;

	//DrawFormatString(400, 600, GetColor(0, 0, 0), "mfAngle::%f", subAngle);

	//é™œç²E«ãƒ»E½E»ãƒ»E¢é™‹æ»‚ï½½E¤éƒ¢E§è³å¤²E½E¨ãƒ»E­é™ï½³ç¹ï½»
	mvRotation.y = mfAngle + DX_PI_F;
	//éƒ¢æ™¢E½E¢éƒ¢æ˜´ãƒ»Eæ™‰E¸Eºãƒ»E«é—”ï½¨è­ï½´ç«ï½´éƒ¢E§ç¹ï½»
	mpModel->SetRotation(mvRotation);
	//mfAngle = mfTargetAngle - Master::mpCamera->GetLookCamera();

	////é™œç²E«ãƒ»E½E»ãƒ»E¢é™‹æ»‚ï½½E¤éƒ¢E§è³å¤²E½E¨ãƒ»E­é™ï½³ç¹ï½»
	//mvRotation.y = mpCamera->GetlookAtPosition().y; //mfAngle + DX_PI_F;
	////éƒ¢æ™¢E½E¢éƒ¢æ˜´ãƒ»Eæ™‰E¸Eºãƒ»E«é—”ï½¨è­ï½´ç«ï½´éƒ¢E§ç¹ï½»
	//mpModel->SetRotation(mvRotation);


}




void Player3D::Jump()
{

	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		mvPosition.y += 300.0f;
		mbjump = true;
		mfjumpPower = JUMP_POWER;

		//éƒ¢E§ãƒ»E¸éƒ¢æ™¢E½E£éƒ¢æ™¢E½E³éƒ¢æ™æ–¡èŸ·æˆŠæ²‚èµæ˜´Eéƒ¢æ˜œï½¹æ–Ÿé‹E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é‚µEºãƒ»E«é‚µEºèœ·E¶ãƒ»ãƒ»
		
	}

	

}

void Player3D::Result()
{


}

void Player3D::Attack()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // éƒ¢æ™E½§E­ç¸ºè‚²E¹E§ãƒ»E¹é‚µEºãƒ»E®éœ‘ï½¥ãƒ»E¶éš²E·èµæ™¢E½å®šæ„¾é«¢Â€ãƒ»E¾ç¹ï½»
	if (mouseInput & MOUSE_INPUT_LEFT && AttackCount >= AttackTime&&now!=ANIMATION_ATTACK)
	{
		AttackCount = 0;
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACK);
		//éš°E¾ãƒ»E»éš°E¦ç¹ï½»Eçš®E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é‚µEºãƒ»E«é™æº½åˆ¤èŸ²E©
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//éƒ¢æ™¢E½E«éƒ¢æ™¢E½E¼éƒ¢æ™å¹²ç¹ï½»é‚µEºéœˆæŠ«é›·é‚µEºãƒ»Eªé‚µEºç¹ï½»
		mpModel->SetLoop(false);
		//éƒ¢æ™¢E½E¢éƒ¢æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é™Ÿå‹ŸE¾å¾ŒãEé™Ÿè¼”ãEãƒ»E©è²E½©çš®E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é‚µEºãƒ»E«éš°é¯‰ï½½E»é‚µEºç¹ï½»
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
	int mouseInput = GetMouseInput(); // éƒ¢æ™E½§E­ç¸ºè‚²E¹E§ãƒ»E¹é‚µEºãƒ»E®éœ‘ï½¥ãƒ»E¶éš²E·èµæ™¢E½å®šæ„¾é«¢Â€ãƒ»E¾ç¹ï½»
	if (mouseInput & MOUSE_INPUT_LEFT && AttackJumpCount >= AttackJumpTime&&!mbjump)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_JUMP);
		mbjump = true;
		AttackJumpCount = 0;
		mfjumpPower = JUMP_POWER;
		//éš°E¾ãƒ»E»éš°E¦ç¹ï½»Eçš®E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é‚µEºãƒ»E«é™æº½åˆ¤èŸ²E©
		mpModel->ChangeAnimation(ANIMATION_ATTACKJUMP);
		//éƒ¢æ™¢E½E«éƒ¢æ™¢E½E¼éƒ¢æ™å¹²ç¹ï½»é‚µEºéœˆæŠ«é›·é‚µEºãƒ»Eªé‚µEºç¹ï½»
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		//éƒ¢æ™¢E½E¢éƒ¢æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é™Ÿå‹ŸE¾å¾ŒãEé™Ÿè¼”ãEãƒ»E©è²E½©çš®E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é‚µEºãƒ»E«éš°é¯‰ï½½E»é‚µEºç¹ï½»
		
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
			mvPosition.y = hitPos.y;//Yé™Ÿè¶£E½E§éš¶ç˜ç”Ÿãƒ»ãƒ»é‚µEºãƒ»E«é‚µEºèœ‰ï½±çª¶E»é‚µEºé„™ï½«ãƒ»E¥
			
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
	int mouseInput = GetMouseInput(); // éƒ¢æ™E½§E­ç¸ºè‚²E¹E§ãƒ»E¹é‚µEºãƒ»E®éœ‘ï½¥ãƒ»E¶éš²E·èµæ™¢E½å®šæ„¾é«¢Â€ãƒ»E¾ç¹ï½»
	if (mouseInput & MOUSE_INPUT_LEFT && AttackSlideCount >= AttackSlideTime)
	{
		//mvOldPosition = mvPosition;
		if (mpTarget == nullptr)return;//éš°E¨ãƒ»Eµéƒ¢E§èœ»éºæ¥³é©•æ“¾E½E¥é‚µEºãƒ»E§é‚µEºé«¦Eªç«ŠéEE¸Eºãƒ»E£é‚µEºè²E‡E½è© etrurn
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		if (mpModel->GetIsSeparate())mpModel->mpSeparateAnimation->SetAnimationCount(1.2f);
		else mpModel->mpAnimation->SetAnimationCount(1.2f);
		AttackSlideCount = 0;
		GoPosition = (VSub(mpTarget->GetPosition(), mvPosition));
		TargetPosition = VScale(GoPosition, 0.5f);

		//éš°E¾ãƒ»E»éš°E¦ç¹ï½»Eçš®E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é‚µEºãƒ»E«é™æº½åˆ¤èŸ²E©
		mpModel->ChangeAnimation(ANIMATION_ATTACKSLIDE);
		//éƒ¢æ™¢E½E«éƒ¢æ™¢E½E¼éƒ¢æ™å¹²ç¹ï½»é‚µEºéœˆæŠ«é›·é‚µEºãƒ»Eªé‚µEºç¹ï½»
		mpModel->SetLoop(false);
		//éƒ¢æ™¢E½E¢éƒ¢æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é™Ÿå‹ŸE¾å¾ŒãEé™Ÿè¼”ãEãƒ»E©è²E½©çš®E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³é‚µEºãƒ»E«éš°é¯‰ï½½E»é‚µEºç¹ï½»
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);

	}
	

	
	if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide)
	{
		new Effect(VAdd(mvPosition, VGet(0.0f, 0.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 255, 30, 20), 50.0f, 0.03f);
		GoPosition = VNorm(GoPosition);
		mfTargetAngle = atan2f(GoPosition.x, GoPosition.z);


		////////////////éƒ¢E§ãƒ»E¹éƒ¢æ™¢E½E©éƒ¢E§ãƒ»E¤éƒ¢æ™‰åˆ¤è›»E¤éš°E¦ç¹ï½»ç¹ï½»é™Ÿè¶£E½E§éš¶ç˜è¡·ãƒ»E§ãƒ»E»é™·é˜ªãƒ»///////////////
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


	mfHp = mfHp < 0 ? 0 : mfHp; //HPé‚µEºç¹ï½»éš´å¹¢E½Eªé›‹ãEÂ€é‚µEºãƒ»E«é‚µEºãƒ»Eªé‚µEºãƒ»E£é‚µEºè²E‡E½ãƒ»éƒ¢E§é›‹æ§­ãƒ»éƒ¢E§è •é¯‰ï½½ãƒ»
	mfHp = mfHp > GetAllStatusState(Object3D::Status_Hp) ? GetAllStatusState(Object3D::Status_Hp) : mfHp; //HPé‚µEºè­´E§è«¤å‘µæ£”ãEE§é™‹æ»‚ï½½E¤éƒ¢E§è³å¤²E½E¶ç¹ï½»ç«ï½´é‚µEºãƒ»E¦é‚µEºç¹ï½»ç¬³ãƒ»E¹E§èŸ²E¨E‚Â€ç«ï½µè«¤å‘µæ£”ãEE§é™‹æ»‚ï½½E¤éƒ¢E§é›‹æ§­ãƒ»éƒ¢E§è •é¯‰ï½½ãƒ»
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

	DashGuage = DashGuage < 0 ? 0 : DashGuage; //HPé‚µEºç¹ï½»éš´å¹¢E½Eªé›‹ãEÂ€é‚µEºãƒ»E«é‚µEºãƒ»Eªé‚µEºãƒ»E£é‚µEºè²E‡E½ãƒ»éƒ¢E§é›‹æ§­ãƒ»éƒ¢E§è •é¯‰ï½½ãƒ»
	DashGuage = DashGuage > MaxDashGauge ? MaxDashGauge : DashGuage; //HPé‚µEºè­´E§è«¤å‘µæ£”ãEE§é™‹æ»‚ï½½E¤éƒ¢E§è³å¤²E½E¶ç¹ï½»ç«ï½´é‚µEºãƒ»E¦é‚µEºç¹ï½»ç¬³ãƒ»E¹E§èŸ²E¨E‚Â€ç«ï½µè«¤å‘µæ£”ãEE§é™‹æ»‚ï½½E¤éƒ¢E§é›‹æ§­ãƒ»éƒ¢E§è •é¯‰ï½½ãƒ»
	unsigned int Color3;
	unsigned int Color4;
	Color3 = GetColor(255, 255, 255);
	Color4 = GetColor(0, 0, 0);
	DrawBox(pos2.x, pos2.y, pos2.x + size2.x, pos2.y + size2.y, Color3, true);
	//DrawBox(pos2.x, pos2.y, pos2.x + size2.x, pos2.y + size2.y, GetColor(255, 255, 255), true);
	DrawBox(pos2.x + 1, pos2.y + 1,
		pos2.x + size2.x * DashRatio - 1,
		pos2.y + size2.y - 1,
		GetColor(255, 255, 0), true);

	//DrawFormatString(pos.x, pos.y, GetColor(255, 255, 255), "hp:%f", mnHp);



	mpLevelUp->Draw();
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
	//Jump();
	if (InputManager::CheckDownKey(KEY_INPUT_2))
	{
		// é—–ï½´éœˆè²»E½å®šï½«E¡ãƒ»E¾é‚µEºãƒ»E£é‚µEºãƒ»E¦éƒ¢E§ç¹§ãƒ»E½E¿ç¹ï½»ç¬˜ãEæ‘è®’ï½«ãƒ»E¾ãƒ»E©é«¦E®ãƒ»E¬ ---------------- //
		Item::ItemInformation* itemInfo;
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::HEAL;
		itemInfo->Name = "é™œç²E§E«ãƒ»E¾ãƒ»E©é«¦E®ãƒ»E¬";
		mpItemManager->AddItem(itemInfo);

		//Master::mpInfClassManager->LogList.push_back(new InfClass(400, itemInfo->Name.c_str(), 1));
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

		// é¨¾ãƒ»ãƒ»Eï½¦
		// éš²E¡ãƒ»E¾é‚µEºãƒ»E£é‚µEºè²E€¥Eéƒ¢E§ãƒ»E¤éƒ¢æ˜´ãƒ»Eå ¤E¸Eºèµæ™¢E½è¿‚temInformationéƒ¢E§é›‹æ§«å¾é™Ÿå‹Ÿå¹²ç¬˜ãEE¹E§ç¹ï½»
		// é‚µEºè­ï½´ãƒ»æªï½¹E§è­½å—¾emManageré‚µEºãƒ»E«é›‹ã‚‘E½E¡é‚µEºèœ‰ï½±çª¶E»é‚µEºç¹§ãƒ»E¿E¡éƒ¢E§ç¹ï½»

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
		// é—–ï½´éœˆè²»E½å®šï½«E¡ãƒ»E¾é‚µEºãƒ»E£é‚µEºãƒ»E¦éƒ¢E§ç¹§ãƒ»E½E¿ç¹ï½»ç¬˜ãEæ‘è®’ï½«ãƒ»E¾ãƒ»E©é«¦E®ãƒ»E¬ ---------------- //
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
			//float VECSizeSecound = VSize(SecoundEneDistance);
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
	//if (collider == 
	// 
	// 
	// 
	// 
	// && check->mpParentObject->GetTag() == Object3D::Tag3D_Player3D)
	//{
	//	mbStageOutFlag = false;
	//	if (check == this->mpCapsuleCollider)
	//	{
	//		mbStageOutFlag = true;//stageé™·Â€ç¹ï½»ç«Šé‹E¸Eºç¹ï½»ç«ŠéEE¸Eºèµæ˜¶å¤¢é‚µEºè²E‡E½èŠ½æ’»ãƒ»E´éš°ãƒ»Â€é©•å‡E½E»é™·å´ï½¼æŠ«ãƒ»éƒ¢E§èœ¿E¥è¾¯æ…•ï½¸Eºãƒ»E®éš°ãƒ»Â€é‚µEºãƒ»E«éš°é¯‰ï½½E»éƒ¢E§ç¹ï½»
	//	}

	//}

	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Obj)
	{
		
		//mvPosition = VAdd(mvOldPosition, VScale(moveVec, -mfSpeed*3));
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
				
				// éš°E¾ãƒ»E»éš°E¦ç¹ï½»Eçš®E¹æ™¢E½E¼éƒ¢E§ãƒ»E·éƒ¢æ™¢E½E§éƒ¢æ™¢E½E³&&Attack1
				if (now == ANIMATION_ATTACK && AttackState == Attack_Normal && !mbjump && !pEne->GetHitjudgment())
				{
					pEne->SetHitjudgment(true);
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));
					AttackHitJudgmentflag = true;
					// é¨¾åŒE½½E»é¬®E±ãƒ»E¢éš°E½ãƒ»Eºéƒ¢E§è •é¯‰ï½¼ãƒ»E¸EºèŸ¶å‘»E½ãƒ»
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

				// é¨¾åŒE½½E»é¬®E±ãƒ»E¢éš°E½ãƒ»Eºéƒ¢E§è •é¯‰ï½¼ãƒ»E¸EºèŸ¶å‘»E½ãƒ»
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
				// é¨¾åŒE½½E»é¬®E±ãƒ»E¢éš°E½ãƒ»Eºéƒ¢E§è •é¯‰ï½¼ãƒ»E¸EºèŸ¶å‘»E½ãƒ»
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

		//if (!mbAttackSlideSerch)break;
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
	if (!(now==ANIMATION_ATTACK)&& !(now == ANIMATION_ATTACKJUMP)&& !(now == ANIMATION_ATTACKSLIDE))//é™ŸåEä¾­ç¬³ãƒ»E¹E§é«®âˆµãƒ»é™ï½³è¢E¹ç¹ï½»é™ŸåŒ…E½E©é›ï½¢ãƒ»E»
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
	// é™ŸåEä¾­ç¬³ãƒ»E¹E§é«®âˆµãƒ»é™ï½³è¢E½èŸ²E©éš´ãƒ»E½E°
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
