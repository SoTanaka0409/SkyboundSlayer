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
	//驛｢譎｢・�E�・�E�驛｢譏ｴ繝ｻ・取刮・�E�・�E�繝ｻ・�E�鬨�E�蠁E���E��E�阮吶・
	mpModel = new Model(filename, initPos, isSeparateAnim);
	mpModel->SetScale(VGet(1.3f, 1.3f, 1.3f));
	VinitPos = initPos;
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;

	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize/2, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 50.0f);
	mpSerchCollider = new SphereCollider(this, mvPosition, HitSerch);//髫�E�・�E�繝ｻ・�E�驛｢・�E�陷�E�閧�E�邊滁E��墓得�E�E�・�E�驍ｵ・�E�陷�E�・�E�繝ｻ邁E��咏ｹ晢�E��E�
	mpAttackCollider = new SphereCollider(this, mvPosition, HitAttackSerch);//髫�E�・�E�繝ｻ・�E�驛｢・�E�陷�E�莠�E�諢幁E��・�E�郢晢�E��E�繝ｻ・�E�髯晏�E�・�E�譎｢・�E�竏ｫ・�E�・�E�陷�E�・�E�郢晢�E��E�
	mpStopCollider = new SphereCollider(this, mvPosition, HitStopSerch);//髫�E�・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�鬮�E�閧�E�霎ｨ陞ｻ・�E�驛｢・�E�陷代・・�E�・�E�隴擾�E��E�隨・�E��E�髯�E��E�郢晢�E��E�
	//new DrawHp("",  //鬨�E�蛹・�E��E�・�E�髯�E�蜑�E�E��E�讓｣蛟ｹ
	//	(VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f))),//髣匁E���E�繝ｻ・�E�髯滂ｽ�E�郢晢�E��E�繝ｻ・�E�繝ｻ・�E�髫�E�阮吶・
	  // 驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�驛｢譎会ｽ�E�譎｢・�E�・�E�鬨�E�蠁E���E��E�阮吶・
	
	
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
	
		
	
	
		if (mpModel != nullptr)
		{
			DeathEnemy();//髫�E�・�E�繝ｻ・�E�驛｢・�E�髮区�E��E�繝ｻ繝ｻ・�E�驍ｵ・�E�雋�E�E�繝ｻ驍ｵ・�E�鬮�E�・�E�遶企豪・�E�・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�驍ｵ・�E�陷�E�・�E�繝ｻ迢暦�E��E�・�E�雋�E∞�E�E�竏ｫ・�E�・�E�繝ｻ・�E�驛｢・�E�繝ｻ・�E�驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譎�E樟繝ｻ螳夲�E��E�繧・�E��E�蜻�E�・�E�閾�E�・�E�・�E�郢晢�E��E�
			Attack();
			CollPositionUpdate();
			RotationByMove();
			Move();
			/////髯滓汚�E�E�・�E�m驛｢・�E�陷�E�逎ｯ笁E��驍ｵ・�E�繝ｻ・�E�髯�E�繝ｻ・�E�・�E�鬨�E�繝ｻ繝ｻ
		
			mpModel->Update();
			//mpDH->Update();//drawHp驍ｵ・�E�繝ｻ・�E�驛｢・�E�繝ｻ・�E�驛｢譏ｴ繝ｻ郢晢�E��E�驛｢譏ｴ繝ｻ郢晢�E��E�驛｢譎�E樟繝ｻ螳壽Τ繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�
			
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
	

}

