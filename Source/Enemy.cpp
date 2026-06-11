#include"Enemy.h"
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
#include"Coin.h"


#include"Wall.h"
#include"Scene.h"
#include"Tree.h"
#include"ColliderManager.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"


Enemy::Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2, float Serch3, float xp,int money, bool isSeparateAnim)
	:Object3D(initPos)
	, mfHp(hp)
	, mfSpeed(speed)
	, mbInvisible(false)
	, mfAngle(0.0f)
	, mfTargetAngle(0.0f)
	, mfSize(HitSize)
	, HitSerch(Serch1)
	, HitAttackSerch(Serch2)
	, HitStopSerch(Serch3)
	, mfHaveXp(xp)
	, AlgHit(20)
	, mfAttack(attack)
	, NoPosition(VGet(0, 0, 0))
	, AttackHitJudgmentflag(false)
	, isHitAttackSearch(false)
	, isHitSerchStop(false)
	, isHitAttack(false)
	, isHitSearch(false)
	, HitJudgmentflag_Player(false)
	, WalkCount(0)
	, WalkTimer(0)
	, isDead(false)
	, mfMaxHp(hp)
	,mfHaveMoney(money)
	//,Animation(false)
{
	SetTag(Object3D::Tag3D_Enemy3D);
	//SpeedDownNomber =mnSpeed-
	//繝｢繝・Ν縺ｮ逕滓・
	mpModel = new Model(filename, initPos, isSeparateAnim);
	mpModel->SetScale(VGet(1.3f, 1.3f, 1.3f));
	VinitPos = initPos;
	//mpModel->AddAttachment("", "mixamorig:RightHandIndex1");
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;
	mpInventory = new Inventory(Object3D::Tag3D_Enemy3D);
	mpItemManager = mpInventory->mpItemManager;
	mpWeaponManager = mpInventory->mpWeaponManager;
	mpEquipmentManager = mpInventory->mpEquipmentManager;
	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize/2, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 30.0f);
	mpSerchCollider = new SphereCollider(this, mvPosition, HitSerch);//謨ｵ繧呈爾遏･縺吶ｋ蜀・
	mpAttackCollider = new SphereCollider(this, mvPosition, HitAttackSerch);//謨ｵ繧呈判謦・＠蟋九ａ繧句・
	mpStopCollider = new SphereCollider(this, mvPosition, HitStopSerch);//謨ｵ縺ｨ縺ｮ霍晞屬繧剃ｿ昴▽蜀・
	mpPurposCollider = new SphereCollider(this, mvPosition, 100.0f);
	//new DrawHp("",  //逕ｻ蜒丞錐
	//	(VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f))),//荳ｭ蠢・ｺｧ讓・
	//	VAdd(mvPosition, VGet(150.0f, 200.0f, 0.0f)),//蟾ｦ荳雁ｺｧ讓・
	//	VAdd(mvPosition, VGet(-150.0f, 0.0f, 0.0f)));//蜿ｳ荳句ｺｧ讓・
	  // 繧ｳ繝ｩ繧､繝繝ｼ逕滓・
	
	
}

Enemy::~Enemy()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
	
}

void Enemy::Update()
{
	
	if (mfHp<=0)
	{
		DeathEnemy();//謨ｵ繧貞偵＠縺溘→縺阪↓繧ｯ繝ｪ繧｢縺吶ｋ縺溘ａ縺ｮ繧ｫ繧ｦ繝ｳ繝医ｒ貂帙ｉ縺・
	}
	else
	{
		if (mpModel != nullptr)
		{
			Attack();
			CollPositionUpdate();
			RotationByMove();
			Move();
			/////蠑ｾm繧呈遠縺､蜃ｦ逅・
		
			mpModel->Update();
			//mpDH->Update();//drawHp縺ｮ繧｢繝・・繝・・繝医ｒ蜻ｼ縺ｶ
			
		}

	}
}

void Enemy::Draw()
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

void Enemy::AttackList()
{
	
	//Attack();

}

void Enemy::Attack()
{
	AnimationState now = mpModel->GetNowState();
	
	//f ()繝励Ξ繧､繝､繝ｼ縺ｫ霑代▼縺・◆譎ゅ→譖ｸ縺・
	if (AttackCount >= AttackInterval&& isHitAttackSearch)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		AttackCount = 0;
		//謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ螟画峩
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//繝ｫ繝ｼ繝励・縺輔○縺ｪ縺・
		mpModel->SetLoop(false);
		//繝｢繝ｼ繧ｷ繝ｧ繝ｳ蠕後・蠕・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ謌ｻ縺・
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		//mpModel->SetRotation(mvRotation);
		isHitAttackSearch = false;
	}
	if (!(now == ANIMATION_ATTACK))
	{
		AttackHitJudgmentflag = false;
		AttackCount++;
	}



}




