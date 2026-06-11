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

// 蛻・牡繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧剃ｽｿ逕ｨ縺吶ｋ縺九・險ｭ螳壹ｒ霑ｽ蜉�・医ョ繝輔か繝ｫ繝医・false・井ｽｿ逕ｨ縺励↑縺・ｼ会ｼ・
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool isSeparateAnim)
	:Object3D(initPos)
	, mfAttack(0)//繝繝｡繝ｼ繧ｸ
	, mfAttackjump(5)
	, mfAttackSlide(7)

	,OnJumpCollider(false)//縺薙・迸ｬ髢薙↓蠖薙◆繧雁愛螳壹ｒ縺､縺代ｋ
	, mbInvisible(false)//謾ｻ謦・′驥崎､・＠縺ｪ縺・ｈ縺・↓
	, mbjump(false)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(speed)//speed
	, JUMP_POWER(jumppower)
	, mfHp(hp)//hp
	, mfMaxHp(hp)//maxhp
	, mfDashSpeed(speed * 2)
	, NewShieldFast(false)//繧ｷ繝ｼ繝ｫ繝峨ｒ逕滓・縺吶ｋ縺九←縺・°
	, mfSize(60.0f)//縺ゅ◆繧雁愛螳・
	, Pause(false)//繝昴・繧ｺ逕ｻ髱｢
	, ChangeCamera(0)//隕也せ螟画峩
	, DashGuage(100.0f)//
	, EneSerchCount(0)
	, Gpush(false)//蠢・ｮｺ謚繧偵＠縺ｦ縺・＞縺・
	, GpushCount(2)//蠢・ｮｺ謚縺ｮ繧ｫ繧ｦ繝ｳ繝・
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
	{//蛻晄悄陬・ｙ
		Weapon::WeaponDate* weaponInf;
		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 1000;
		weaponInf->filename = "Resource/3D/Sabel.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon1;
		weaponInf->name = "weapon1";
		mpWeaponManager->AddWeapon(weaponInf);
		mpWeaponManager->ChangeWeapon(weaponInf);

		mfNormalAttack = weaponInf->damage;//譛蛻昴↓蛻晄悄陬・ｙ縺ｮ謾ｻ謦・鴨縺ｮ蛻晄悄蛹門・逅・ｒ陦後≧
		mfAttack = weaponInf->damage;//譛蛻昴↓蛻晄悄陬・ｙ縺ｮ謾ｻ謦・鴨縺ｮ蛻晄悄蛹門・逅・ｒ陦後≧
	}




	MaxDashGauge = DashGuage;

	//mpTarget = new Enemy3D("", VGet(20000, 0, 0), 0, 0, 0, 0, 0);
	
	//mpCamera->ResetCameraPlayer(true);
	 // 繧ｳ繝ｩ繧､繝繝ｼ逕滓・
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
	mpInventory->Update();//inventory縺�縺代・縺薙％縺ｫ
	if (Master::ShopClassOn || Master::InventoryClasOn)return;
	///////////////////////////////////////////////
	

	if (InputManager::CheckDownKey(KEY_INPUT_O)&&!Master::NearShopOn)
	{
		Master::InventoryClasOn = true;
		Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//蜉ｹ譫憺浹
	}
		
	
	if (Master::PauseOn == false)//繝昴・繧ｺ荳ｭ縺ｯ蜍輔°縺ｪ縺・ｈ縺・↓縺吶ｋ
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
		////////////////////////陦悟虚/////////////////
		SelectAttack();
		Result();
		ResetNUETRAL();
		//繧ｸ繝｣繝ｳ繝怜・逅・
		CollPositionUpdate();
		Evasion();
		//謾ｻ謦・・逅・
		SelectAttack();
		//遘ｻ蜍募・逅・
		MoveEx();
		//蝗櫁ｻ｢蜃ｦ逅・
		RotationByMove();
		EnemySerch();

		//Interval++;
		//if (Interval >= 10)//驫・ｒ謦・▽繧､繝ｳ繧ｿ繝ｼ繝舌Ν
		//{
		//	Shot();
		//	Interval = 0;
		//}
		if (mpLevelUp->GetLevelUp())
		{ 
			Master::mpSoundManager->PlaySE(SoundManager::SE_LEVELUP);
			mfHp = GetAllStatusState(Object3D::Status_Hp);//hp繧貞・蝗槭＆縺帙ｋ
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


		FirstNearVec = 0;//謾ｻ謦・＠縺ｦ縺ｪ縺・→縺阪↓謨ｵ縺ｨ縺ｮ霍晞屬繧呈ｸｬ繧九き繧ｦ繝ｳ繝医ｒ繝ｪ繧ｻ繝・ヨ
		mpTarget = nullptr;//逶ｮ讓吶・繧ｿ繝ｼ繧ｲ繝・ヨ繧偵Μ繧ｻ繝・ヨ

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
	if (Master::ShopClassOn || Master::InventoryClasOn)return;
	if (Master::PauseOn == false)///繝昴・繧ｺ逕ｻ髱｢縺ｧ縺ｯ縺ｪ縺・→縺・
	{
		
		bar();
		
	}
	////////////////////////荳我ｺｺ遘ｰ隕也せ縺ｮ譎ゅ・縺ｿ繝励Ξ繧､繝､繝ｼ縺瑚ｦ九∴繧九ｈ縺・↓縺吶ｋ///////////////
	if (Master::mpCamera->GetCamera3() == true)
	{
		//繝｢繝・Ν縺ｮ謠冗判
		mpModel->Draw();
	}

	if (Master::mpDebug->Getdebug() == true)
	{
		//縺ｧ繝舌ャ繧ｯ陦ｨ遉ｺ譁ｹ豕・
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
		DrawFormatString(100, 300, GetColor(255, 255, 255), "謾ｻ謦・鴨:%f", GetAllStatusState(Object3D::Status_Attack));
		DrawFormatString(100, 400, GetColor(255, 255, 255), "髦ｲ蠕｡蜉・%f", mpEquipmentManager->GetDamage());
		DrawFormatString(100, 500, GetColor(255, 255, 255), "s繧ｹ繝斐・繝・%f", GetAllStatusState(Object3D::Status_Speed));
		DrawFormatString(100, 350, GetColor(255, 255, 255), "邨碁ｨ灘､:%d", mpLevelUp->GetXp());
		DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f       Z:%f", mvPosition.x, mvPosition.y, mvPosition.z);
	}
	//豁ｦ蝎ｨ繧貞桁繧繧医≧縺ｪ逅・ｽ・
	DrawSphere3D(
		mpModel->GetAttachmentPosition(),
		30.0f,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

//遘ｻ蜍募・逅・せ繝・・繧ｸ縺ｨ縺ｮ蠖薙◆繧雁愛螳・
void Player3D::MoveEx()
{
	AnimationState state = mpModel->GetNowState();
	if (state==ANIMATION_ATTACKJUMP||state == ANIMATION_ATTACK || state == ANIMATION_JUMP_OUT || state == ANIMATION_SLIDE||state==ANIMATION_ATTACKSLIDE||Master::InventoryClasOn||Master::ShopClassOn)
	{
		return;//迚ｹ螳壹・繝｢繝ｼ繧ｷ繝ｧ繝ｳ荳ｭ縺ｧ縺ゅｌ縺ｰ菴輔ｂ縺励↑縺・
	}

	moveVec = VGet(0.0f, 0.0f, 0.0f);//遘ｻ蜍墓婿蜷・
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);//繧ｫ繝｡繝ｩ縺ｮ荳頑婿蜷代・繧ｯ繝医Ν
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);//繧ｫ繝｡繝ｩ縺ｮ蟾ｦ譁ｹ蜷代・繧ｯ繝医Ν
	//VECTOR DownMoveVector = VGet(0.0f, 0.0f, 0.0f);//繧ｫ繝｡繝ｩ縺ｮ荳区婿蜷代・繧ｯ繝医Ν
	//VECTOR RightMoveVector = VGet(0.0f, 0.0f, 0.0f);//繧ｫ繝｡繝ｩ縺ｮ蜿ｳ譁ｹ蜷代・繧ｯ繝医Ν


	//繧ｫ繝｡繝ｩ縺ｮ蜷代″縺九ｉ遘ｻ蜍輔・繧ｯ繝医Ν繧呈ｱゅａ繧・
	{
		//荳頑婿蜷代∈縺ｮ遘ｻ蜍輔・繧ｯ繝医Ν縺ｯ縲√き繝｡繝ｩ隕也せ譁ｹ蜷代°繧謁謌仙・繧呈栢縺・◆繧ゅ・縺ｨ縺吶ｋ
		UpMoveVector = VSub(Master::mpCamera->GetlookAtPosition(), Master::mpCamera->GetPosition());
		//UpMoveVector.y = 0.0f;

		//蟾ｦ譁ｹ蜷代∈縺ｮ遘ｻ蜍輔・繧ｯ繝医Ν縺後∽ｸ頑婿蜷代・遘ｻ蜍輔・繧ｯ繝医Ν縺ｨ縲〆霆ｸ縺ｮ繝励Λ繧ｹ譁ｹ蜷代∈縺ｮ繝吶け繝医Ν縺ｫ蝙ら峩縺ｪ譁ｹ蜷托ｼ亥､也ｩ搾ｼ・
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));
		//leftMoveVector.y = 0.0f;


		//遘ｻ蜍輔・繧ｯ繝医Ν縺ｯ遘ｻ蜍暮㍼繧貞刈蜻ｳ縺励↑縺・・縺ｧ縲∵ｭ｣隕丞喧縺励※縺翫￥(繝吶け繝医Ν縺ｮ髟ｷ縺輔ｒ・代↓縺吶ｋ縺薙→)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	//VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);//遘ｻ蜍墓婿蜷・

	if (CheckHitKey(KEY_INPUT_A))//蟾ｦ譁ｹ蜷代∈縺ｮ遘ｻ騾・
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_D))//蜿ｳ譁ｹ蜷代∈縺ｮ遘ｻ騾・
	{
		//veVec.x = 1.0f;
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
		//moveVec = VSub(moveVec, leftMoveVector);

	}
	if (CheckHitKey(KEY_INPUT_W))//螂･譁ｹ蜷代∈縺ｮ遘ｻ騾・
	{
		//veVec.z = 1.0;
		moveVec = VAdd(moveVec, UpMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_S))//謇句燕譁ｹ蜷代∈縺ｮ遘ｻ騾・
	{
		//moveVec.z = -1.0;
		moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));
		//moveVec = VSub(moveVec, UpMoveVector);

	}
	//遘ｻ蜍輔＠縺ｦ縺・ｋ迥ｶ諷九〒縺ゅｌ縺ｰ
	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{
		//mpModel->ChangeAnimation(ModelAnimation::ANIMATION_RUN);

		//遘ｻ蜍墓婿蜷代ｒ豁｣隕丞喧縺励※縺翫￥
		moveVec = VNorm(moveVec);

		//譁ｰ縺励＞蝗櫁ｻ｢繧偵そ繝・ヨ
		mfTargetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;//縺薙％縺ｧ莉翫く繝｣繝ｩ繧ｯ繧ｿ窶輔′蜷代＞縺ｦ縺・ｋ譁ｹ蜷代ｒ蜃ｺ縺吶ゆｿ晏ｭ・
	}
	//繧ｸ繝｣繝ｳ繝鈴幕蟋九・繧｢繝九Γ縺倥ｃ縺ｪ縺・・縺ｧ縺ゅｌ縺ｰ
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



	//遘ｻ蜍輔＆縺帙ｋ
	mvOldPosition = mvPosition;//蜑榊屓縺ｮ蠎ｧ讓吶ｒ荳譌ｦ菫晄戟


	mvPosition = VAdd(mvPosition, VScale(moveVec, GetAllStatusState(Object3D::Status_Speed)));
	////////////////////////////////////////////////////////



		/////繧ｹ繝・・繧ｸ縺ｨ縺ｮ蠖薙◆繧雁愛螳壹ｒ縺吶ｋ////////////
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
				//繧ｹ繝・・繧ｸ縺ｨ繝励Ξ繧､繝､繝ｼ縺ｮ繧ｫ繝励そ繝ｫ縺悟ｽ薙◆縺｣縺ｦ縺・ｋ蝣ｴ蜷・
				if (pStage->CheckHit_Capsule(VAdd(mvPosition, VGet(0.0f, -150.0f, 0.0f)), VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
				{
					//蠖薙◆縺｣縺ｦ縺・ｋ縺ｧ縺ゅｍ縺・・繝ｪ繧ｴ繝ｳ縺ｨ縺ｮ謗･隗ｦ轤ｹ繧呈ｱゅａ繧・
					hitPos = pStage->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, 1000.0f, 0.0f)),//繝励Ξ繧､繝､繝ｼ縺ｮ閹晏ｽ薙◆繧・螟壼・)縺ｨ
						VAdd(mvPosition, VGet(0.0f, -1000.0f, 0.0f))//繝励Ξ繧､繝､繝ｼ縺ｮ蟆代＠荳九≠縺溘ｊ繧堤ｷ壼・縺ｨ縺励※謖・ｮ・
					);

					//蠖薙◆縺｣縺溷愛螳壹ｒ蜿悶▲縺ｦ縺翫￥
					isHit = true;
				}
			}
		}
	}

	if (isHit)
	{
		//蝨ｰ髱｢縺ｫ豐ｿ縺｣縺ｦ豁ｩ縺・※縺・ｋ迥ｶ諷九→縺励※縲〆蠎ｧ讓吶ｒ繧ｹ繝・・繧ｸ縺ｫ蜷医ｏ縺帙ｋ
		mvPosition.y = hitPos.y;
	}
	if (isHit == false)
	{
		//隕∵隼蝟・
		mvPosition.y += -8.0f;//關ｽ荳九☆繧矩溷ｺｦ
		if (mvPosition.y <= 0.0f || mvPosition.y <= hitPos.y)
		{
			mvPosition.y = hitPos.y;
		}
	}

	/////////////////////////螢√・蠖薙◆繧雁愛螳・///////////////////////////
	bool hitwall = false;
	bool hitwalls = false;
	auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
	if (!walls.empty())
	{
		// 蠖薙◆縺｣縺ｦ縺・ｋ螢√ｒ隱ｿ縺ｹ繧・
		// hint: 迴ｾ迥ｶ縺ｮ蜃ｦ逅・〒縺ｯ1譫壹・螢√＠縺区怙邨ら噪縺ｫ蛻､螳壹＆繧後↑縺・・縺ｧ縲・譫夲ｼ井ｻ･荳奇ｼ峨↓蠖薙◆縺｣縺ｦ縺・◆蝣ｴ蜷医・蜃ｦ逅・ｒ閠・∴繧・
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				// 繝励Ξ繧､繝､繝ｼ繧貞桁繧繧医≧縺ｪ繧ｫ繝励そ繝ｫ蝙九・蛻､螳壹→縲∝｣√・荳芽ｧ貞ｽ｢繝昴Μ繧ｴ繝ｳ縺ｨ縺ｮ蠖薙◆繧雁愛螳壹ｒ陦後≧
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
					DrawFormatString(20, 820, GetColor(255, 0, 0), "エリアの端っこです！");
					DrawFormatString(20, 820, GetColor(255, 0, 0), "End of Area!");
					// 螢√↓豐ｿ縺｣縺ｦ陦後￥繧医≧縺ｪ繝吶け繝医Ν繧貞叙蠕・
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 螢∵ｲｿ縺・・繧ｯ繝医Ν
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 遘ｻ蜍墓婿蜷代・繧ｯ繝医Ν縺ｮ蜿榊ｯｾ繝吶け繝医Ν縺ｨ縲∝｣√・豕慕ｷ壹→縺ｮ蜀・ｩ阪ｒ豎ゅａ繧・
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 螢∵ｲｿ縺・・繧ｯ繝医Ν繧定ｨ育ｮ・

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
	if (damage - mpEquipmentManager->GetDamage() <= 0) { mfHp -= 1; return; }//譛菴弱〒繧・貂帙ｉ縺・
	if (now == ANIMATION_SLIDE)return;
	mfHp -= (damage-mpEquipmentManager->GetDamage());//陬・ｙ縺励※縺・ｋ繝繝｡繝ｼ繧ｸ蛻・ム繝｡繝ｼ繧ｸ繧呈ｸ帙ｉ縺・

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
	}
	if (mpModel->GetNowState() == ANIMATION_SLIDE)
	{
		mvPosition = VAdd(mvPosition, VScale(oldmoveVec, mfEvasionSpeed));
		mpModel->SetPosition(mvPosition);
	}






}