void Enemy::Attack()
{
	AnimationState now = mpModel->GetNowState();
	
	if (AttackCount >= AttackInterval&& isHitAttackSearch)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		AttackCount = 0;
		//髫�E�・�E�繝ｻ・�E�髫�E�・�E�郢晢�E��E�・守坩・�E�譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驍ｵ・�E�繝ｻ・�E�髯樊ｺ�E�蛻�E�陝ｲ・�E�
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驛｢譎丞ｹ�E�郢晢�E��E�驍ｵ・�E�髴域喚髮�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�
		mpModel->SetLoop(false);
		//驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�髯溷供�E�E�蠕後�E髯溯�E�斐�E繝ｻ・�E�雋�E�E��E�逧�E�・�E�譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驍ｵ・�E�繝ｻ・�E�髫�E�魁E��ｽ�E�・�E�驍ｵ・�E�郢晢�E��E�
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
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
	if (Master::SafePointOn)mvPosition = VinitPos;//驛｢・�E�郢�E�繝ｻ・�E�・�E�驛｢譎丞ｹ�E�・取ｨ抵�E��E�・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驍ｵ・�E�隰疲�E�ゑｽ�E�・�E�霑壼生繝ｻ髯懶�E��E�繝ｻ・�E�髯晢�E��E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�繝ｻ迢暦�E��E�・�E�繝ｻ・�E�驛｢・�E�霑壼生繝ｻ髫�E�蟶�E�・�E�蛟･繝ｻ驛｢・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�髫�E�魁E��ｽ�E�・�E�驛｢・�E�郢晢�E��E�
	

	auto mpPlayer = Master::mpPlayer;
	Player3D* pPlayer = Master::mpPlayer;

	moveVec = VGet(0.0f, 0.0f, 0.0f);
	
	{
		if (isHitSerchStop) { mfSpeed = 0; } else { mfSpeed = mfNormalSpeed; }
		
		GoPosition = VSub(pPlayer->GetPosition(), mvPosition);
		GoPosition.y = 0.0f; // Fix Y axis movement
		if (VSquareSize(GoPosition) > 0.0001f) GoPosition = VNorm(GoPosition);
		moveVec = GoPosition;

		bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
		if (isMove) { mpModel->ChangeAnimation(ANIMATION_RUN); mfTargetAngle = atan2f(moveVec.x, moveVec.z); }
		else { mpModel->ChangeAnimation(ANIMATION_NEUTRAL); }

		VECTOR oldPosition = mvPosition;
		mvPosition = VAdd(mvPosition, VScale(moveVec, mfSpeed));
		//////////////////////////////////////////////////////

		VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
		//驛｢・�E�繝ｻ・�E�驛｢譏ｴ繝ｻ郢晢�E��E�驛｢・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�髯溷・萓ｭ隨�E�繝ｻ・�E�・�E�鬮�E�竏ｵ繝ｻ髯橸�E��E�陞｢・�E�繝ｻ蝣�E�・�E�・�E�陷�E�・�E�繝ｻ繝ｻ
		hitPos = VGet(0.0f, 0.0f, 0.0f);
		bool isHit = false;
		// �n�`�i�X�e�[�W�j�ɉ�������
		TerrainFollow(0.0f, 150.0f, 40.0f, 150.0f, -40.0f, 4.0f);
		auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
		if (!walls.empty())
		{
			// 髯溷・萓ｭ隨�E�繝ｻ・�E�・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�繝ｻ邁E���E��E�遶丞､�E�・�E�蟶晏擅繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驛｢・�E�郢晢�E��E�
			// hint: 髴托ｽ�E�繝ｻ・�E�髴托ｽ�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�髯�E�繝ｻ・�E�・�E�鬨�E�繝ｻ繝ｻ邵�E�蝣�E�・�E�・�E�繝ｻ・�E�1髫�E�・�E�陞｢・�E�郢晢�E��E�髯橸�E��E�遶丞､�E�・�E�・�E�驍ｵ・�E�陋ｹ・�E�隲�E�蜥弱♀郢�E�迚咎�E��E�驍ｵ・�E�繝ｻ・�E�髯具�E��E�繝ｻ・�E�髯橸�E��E�陞｢・�E�繝ｻ繝ｻ・�E�・�E�陟募�E��E�繝ｻ驍ｵ・�E�郢晢�E��E�郢晢�E��E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�髫�E�・�E�陞滂ｽ�E�繝ｻ・�E�闔蛹・�E��E�・�E�繝ｻ・�E�髣匁E���E�陞ゅ・・�E�・�E�陝ｲ・�E�遶頑･�E�・�E�蜀�E�萓ｭ隨�E�繝ｻ・�E�・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�隨�E�繝ｻ謦�E�繝ｻ・�E�髯�E�・�E�陋ｹ・�E�郢晢�E��E�髯�E�繝ｻ・�E�・�E�鬨�E�繝ｻ繝ｻ繝ｻ蟶晁箔郢晢�E��E�遶擾�E��E�驛｢・�E�郢晢�E��E�
			for (int i = 0; i < walls.size(); i++)
			{
				Wall* wall = walls.at(i)->CastTo<Wall>();
				if (wall != nullptr)
				{
					std::vector<VERTEX3D> vertex = wall->GetVertex();

					// 驛｢譎丞ｹ�E�・取ｨ抵�E��E�・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驛｢・�E�髮区�E�奁E���E�竏ｫ・�E�・�E�驛｢・�E�陋ｹ・�E�遶匁E���E�驍ｵ・�E�繝ｻ・�E�驛｢・�E�繝ｻ・�E�驛｢譎丞ｹ�E�邵�E�譎会ｽ�E�譎｢・�E�・�E�髯懷姓�E�E�譏ｴ繝ｻ髯具�E��E�繝ｻ・�E�髯橸�E��E�陞｢・�E�遶雁E���E�・�E�・�E�遶乗劼・�E�・�E�遶丞｣�E�繝ｻ髣匁E���E�髣・�E��E�繝ｻ・�E�髮区�E��E�・�E�・�E�繝ｻ・�E�驛｢譎�E�E��E�・取㏍・�E�・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�髯溷・萓ｭ隨�E�繝ｻ・�E�・�E�鬮�E�竏ｵ繝ｻ髯橸�E��E�陞｢・�E�繝ｻ蟶晏距陟募ｨ�E�魘ｬ
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



						// 髯橸�E��E�遶丞｣�E�繝ｻ髮取�E・�E�・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�鬮�E�・�E�陟暮�E�会ｽ�E�・�E�驛｢・�E�陋ｹ・�E�遶匁E���E�驍ｵ・�E�繝ｻ・�E�驛｢譎�E函邵�E�驢搾�E��E�譎�E樟�E取刮・�E�・�E�髮区�E��E�蠕宣辧霈斐・
						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 髯橸�E��E�遶擾�E��E�繝ｻ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�郢晢�E��E�驛｢・�E�繝ｻ・�E�驛｢譎�E樟�E弱・
						float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 鬩募�E・�E�・�E�髯�E�讎奁E���E�轣偁E���E�・�E�髯�E�・�E�闔会ｽ�E�郢晢�E��E�驛｢・�E�繝ｻ・�E�驛｢譎�E樟�E取刮・�E�・�E�繝ｻ・�E�髯�E�・�E�隶主�E�・�E��E�・�E�繝ｻ・�E�驛｢譎�E函邵�E�驢搾�E��E�譎�E樟�E取刮・�E�・�E�繝ｻ・�E�驍ｵ・�E�遶乗劼・�E�・�E�遶丞｣�E�繝ｻ髮主�E�薙�E繝ｻ・�E�陞｢・�E�遶雁E���E�・�E�・�E�繝ｻ・�E�髯�E��E�郢晢�E��E�繝ｻ・�E�鬮�E�・�E�繝ｻ螳夲�E��E�蠑ｱ・・・竏ｫ・�E�・�E�郢晢�E��E�
						slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 髯橸�E��E�遶擾�E��E�繝ｻ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�郢晢�E��E�驛｢・�E�繝ｻ・�E�驛｢譎�E樟�E取刮・�E�・�E�陞ｳ螟ｲ・�E�・�E�髢�E�・�E�繝ｻ・�E�郢晢�E��E�

						// 髣匁E���E�髫�E�魁E��ｽ�E�・�E�髯溯�E��E�・�E�・�E�髫�E�轣倡函繝ｻ螳夲�E��E�魁E��ｽ�E�・�E�驍ｵ・�E�陷会ｽ�E�遯�E�・�E�驍ｵ・�E�闕ｵ譎｢・�E�闁E��讀�E�遶丞｣�E�繝ｻ髮取�E・�E�・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�髮弱・・�E�・�E�驍ｵ・�E�闕ｳ螂�E�E��E�閧�E�・�E�・�E�郢晢�E��E�遶企豪・�E�・�E�陷�E�・�E�繝ｻ繝ｻ
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
	//髴托ｽ�E�繝ｻ・�E�髯懶�E��E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�髯懁E��・�E�繝ｻ・�E�・�E�繝ｻ・�E�髯区�E�ゑｽ�E�・�E�驍ｵ・�E�闕ｵ譎｢・�E�陋ｾ�E�・�E�繝ｻ・�E�髫�E�轣倡函郢晢�E��E�髯懁E��・�E�繝ｻ・�E�・�E�繝ｻ・�E�髯区�E�ゑｽ�E�・�E�驍ｵ・�E�繝ｻ・�E�髯晢�E��E�繝ｻ・�E�髯具�E��E�郢晢�E��E�繝ｻ螳夲�E��E�蠑ｱ・・・竏ｫ・�E�・�E�郢晢�E��E�
	float subAngle = mfTargetAngle - mfAngle;

	//驍ｵ・�E�郢�E�繝ｻ・�E�邁E���E��E�繝ｻ・�E�・�E�髯�E�・�E�闔会ｽ�E�・ゑｽ�E�驛｢・�E�陝ｲ・�E�遶匁E���E�驛｢・�E�陋ｹ・�E�陝・�E�髯�E�・�E�闔会ｽ�E�郢晢�E��E�髯晢�E��E�繝ｻ・�E�驍ｵ・�E�鬲・�E�夲�E��E�・�E�隰・∞�E�E�・�E�陋帙�E・�E�・�E�闔牙遜�E�E�・�E�繝ｻ・�E�髣比ｼ夲�E��E�・�E�髣匁E���E�陞ゅ・・�E�・�E�闔蛹・�E��E�・�E�繝ｻ・�E�髣匁E���E�陷茨�E��E�繝ｻ・�E�陝ｲ・�E�遶企豪・�E�・�E�繝ｻ・�E�驛｢・�E�闕ｵ譎｢・�E�繝ｻ・�E�・�E�繝ｻ・�E�驍ｵ・�E�陟募�E��E�繝ｻ驍ｵ・�E�郢晢�E��E�郢晢�E��E�驍ｵ・�E�陞｢・�E�遶企�E・�E�・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�
	//髯晢�E��E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�髯区�E�ゑｽ�E�・�E�驍ｵ・�E�鬲・�E�夲�E��E�・�E�隰・∞�E�E�・�E�陋帙�E・�E�・�E�髯應ｼ夲�E��E�・�E�繝ｻ・�E�髣匁E���E�陞ゅ・・�E�・�E�闔蛹・�E��E�・�E�繝ｻ・�E�髣匁E���E�陷茨�E��E�繝ｻ・�E�陝ｲ・�E�遶企豪・�E�・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�隨�E�繝ｻ・�E�・�E�髢�E�・�E�雎ｬ・�E�髮弱・・�E�・�E�驍ｵ・�E�陷�E�・�E�繝ｻ繝ｻ
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//鬮�E�闍難�E��E�讖ｸ・�E�・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�髯晢�E��E�繝ｻ・�E�髯具�E��E�郢晢�E��E�繝ｻ螳夲�E��E�謌翫・郢晢�E��E�遶願侭繝ｻ髣雁E���E�繝ｻ鬮�E�蜿�E�・�E�・�E�隨・�E��E�驍ｵ・�E�闔会ｽ�E�繝ｻ繝ｻ
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
	//髣碑崟螳�E�鬯�E�繝ｻ・�E�・�E�郢晢�E��E�遯�E�・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�陷会ｽ�E�繝ｻ讓｣蝗馴寞讖ｸ・�E�・�E�繝ｻ・�E�驛｢・�E�陜｣・�E�髢�E�繝ｻ諤弱・・�E�
	mfAngle = mfTargetAngle - subAngle;

	//髯懁E��・�E�繝ｻ・�E�・�E�繝ｻ・�E�髯区�E�ゑｽ�E�・�E�驛｢・�E�陞ｳ螟ｲ・�E�・�E�繝ｻ・�E�髯橸�E��E�郢晢�E��E�
	mvRotation.y = mfAngle + DX_PI_F;
	//驛｢譎｢・�E�・�E�驛｢譏ｴ繝ｻ・取刮・�E�・�E�繝ｻ・�E�髣費�E��E�隴擾�E��E�遶擾�E��E�驛｢・�E�郢晢�E��E�
	mpModel->SetRotation(mvRotation);


}

void Enemy::Damage(float damage)
{

	mfHp -= damage;
	if(mfHp <= 0)
	{
		mfHp = 0;
		isDead = true;
	}
}



void Enemy::DeathEnemy()
{
	if (!isDead)return;
	
	mpModel->ChangeAnimation(ANIMATION_DYING);
	//驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驛｢譎丞ｹ�E�郢晢�E��E�驍ｵ・�E�髴域喚髮�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�
	mpModel->SetLoop(false);
	//驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�髯溷供�E�E�蠕後�E髯溯�E�斐�E繝ｻ・�E�雋�E�E��E�逧�E�・�E�譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譎｢・�E�・�E�驍ｵ・�E�繝ｻ・�E�髫�E�魁E��ｽ�E�・�E�驍ｵ・�E�郢晢�E��E�
	//驛｢譏ｴ繝ｻ郢晢�E��E�驛｢譏ｴ繝ｻ郢晢�E��E�驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譏ｴ繝ｻ郢晢�E��E�髯溯�E�斐�E郢晢�E��E�髯溯�E�斐�E繝ｻ・�E�雋�E�E��E�逧�E�・�E�譏ｴ繝ｻ郢晢�E��E�驛｢・�E�繝ｻ・�E�驛｢譎｢・�E�・�E�驛｢譏ｴ繝ｻ郢晢�E��E�驍ｵ・�E�繝ｻ・�E�髫�E�魁E��ｽ�E�・�E�驍ｵ・�E�郢晢�E��E�
	mpModel->SetLoopFinishState(ANIMATION_MAX);
	
	Delete();
	if (mpModel->IsAnimationLoopFinish())
	{
		GiveRewards();
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
		mpStopCollider->mvPosition = pos;
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->mvPosition = pos;
	}

}

void Enemy::OnEnter(Collider* collider, Collider* check)//髯晢�E��E�繝ｻ・�E�髯句�E��E�・�E�・�E�.
{
	auto mpPlayer = Master::mpPlayer;
	if (mfHp <= 0)return;
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Obj)
	{
		
		mvPosition = VinitPos;
	}
	
	if (check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::mpPlayer;
		if (player == nullptr)return;
	
		if (collider == mpSerchCollider &&player->GetCollisionCollider()==check)
		{
			isHitSearch = true;
		}
		if (collider == mpAttackCollider && player->GetCollisionCollider() == check)
		{
			isHitAttackSearch = true;//髫�E�・�E�繝ｻ・�E�髫�E�・�E�郢晢�E��E�陝ｷ謌頑ｲり嵯譏ｴ�E�ｨ驛｢譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�

		}
		if (collider == mpStopCollider && player->GetCollisionCollider() == check)
		{
			isHitSerchStop = true;
		}
	}
	//髯溷・萓ｭ隨�E�繝ｻ・�E�・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�繝ｻ遏ｩ・�E�・�E�隰撰�E��E�陷・�E��E�驍ｵ・�E�繝ｻ・�E�髯�E�繝ｻ・�E�・�E�鬨�E�繝ｻ繝ｻ
}

void Enemy::OnTrigger(Collider* collider, Collider* check)
{//髯溷・萓ｭ隨�E�繝ｻ・�E�・�E�繝ｻ・�E�驍ｵ・�E�雋�E�E��E�隶・�E�鬯�E�・�E�髦�E�蜷�E�繝ｻ髯�E�繝ｻ・�E�・�E�鬨�E�繝ｻ繝ｻ
	if (mfHp <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = mpModel->GetNowState();
	if (collider == mpAttachCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)
	{//mpModel驍ｵ・�E�繝ｻ・�E�鬨�E�・�E�繝ｻ・�E�髯�E�・�E�繝ｻ・�E�驛｢・�E�郢�E�繝ｻ譌ｺ驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�繝ｻ・�E�驍ｵ・�E�郢晢�E��E�繝ｻ迢暦�E��E�・�E�遶丞､ゑｽ�E�・�E�繝ｻ・�E�驍ｵ・�E�髦�E�蜷�E�繝ｻif髫�E�竏壹・遶頑･�E�諤�E�繝ｻ・�E�驛｢・�E�陝ｲ・�E�遶企�E・�E�・�E�郢晢�E��E�
		Player3D* player = Master::mpPlayer;
		if (player == nullptr)return;
		if (check == player->GetCollisionCollider())
		{
			if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
			{

				player->Damage(mfAttack);
				AttackHitJudgmentflag = true;//髯溷・萓ｭ隨�E�繝ｻ・�E�・�E�繝ｻ・�E�驍ｵ・�E�雋�E∞�E�E�閧�E�・�E�譎｢・�E�・�E�
			}
		}
	}
	
}

void Enemy::OnExit(Collider* collider, Collider* check)
{
	if (mfHp <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = mpModel->GetNowState();
	if (check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		
		Player3D* player = Master::mpPlayer;
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
				isHitAttackSearch = false;//髫�E�・�E�繝ｻ・�E�髫�E�・�E�郢晢�E��E�陝ｷ謌頑ｲり嵯譏ｴ�E�ｨ驛｢譎｢・�E�・�E�驛｢・�E�繝ｻ・�E�

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


void Enemy::GiveRewards()
{
	auto mpPlayer = Master::mpPlayer;
	Player3D* player = Master::mpPlayer;
	if (player != nullptr) {
		player->mpHaveMoney->AddMoney(mfHaveMoney);
		player->mpLevelUp->AddXp(mfHaveXp);
	}
}