void Enemy::Move()
{
	AnimationState now = mpModel->GetNowState();
	if (now == ANIMATION_ATTACK||now==ANIMATION_ATTACKMAGIC||now==ANIMATION_ATTACKJUMP)return;
	if (Master::SafePointOn)mvPosition = VinitPos;//繧ゅ＠繝励Ξ繧､繝､繝ｼ縺悟ｮ牙・蝨ｰ蟶ｯ縺ｫ縺・ｋ縺ｪ繧牙・譛溘・繧ｹ縺ｫ謌ｻ繧・
	

	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);

	 moveVec = VGet(0.0f, 0.0f, 0.0f);
	 UpMoveVector = VGet(0.0f, 0.0f, 0.0f);
	 leftMoveVector = VGet(0.0f, 0.0f, 0.0f);
	{
		//荳頑婿蜷代∈縺ｮ遘ｻ蜍輔・繧ｯ繝医Ν縺ｯ縲√き繝｡繝ｩ隕也せ譁ｹ蜷代°繧謁謌仙・繧呈栢縺・◆繧ゅ・縺ｨ縺吶ｋ
		UpMoveVector = VSub(Master::mpCamera->GetlookAtPosition(), Master::mpCamera->GetPosition());
		UpMoveVector.y = 0.0f;

		//蟾ｦ譁ｹ蜷代∈縺ｮ遘ｻ蜍輔・繧ｯ繝医Ν縺後∽ｸ頑婿蜷代・遘ｻ蜍輔・繧ｯ繝医Ν縺ｨ縲〆霆ｸ縺ｮ繝励Λ繧ｹ譁ｹ蜷代∈縺ｮ繝吶け繝医Ν縺ｫ蝙ら峩縺ｪ譁ｹ蜷托ｼ亥､也ｩ搾ｼ・
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));
		leftMoveVector.y = 0.0f;


		//遘ｻ蜍輔・繧ｯ繝医Ν縺ｯ遘ｻ蜍暮㍼繧貞刈蜻ｳ縺励↑縺・・縺ｧ縲∵ｭ｣隕丞喧縺励※縺翫￥(繝吶け繝医Ν縺ｮ髟ｷ縺輔ｒ・代↓縺吶ｋ縺薙→)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}


	//////////////遘ｻ蜍墓婿蜷代→繧ｹ繝斐・繝峨・蜃ｦ逅・///////////////
	{
		WalkCount++;
		if (isHitSerchStop)
		{
			mfSpeed = 0;///騾ｲ縺ｾ縺ｪ縺・ｈ縺・↓縺吶ｋ
		}
		else
		{
			mfSpeed = mfNormalSpeed;
		}
		if (WalkTimer < WalkCount)
		{
			WalkTimer = rand()%300;//方向転換タイマー
			mnAlgorithm = rand()%8; // 0-3: 歩く, 4-7: 立ち止まる
			WalkCount = 0;
		}

		if (mnAlgorithm == 0)//左方向への移動
		{
			moveVec = VAdd(moveVec, leftMoveVector);
		}
		else if (mnAlgorithm == 1)//右方向への移動
		{
			moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
		}
		else if (mnAlgorithm == 2)//奥方向への移動
		{
			moveVec = VAdd(moveVec, UpMoveVector);
		}
		else if (mnAlgorithm == 3)//手前方向への移動
		{
			moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));
		}
		else
		{
			// mnAlgorithmが 4〜7 の場合は何もしない（立ち止まる）
		}



		GoPosition = VSub(pPlayer->GetPosition(), mvPosition);
		GoPosition = VNorm(GoPosition);///繝励Ξ繧､繝､繝ｼ縺ｸ蜷代°縺・・逅・


		bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
		if (isMove || isHitSearch)
		{
			mpModel->ChangeAnimation(ANIMATION_RUN);

			//遘ｻ蜍墓婿蜷代ｒ豁｣隕丞喧縺励※縺翫￥
			//moveVec = VNorm(moveVec);
			//moveVec =VAdd( moveVec  ,mnSpeed);
			if (isHitSearch == false)
			{
				mfTargetAngle = atan2f(moveVec.x, moveVec.z);
			}
			if (isHitSearch)
			{
				mfTargetAngle = atan2f(GoPosition.x, GoPosition.z);
			}

		}
		else
		{
			mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
		}
		VECTOR oldPosition = mvPosition;//蜑榊屓縺ｮ蠎ｧ讓吶ｒ荳譌ｦ菫晄戟
		if (isHitSearch == false)
		{
			mvPosition = VAdd(mvPosition, VScale(moveVec, mfSpeed));
		}
		if (isHitSearch)
		{
			mvPosition = VAdd(mvPosition, VScale(GoPosition, mfSpeed));

		}
		//////////////////////////////////////////////////////

		VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
		//繧ｹ繝・・繧ｸ縺ｨ縺ｮ蠖薙◆繧雁愛螳壹ｒ縺吶ｋ
		hitPos = VGet(0.0f, 0.0f, 0.0f);
		bool isHit = false;


		auto obj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Stage);
		if (obj != nullptr)
		{
			Stage* pStage = dynamic_cast<Stage*>(obj);
			if (pStage != nullptr)
			{
				//繧ｹ繝・・繧ｸ縺ｨ繝励Ξ繧､繝､繝ｼ縺ｮ繧ｫ繝励そ繝ｫ縺悟ｽ薙◆縺｣縺ｦ縺・ｋ蝣ｴ蜷・
				if (pStage->CheckHit_Capsule(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
				{
					//蠖薙◆縺｣縺ｦ縺・ｋ縺ｧ縺ゅｍ縺・・繝ｪ繧ｴ繝ｳ縺ｨ縺ｮ謗･隗ｦ轤ｹ繧呈ｱゅａ繧・
					hitPos = pStage->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),//enemy縺ｮ閹晏ｽ薙◆繧・螟壼・)縺ｨ
						VAdd(mvPosition, VGet(0.0f, -40.0f, 0.0f))//enemy縺ｮ蟆代＠荳九≠縺溘ｊ繧堤ｷ壼・縺ｨ縺励※謖・ｮ・
					);

					//蠖薙◆縺｣縺溷愛螳壹ｒ蜿悶▲縺ｦ縺翫￥
					isHit = true;
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
			mvPosition.y += -4.0f;//關ｽ荳九☆繧矩溷ｺｦ
			if (mvPosition.y <= 0.0f)
			{
				mvPosition.y = oldPosition.y;
			}
		}
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



						// 螢√↓豐ｿ縺｣縺ｦ陦後￥繧医≧縺ｪ繝吶け繝医Ν繧貞叙蠕・
						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 螢∵ｲｿ縺・・繧ｯ繝医Ν
						float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 遘ｻ蜍墓婿蜷代・繧ｯ繝医Ν縺ｮ蜿榊ｯｾ繝吶け繝医Ν縺ｨ縲∝｣√・豕慕ｷ壹→縺ｮ蜀・ｩ阪ｒ豎ゅａ繧・
						slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 螢∵ｲｿ縺・・繧ｯ繝医Ν繧定ｨ育ｮ・

						// 荳譌ｦ蠎ｧ讓吶ｒ謌ｻ縺励※縺九ｉ螢√↓豐ｿ縺｣縺ｦ豁ｩ縺上ｈ縺・↓縺吶ｋ
						mvPosition = oldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
					}
				}
			}
		}




		mpModel->SetPosition(mvPosition);


	}
}