void Player3D::RotationByMove()
{

	//////莉雁髄縺・※縺ｻ縺励＞隗貞ｺｦ繧堤肇蜃ｺ
	//Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag_3D_Camera)
	//迴ｾ蝨ｨ縺ｮ蝗櫁ｻ｢蛟､縺九ｉ逶ｮ讓吶・蝗櫁ｻ｢蛟､縺ｮ蟾ｮ蛻・ｒ豎ゅａ繧・
	float subAngle = mfTargetAngle - mfAngle;



	//縺ゅｋ譁ｹ蜷代°繧峨≠繧区婿蜷代・蟾ｮ縺鯉ｼ托ｼ假ｼ仙ｺｦ莉･荳奇ｼ井ｻ･荳具ｼ峨↓縺ｪ繧九％縺ｨ縺後↑縺・・縺壹↑縺ｮ縺ｧ
	//蟾ｮ縺ｮ蛟､縺鯉ｼ托ｼ假ｼ蝉ｻ･荳奇ｼ井ｻ･荳具ｼ峨↓縺ｪ縺｣縺ｦ縺・◆繧臥浣豁｣縺吶ｋ
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//隗貞ｺｦ縺ｮ蟾ｮ蛻・ｒ蠕舌・↓・舌↓霑代▼縺代ｋ
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

	//莉雁髄縺・※縺ｻ縺励＞隗貞ｺｦ繧堤肇蜃ｺ
	mfAngle = mfTargetAngle - subAngle;

	//DrawFormatString(400, 600, GetColor(0, 0, 0), "mfAngle::%f", subAngle);

	//蝗櫁ｻ｢蛟､繧定ｨｭ螳・
	mvRotation.y = mfAngle + DX_PI_F;
	//繝｢繝・Ν縺ｫ莨昴∴繧・
	mpModel->SetRotation(mvRotation);
	//mfAngle = mfTargetAngle - Master::mpCamera->GetLookCamera();

	////蝗櫁ｻ｢蛟､繧定ｨｭ螳・
	//mvRotation.y = mpCamera->GetlookAtPosition().y; //mfAngle + DX_PI_F;
	////繝｢繝・Ν縺ｫ莨昴∴繧・
	//mpModel->SetRotation(mvRotation);


}