void Enemy::RotationByMove()
{
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
	//莉雁髄縺・※縺ｻ縺励＞隗貞ｺｦ繧堤肇蜃ｺ
	mfAngle = mfTargetAngle - subAngle;

	//蝗櫁ｻ｢蛟､繧定ｨｭ螳・
	mvRotation.y = mfAngle + DX_PI_F;
	//繝｢繝・Ν縺ｫ莨昴∴繧・
	mpModel->SetRotation(mvRotation);


}

void Enemy::Damage(float damage)
{

	mfHp -= damage;

}



void Enemy::DeathEnemy()
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	isDead = true;
	//Delete();
	//Master::TutorialCount++;
	mpModel->ChangeAnimation(ANIMATION_DYING);
	//繝ｫ繝ｼ繝励・縺輔○縺ｪ縺・
	mpModel->SetLoop(false);
	//繝｢繝ｼ繧ｷ繝ｧ繝ｳ蠕後・蠕・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ謌ｻ縺・
	mpModel->SetLoopFinishState(ANIMATION_MAX);
	//DeathColliderPosition();
	
	//Master::mpLevelUp->SetXp(Master::mpLevelUp->GetXp() + mfHaveXp);
	Delete();
	if (mpModel->IsAnimationLoopFinish())
	{
		mpInventory->DropInventory(mnChance, mbWeapon, mbItem);//繧ｭ繝｣繝ｩ繧ｯ繝ｩ繧ｹ縺ｮ繧ｹ繝医Λ繧ｯ繝医〒蛻晄悄蛹悶☆繧・
		if (player != nullptr) {
			player->mpHaveMoney->AddMoney(mfHaveMoney);
			player->mpLevelUp->AddXp(mfHaveXp);
		}
		
		if (Master::TutorialFlag) { Master::TutorialCount++; }
		
		
		SetDeleteFlag(true);
	}
	
	mpModel->Draw();
	mpModel->Update();
	
	
}