void Player3D::Jump()
{

	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		mvPosition.y += 300.0f;
		mbjump = true;
		mfjumpPower = JUMP_POWER;

		//繧ｸ繝｣繝ｳ繝鈴幕蟋九い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ縺吶ｋ
		
	}

	

}

void Player3D::Result()
{


}

void Player3D::Attack()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // 繝槭え繧ｹ縺ｮ迥ｶ諷九ｒ蜿門ｾ・
	if (mouseInput & MOUSE_INPUT_LEFT && AttackCount >= AttackTime&&now!=ANIMATION_ATTACK)
	{
		AttackCount = 0;
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACK);
		//謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ螟画峩
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//繝ｫ繝ｼ繝励・縺輔○縺ｪ縺・
		mpModel->SetLoop(false);
		//繝｢繝ｼ繧ｷ繝ｧ繝ｳ蠕後・蠕・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ謌ｻ縺・
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
	int mouseInput = GetMouseInput(); // 繝槭え繧ｹ縺ｮ迥ｶ諷九ｒ蜿門ｾ・
	if (mouseInput & MOUSE_INPUT_LEFT && AttackJumpCount >= AttackJumpTime&&!mbjump)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_JUMP);
		mbjump = true;
		AttackJumpCount = 0;
		mfjumpPower = JUMP_POWER;
		//謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ螟画峩
		mpModel->ChangeAnimation(ANIMATION_ATTACKJUMP);
		//繝ｫ繝ｼ繝励・縺輔○縺ｪ縺・
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		//繝｢繝ｼ繧ｷ繝ｧ繝ｳ蠕後・蠕・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ謌ｻ縺・
		
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
			mvPosition.y = hitPos.y;//Y蠎ｧ讓吶ｒ0縺ｫ縺励※縺翫￥
			
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
	int mouseInput = GetMouseInput(); // 繝槭え繧ｹ縺ｮ迥ｶ諷九ｒ蜿門ｾ・
	if (mouseInput & MOUSE_INPUT_LEFT && AttackSlideCount >= AttackSlideTime)
	{
		//mvOldPosition = mvPosition;
		if (mpTarget == nullptr)return;//謨ｵ繧呈─遏･縺ｧ縺阪↑縺｣縺溘ｉretrurn
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		if (mpModel->GetIsSeparate())mpModel->mpSeparateAnimation->SetAnimationCount(1.2f);
		else mpModel->mpAnimation->SetAnimationCount(1.2f);
		AttackSlideCount = 0;
		GoPosition = (VSub(mpTarget->GetPosition(), mvPosition));
		TargetPosition = VScale(GoPosition, 0.5f);

		//謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ螟画峩
		mpModel->ChangeAnimation(ANIMATION_ATTACKSLIDE);
		//繝ｫ繝ｼ繝励・縺輔○縺ｪ縺・
		mpModel->SetLoop(false);
		//繝｢繝ｼ繧ｷ繝ｧ繝ｳ蠕後・蠕・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ謌ｻ縺・
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);

	}
	

	
	if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide)
	{
		new Effect(VAdd(mvPosition, VGet(0.0f, 0.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 255, 30, 20), 50.0f, 0.03f);
		GoPosition = VNorm(GoPosition);
		mfTargetAngle = atan2f(GoPosition.x, GoPosition.z);


		////////////////繧ｹ繝ｩ繧､繝画判謦・・蠎ｧ讓咏ｧｻ蜍・///////////////
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


	mfHp = mfHp < 0 ? 0 : mfHp; //HP縺・譛ｪ貅縺ｫ縺ｪ縺｣縺溘ｉ0繧貞・繧後ｋ
	mfHp = mfHp > GetAllStatusState(Object3D::Status_Hp) ? GetAllStatusState(Object3D::Status_Hp) : mfHp; //HP縺梧怙螟ｧ蛟､繧定ｶ・∴縺ｦ縺・◆繧峨∵怙螟ｧ蛟､繧貞・繧後ｋ
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

	DashGuage = DashGuage < 0 ? 0 : DashGuage; //HP縺・譛ｪ貅縺ｫ縺ｪ縺｣縺溘ｉ0繧貞・繧後ｋ
	DashGuage = DashGuage > MaxDashGauge ? MaxDashGauge : DashGuage; //HP縺梧怙螟ｧ蛟､繧定ｶ・∴縺ｦ縺・◆繧峨∵怙螟ｧ蛟､繧貞・繧後ｋ
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
		// 菴輔ｒ諡ｾ縺｣縺ｦ繧ょｿ・★蝗槫ｾｩ阮ｬ ---------------- //
		Item::ItemInformation* itemInfo;
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::HEAL;
		itemInfo->Name = "蝗槫ｾｩ阮ｬ";
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

		// 逅・Φ
		// 諡ｾ縺｣縺溘い繧､繝・Β縺九ｉItemInformation繧貞叙蠕励☆繧・
		// 縺昴ｌ繧棚temManager縺ｫ貂｡縺励※縺ゅ￡繧・

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
		// 菴輔ｒ諡ｾ縺｣縺ｦ繧ょｿ・★蝗槫ｾｩ阮ｬ ---------------- //
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
	//		mbStageOutFlag = true;//stage蜀・↓縺・↑縺九▲縺溘ｉ蝣ｴ謇遘ｻ蜍輔☆繧句燕縺ｮ謇縺ｫ謌ｻ繧・
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
				
				// 謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ&&Attack1
				if (now == ANIMATION_ATTACK && AttackState == Attack_Normal && !mbjump && !pEne->GetHitjudgment())
				{
					pEne->SetHitjudgment(true);
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));
					AttackHitJudgmentflag = true;
					// 逕ｻ髱｢謠ｺ繧後＆縺帙ｋ
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

				// 逕ｻ髱｢謠ｺ繧後＆縺帙ｋ
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
				// 逕ｻ髱｢謠ｺ繧後＆縺帙ｋ
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
	if (!(now==ANIMATION_ATTACK)&& !(now == ANIMATION_ATTACKJUMP)&& !(now == ANIMATION_ATTACKSLIDE))//蠖薙◆繧雁愛螳壹・蠕ｩ豢ｻ
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
	// 蠖薙◆繧雁愛螳壽峩譁ｰ
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
		return mpWeaponManager->GetDamage() + mpBuffManager->GetBuff(state)+mpLevelUp->GetAttack();
	}
	if (state == Status_Speed)
	{
		return mfSpeed + mpBuffManager->GetBuff(state)+mpLevelUp->GetSpeed();
	}
	if (state == Status_Hp)
	{
		return mfMaxHp + mpLevelUp->GetHp();
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