void Enemy::DeathColliderPosition()
{
	VECTOR pos = VGet(10000, 10000, 10000);
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->mvPosition = pos;
		mpCapsuleCollider->mvPosition2 = pos;
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->mvPosition = pos;
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->mvPosition = pos;
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->SetDeleteFlag(true);
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->mvPosition = pos;
	}
	
	
	

	mpStopCollider->mvPosition = pos;

}

void Enemy::OnEnter(Collider* collider, Collider* check)//蟾ｦ蛛ｴ.
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	if (mfHp <= 0)return;
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Obj)
	{
		
		mvPosition = VinitPos;
	}
	
	if (check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr)return;
	
		if (collider == mpSerchCollider &&player->GetCollisionCollider()==check)
		{
			isHitSearch = true;
		}
		if (collider == mpAttackCollider && player->GetCollisionCollider() == check)
		{
			isHitAttackSearch = true;//謾ｻ謦・幕蟋九ヵ繝ｩ繧ｰ

		}
		if (collider == mpStopCollider && player->GetCollisionCollider() == check)
		{
			isHitSerchStop = true;
		}
	}
	//蠖薙◆縺｣縺ｦ縺・ｋ髢捺凾縺ｮ蜃ｦ逅・
}

void Enemy::OnTrigger(Collider* collider, Collider* check)
{//蠖薙◆縺｣縺溽椪髢薙・蜃ｦ逅・
	if (mfHp <= 0)return; auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	AnimationState now = mpModel->GetNowState();
	if (collider == mpAttachCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)
	{//mpModel縺ｮ逡ｪ蜿ｷ繧ゅ≠縺｣縺ｦ縺・ｋ縲√￠縺ｩ縺薙・if譁・↓蜈･繧峨↑縺・
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr)return;
		if (check == player->GetCollisionCollider())
		{
			//if (pPlayer->GetInvisible()) { return; }
			if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
			{

				player->Damage(mfAttack);
				AttackHitJudgmentflag = true;//蠖薙◆縺｣縺溘ｈ繝ｼ
			}
		}

	}
	if (collider == mpPurposCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr) return;
		if (check == player->GetCollisionCollider())
		{
			mpPurposCollider->mvPosition = PurposePosition;

		}
	
	}
	
}

void Enemy::OnExit(Collider* collider, Collider* check)
{
	if (mfHp <= 0)return; auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	AnimationState now = mpModel->GetNowState();
	if (check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr)return;
		if (collider == mpAttachCollider && player->GetCollisionCollider() == check)
			{

			}
			if (collider == mpSerchCollider && player->GetCollisionCollider() == check)
			{
				isHitSearch = false;
			}
			if (collider == mpAttackCollider && player->GetCollisionCollider() == check)
			{
				isHitAttackSearch = false;//謾ｻ謦・幕蟋九ヵ繝ｩ繧ｰ

			}
			if (collider == mpStopCollider && player->GetCollisionCollider() == check)
			{
				isHitSerchStop = false;
			}
		
	}
}

void Enemy::CollPositionUpdate()
{
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->mvPosition = mvPosition;
		mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
		
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->mvPosition = mpModel->GetAttachmentPosition();
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->mvPosition = mvPosition;
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->mvPosition = VAdd(mvPosition, VGet(0.0f, mfSize / 2, 0.0f));
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->mvPosition = VAdd(mvPosition, VGet(0.0f, mfSize / 2, 0.0f));
	}
	
	


	
	
}

void Enemy::Delete()
{
	
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->SetDeleteFlag(true);
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->SetDeleteFlag(true);
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->SetDeleteFlag(true);
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->SetDeleteFlag(true);
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->SetDeleteFlag(true);
	}
	
}
