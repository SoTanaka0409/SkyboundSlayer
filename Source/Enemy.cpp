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
	//郢晢ｽ｢郢昴・ﾎ晉ｸｺ・ｮ騾墓ｻ薙・
	mpModel = new Model(filename, initPos, isSeparateAnim);
	mpModel->SetScale(VGet(1.3f, 1.3f, 1.3f));
	VinitPos = initPos;
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;

	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize/2, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 30.0f);
	mpSerchCollider = new SphereCollider(this, mvPosition, HitSerch);//隰ｨ・ｵ郢ｧ蜻育粟驕擾ｽ･邵ｺ蜷ｶ・玖怙繝ｻ
	mpAttackCollider = new SphereCollider(this, mvPosition, HitAttackSerch);//隰ｨ・ｵ郢ｧ蜻亥愛隰ｦ繝ｻ・�陝倶ｹ晢ｽ∫ｹｧ蜿･繝ｻ
	mpStopCollider = new SphereCollider(this, mvPosition, HitStopSerch);//隰ｨ・ｵ邵ｺ・ｨ邵ｺ・ｮ髴肴辨螻ｬ郢ｧ蜑・ｽｿ譏ｴ笆ｽ陷繝ｻ
	//new DrawHp("",  //騾包ｽｻ陷剃ｸ樣倹
	//	(VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f))),//闕ｳ・ｭ陟｢繝ｻ・ｺ・ｧ隶薙・
	  // 郢ｧ・ｳ郢晢ｽｩ郢ｧ・､郢晉ｹ晢ｽｼ騾墓ｻ薙・
	
	
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
			DeathEnemy();//隰ｨ・ｵ郢ｧ雋槫・・�邵ｺ貅倪・邵ｺ髦ｪ竊鍋ｹｧ・ｯ郢晢ｽｪ郢ｧ・｢邵ｺ蜷ｶ・狗ｸｺ貅假ｽ∫ｸｺ・ｮ郢ｧ・ｫ郢ｧ・ｦ郢晢ｽｳ郢晏現・定ｲょｸ呻ｽ臥ｸｺ繝ｻ
			Attack();
			CollPositionUpdate();
			RotationByMove();
			Move();
			/////陟托ｽｾm郢ｧ蜻磯□邵ｺ・､陷・ｽｦ騾・・
		
			mpModel->Update();
			//mpDH->Update();//drawHp邵ｺ・ｮ郢ｧ・｢郢昴・繝ｻ郢昴・繝ｻ郢晏現・定惱・ｼ邵ｺ・ｶ
			
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
		//隰ｾ・ｻ隰ｦ繝ｻﾎ皮ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ・ｫ陞溽判蟲ｩ
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//郢晢ｽｫ郢晢ｽｼ郢晏干繝ｻ邵ｺ霈披雷邵ｺ・ｪ邵ｺ繝ｻ
		mpModel->SetLoop(false);
		//郢晢ｽ｢郢晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ陟募ｾ後・陟輔・・ｩ貅佩皮ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ・ｫ隰鯉ｽｻ邵ｺ繝ｻ
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
	if (Master::SafePointOn)mvPosition = VinitPos;//郢ｧ繧・ｼ�郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ謔滂ｽｮ迚吶・陜ｨ・ｰ陝ｶ・ｯ邵ｺ・ｫ邵ｺ繝ｻ・狗ｸｺ・ｪ郢ｧ迚吶・隴帶ｺ倥・郢ｧ・ｹ邵ｺ・ｫ隰鯉ｽｻ郢ｧ繝ｻ
	

	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);

	 moveVec = VGet(0.0f, 0.0f, 0.0f);
	 UpMoveVector = VGet(0.0f, 0.0f, 0.0f);
	 leftMoveVector = VGet(0.0f, 0.0f, 0.0f);
	{
		//闕ｳ鬆大ｩｿ陷ｷ莉｣竏育ｸｺ・ｮ驕假ｽｻ陷崎ｼ斐・郢ｧ・ｯ郢晏現ﾎ晉ｸｺ・ｯ邵ｲ竏壹″郢晢ｽ｡郢晢ｽｩ髫穂ｹ溘○隴・ｽｹ陷ｷ莉｣ﾂｰ郢ｧ隰∬ｬ御ｻ吶・郢ｧ蜻域�｢邵ｺ繝ｻ笳・ｹｧ繧・・邵ｺ・ｨ邵ｺ蜷ｶ・・
		UpMoveVector = VSub(Master::mpCamera->GetlookAtPosition(), Master::mpCamera->GetPosition());
		UpMoveVector.y = 0.0f;

		//陝ｾ・ｦ隴・ｽｹ陷ｷ莉｣竏育ｸｺ・ｮ驕假ｽｻ陷崎ｼ斐・郢ｧ・ｯ郢晏現ﾎ晉ｸｺ蠕娯或・ｸ鬆大ｩｿ陷ｷ莉｣繝ｻ驕假ｽｻ陷崎ｼ斐・郢ｧ・ｯ郢晏現ﾎ晉ｸｺ・ｨ邵ｲ縲・怕・ｸ邵ｺ・ｮ郢晏干ﾎ帷ｹｧ・ｹ隴・ｽｹ陷ｷ莉｣竏育ｸｺ・ｮ郢晏生縺醍ｹ晏現ﾎ晢ｽｫ陜吶ｉ蟲ｩ邵ｺ・ｪ隴・ｽｹ陷ｷ謇假ｽｼ莠･・､荵滂ｽｩ謳ｾ・ｼ繝ｻ
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));
		leftMoveVector.y = 0.0f;


		//驕假ｽｻ陷崎ｼ斐・郢ｧ・ｯ郢晏現ﾎ晉ｸｺ・ｯ驕假ｽｻ陷肴坩郢ｧ雋槫・陷ｻ・ｳ邵ｺ蜉ｱ竊醍ｸｺ繝ｻ繝ｻ邵ｺ・ｧ邵ｲ竏ｵ・ｭ・｣髫穂ｸ槫密邵ｺ蜉ｱ窶ｻ邵ｺ鄙ｫ・･(郢晏生縺醍ｹ晏現ﾎ晉ｸｺ・ｮ鬮滂ｽｷ邵ｺ霈費ｽ偵・莉｣竊鍋ｸｺ蜷ｶ・狗ｸｺ阮吮・)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}


	//////////////驕假ｽｻ陷榊｢灘ｩｿ陷ｷ莉｣竊堤ｹｧ・ｹ郢晄鱒繝ｻ郢晏ｳｨ繝ｻ陷・ｽｦ騾・・///////////////
	{
		WalkCount++;
		if (isHitSerchStop)
		{
			mfSpeed = 0;///鬨ｾ・ｲ邵ｺ・ｾ邵ｺ・ｪ邵ｺ繝ｻ・育ｸｺ繝ｻ竊鍋ｸｺ蜷ｶ・・
		}
		else
		{
			mfSpeed = mfNormalSpeed;
		}
		if (WalkTimer < WalkCount)
		{
			WalkTimer = rand()%300;//譁ｹ蜷題ｻ｢謠帙ち繧､繝槭・
			mnAlgorithm = rand()%8; // 0-3: 豁ｩ縺・ 4-7: 遶九■豁｢縺ｾ繧・
			WalkCount = 0;
		}

		if (mnAlgorithm == 0)//蟾ｦ譁ｹ蜷代∈縺ｮ遘ｻ蜍・
		{
			moveVec = VAdd(moveVec, leftMoveVector);
		}
		else if (mnAlgorithm == 1)//蜿ｳ譁ｹ蜷代∈縺ｮ遘ｻ蜍・
		{
			moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
		}
		else if (mnAlgorithm == 2)//螂･譁ｹ蜷代∈縺ｮ遘ｻ蜍・
		{
			moveVec = VAdd(moveVec, UpMoveVector);
		}
		else if (mnAlgorithm == 3)//謇句燕譁ｹ蜷代∈縺ｮ遘ｻ蜍・
		{
			moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));
		}
		else
		{
			// mnAlgorithm縺・4・・ 縺ｮ蝣ｴ蜷医・菴輔ｂ縺励↑縺・ｼ育ｫ九■豁｢縺ｾ繧具ｼ・
		}



		GoPosition = VSub(pPlayer->GetPosition(), mvPosition);
		GoPosition = VNorm(GoPosition);///郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ・ｸ陷ｷ莉｣ﾂｰ邵ｺ繝ｻ繝ｻ騾・・


		bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
		if (isMove || isHitSearch)
		{
			mpModel->ChangeAnimation(ANIMATION_RUN);

			//驕假ｽｻ陷榊｢灘ｩｿ陷ｷ莉｣・定ｱ・ｽ｣髫穂ｸ槫密邵ｺ蜉ｱ窶ｻ邵ｺ鄙ｫ・･
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
		VECTOR oldPosition = mvPosition;//陷第ｦ雁ｱ鍋ｸｺ・ｮ陟趣ｽｧ隶灘生・定叉隴鯉ｽｦ闖ｫ譎・亜
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
		//郢ｧ・ｹ郢昴・繝ｻ郢ｧ・ｸ邵ｺ・ｨ邵ｺ・ｮ陟冶侭笳・ｹｧ髮∵・陞ｳ螢ｹ・堤ｸｺ蜷ｶ・・
		hitPos = VGet(0.0f, 0.0f, 0.0f);
		bool isHit = false;


		auto obj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Stage);
		if (obj != nullptr)
		{
			Stage* pStage = dynamic_cast<Stage*>(obj);
			if (pStage != nullptr)
			{
				//郢ｧ・ｹ郢昴・繝ｻ郢ｧ・ｸ邵ｺ・ｨ郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ・ｮ郢ｧ・ｫ郢晏干縺晉ｹ晢ｽｫ邵ｺ謔滂ｽｽ阮吮螺邵ｺ・｣邵ｺ・ｦ邵ｺ繝ｻ・玖撻・ｴ陷ｷ繝ｻ
				if (pStage->CheckHit_Capsule(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
				{
					//陟冶侭笳・ｸｺ・｣邵ｺ・ｦ邵ｺ繝ｻ・狗ｸｺ・ｧ邵ｺ繧・ｽ咲ｸｺ繝ｻ繝ｻ郢晢ｽｪ郢ｧ・ｴ郢晢ｽｳ邵ｺ・ｨ邵ｺ・ｮ隰暦ｽ･髫暦ｽｦ霓､・ｹ郢ｧ蜻茨ｽｱ繧・ｽ∫ｹｧ繝ｻ
					hitPos = pStage->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),//enemy邵ｺ・ｮ髢ｹ譎擾ｽｽ阮吮螺郢ｧ繝ｻ陞溷｣ｼ繝ｻ)邵ｺ・ｨ
						VAdd(mvPosition, VGet(0.0f, -40.0f, 0.0f))//enemy邵ｺ・ｮ陝・ｻ｣・�闕ｳ荵昶旺邵ｺ貅假ｽ顔ｹｧ蝣､・ｷ螢ｼ繝ｻ邵ｺ・ｨ邵ｺ蜉ｱ窶ｻ隰悶・・ｮ繝ｻ
					);

					//陟冶侭笳・ｸｺ・｣邵ｺ貅ｷ諢幄楜螢ｹ・定愾謔ｶ笆ｲ邵ｺ・ｦ邵ｺ鄙ｫ・･
					isHit = true;
				}
			}
		}

		if (isHit)
		{
			//陜ｨ・ｰ鬮ｱ・｢邵ｺ・ｫ雎撰ｽｿ邵ｺ・｣邵ｺ・ｦ雎・ｽｩ邵ｺ繝ｻ窶ｻ邵ｺ繝ｻ・玖ｿ･・ｶ隲ｷ荵昶・邵ｺ蜉ｱ窶ｻ邵ｲ縲・�趣ｽｧ隶灘生・堤ｹｧ・ｹ郢昴・繝ｻ郢ｧ・ｸ邵ｺ・ｫ陷ｷ蛹ｻ・冗ｸｺ蟶呻ｽ・
			mvPosition.y = hitPos.y;
		}
		if (isHit == false)
		{
			//髫補扱髫ｼ陜溘・
			mvPosition.y += -4.0f;//髣懶ｽｽ闕ｳ荵昶・郢ｧ遏ｩ貅ｷ・ｺ・ｦ
			if (mvPosition.y <= 0.0f)
			{
				mvPosition.y = oldPosition.y;
			}
		}
		auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
		if (!walls.empty())
		{
			// 陟冶侭笳・ｸｺ・｣邵ｺ・ｦ邵ｺ繝ｻ・玖椶竏夲ｽ帝坡・ｿ邵ｺ・ｹ郢ｧ繝ｻ
			// hint: 霑ｴ・ｾ霑･・ｶ邵ｺ・ｮ陷・ｽｦ騾・・縲堤ｸｺ・ｯ1隴ｫ螢ｹ繝ｻ陞｢竏夲ｼ�邵ｺ蛹ｺ諤咎お繧牙飭邵ｺ・ｫ陋ｻ・､陞ｳ螢ｹ・・ｹｧ蠕娯・邵ｺ繝ｻ繝ｻ邵ｺ・ｧ邵ｲ繝ｻ隴ｫ螟ｲ・ｼ莠包ｽｻ・･闕ｳ螂・ｽｼ蟲ｨ竊楢�冶侭笳・ｸｺ・｣邵ｺ・ｦ邵ｺ繝ｻ笳・撻・ｴ陷ｷ蛹ｻ繝ｻ陷・ｽｦ騾・・・帝蔓繝ｻ竏ｴ郢ｧ繝ｻ
			for (int i = 0; i < walls.size(); i++)
			{
				Wall* wall = dynamic_cast<Wall*>(walls.at(i));
				if (wall != nullptr)
				{
					std::vector<VERTEX3D> vertex = wall->GetVertex();

					// 郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ郢ｧ雋樊｡∫ｹｧ郢ｧ蛹ｻ竕ｧ邵ｺ・ｪ郢ｧ・ｫ郢晏干縺晉ｹ晢ｽｫ陜吩ｹ昴・陋ｻ・､陞ｳ螢ｹ竊堤ｸｲ竏晢ｽ｣竏壹・闕ｳ闃ｽ・ｧ雋橸ｽｽ・｢郢晄亢ﾎ懃ｹｧ・ｴ郢晢ｽｳ邵ｺ・ｨ邵ｺ・ｮ陟冶侭笳・ｹｧ髮∵・陞ｳ螢ｹ・帝勗蠕娯鴬
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



						// 陞｢竏壺・雎撰ｽｿ邵ｺ・｣邵ｺ・ｦ髯ｦ蠕鯉ｿ･郢ｧ蛹ｻ竕ｧ邵ｺ・ｪ郢晏生縺醍ｹ晏現ﾎ晉ｹｧ雋槫徐陟輔・
						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 陞｢竏ｵ・ｲ・ｿ邵ｺ繝ｻ繝ｻ郢ｧ・ｯ郢晏現ﾎ・
						float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 驕假ｽｻ陷榊｢灘ｩｿ陷ｷ莉｣繝ｻ郢ｧ・ｯ郢晏現ﾎ晉ｸｺ・ｮ陷ｿ讎奇ｽｯ・ｾ郢晏生縺醍ｹ晏現ﾎ晉ｸｺ・ｨ邵ｲ竏晢ｽ｣竏壹・雎墓・・ｷ螢ｹ竊堤ｸｺ・ｮ陷繝ｻ・ｩ髦ｪ・定ｱ弱ｅ・∫ｹｧ繝ｻ
						slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 陞｢竏ｵ・ｲ・ｿ邵ｺ繝ｻ繝ｻ郢ｧ・ｯ郢晏現ﾎ晉ｹｧ螳夲ｽｨ閧ｲ・ｮ繝ｻ

						// 闕ｳ隴鯉ｽｦ陟趣ｽｧ隶灘生・定ｬ鯉ｽｻ邵ｺ蜉ｱ窶ｻ邵ｺ荵晢ｽ芽椶竏壺・雎撰ｽｿ邵ｺ・｣邵ｺ・ｦ雎・ｽｩ邵ｺ荳奇ｽ育ｸｺ繝ｻ竊鍋ｸｺ蜷ｶ・・
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
	//霑ｴ・ｾ陜ｨ・ｨ邵ｺ・ｮ陜玲ｫ・ｽｻ・｢陋滂ｽ､邵ｺ荵晢ｽ蛾ｶ・ｮ隶灘生繝ｻ陜玲ｫ・ｽｻ・｢陋滂ｽ､邵ｺ・ｮ陝ｾ・ｮ陋ｻ繝ｻ・定ｱ弱ｅ・∫ｹｧ繝ｻ
	float subAngle = mfTargetAngle - mfAngle;

	//邵ｺ繧・ｽ玖ｭ・ｽｹ陷ｷ莉｣ﾂｰ郢ｧ蟲ｨ竕�郢ｧ蛹ｺ蟀ｿ陷ｷ莉｣繝ｻ陝ｾ・ｮ邵ｺ魃会ｽｼ謇假ｽｼ蛛・ｽｼ莉呻ｽｺ・ｦ闔会ｽ･闕ｳ螂・ｽｼ莠包ｽｻ・･闕ｳ蜈ｷ・ｼ蟲ｨ竊鍋ｸｺ・ｪ郢ｧ荵晢ｼ・ｸｺ・ｨ邵ｺ蠕娯・邵ｺ繝ｻ繝ｻ邵ｺ螢ｹ竊醍ｸｺ・ｮ邵ｺ・ｧ
	//陝ｾ・ｮ邵ｺ・ｮ陋滂ｽ､邵ｺ魃会ｽｼ謇假ｽｼ蛛・ｽｼ陜会ｽｻ・･闕ｳ螂・ｽｼ莠包ｽｻ・･闕ｳ蜈ｷ・ｼ蟲ｨ竊鍋ｸｺ・ｪ邵ｺ・｣邵ｺ・ｦ邵ｺ繝ｻ笳・ｹｧ閾･豬｣雎・ｽ｣邵ｺ蜷ｶ・・
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//髫苓ｲ橸ｽｺ・ｦ邵ｺ・ｮ陝ｾ・ｮ陋ｻ繝ｻ・定�戊・繝ｻ竊薙・闊娯・髴台ｻ｣笆ｼ邵ｺ莉｣・・
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
	//闔蛾宦鬮・ｸｺ繝ｻ窶ｻ邵ｺ・ｻ邵ｺ蜉ｱ・樣囓雋橸ｽｺ・ｦ郢ｧ蝣､閧・怎・ｺ
	mfAngle = mfTargetAngle - subAngle;

	//陜玲ｫ・ｽｻ・｢陋滂ｽ､郢ｧ螳夲ｽｨ・ｭ陞ｳ繝ｻ
	mvRotation.y = mfAngle + DX_PI_F;
	//郢晢ｽ｢郢昴・ﾎ晉ｸｺ・ｫ闔ｨ譏ｴ竏ｴ郢ｧ繝ｻ
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
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	
	mpModel->ChangeAnimation(ANIMATION_DYING);
	//郢晢ｽｫ郢晢ｽｼ郢晏干繝ｻ邵ｺ霈披雷邵ｺ・ｪ邵ｺ繝ｻ
	mpModel->SetLoop(false);
	//郢晢ｽ｢郢晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ陟募ｾ後・陟輔・・ｩ貅佩皮ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ・ｫ隰鯉ｽｻ邵ｺ繝ｻ
	//郢昴・繝ｻ郢昴・繝ｻ郢ｧ・ｼ郢晢ｽｧ郢昴・繝ｻ陟輔・繝ｻ陟輔・・ｩ貅佩皮ｹ昴・繝ｻ郢ｧ・ｼ郢晢ｽｧ郢昴・繝ｻ邵ｺ・ｫ隰鯉ｽｻ邵ｺ繝ｻ
	mpModel->SetLoopFinishState(ANIMATION_MAX);
	
	Delete();
	if (mpModel->IsAnimationLoopFinish())
	{
		if (mbItem && GetRand(100) < mnChance) {
			Item::ItemInformation* info = new Item::ItemInformation();
			info->ID = Item::HEAL;
			info->Count = 1;
			info->Name = "HEAL";
			Master::mpItemManager->AddItem(info);
		}
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
		mpStopCollider->mvPosition = pos;
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->mvPosition = pos;
	}

}

void Enemy::OnEnter(Collider* collider, Collider* check)//陝ｾ・ｦ陋幢ｽｴ.
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
			isHitAttackSearch = true;//隰ｾ・ｻ隰ｦ繝ｻ蟷戊沂荵昴Ψ郢晢ｽｩ郢ｧ・ｰ

		}
		if (collider == mpStopCollider && player->GetCollisionCollider() == check)
		{
			isHitSerchStop = true;
		}
	}
	//陟冶侭笳・ｸｺ・｣邵ｺ・ｦ邵ｺ繝ｻ・矩ｫ｢謐ｺ蜃ｾ邵ｺ・ｮ陷・ｽｦ騾・・
}

void Enemy::OnTrigger(Collider* collider, Collider* check)
{//陟冶侭笳・ｸｺ・｣邵ｺ貅ｽ讀ｪ鬮｢阮吶・陷・ｽｦ騾・・
	if (mfHp <= 0)return; auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	AnimationState now = mpModel->GetNowState();
	if (collider == mpAttachCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)
	{//mpModel邵ｺ・ｮ騾｡・ｪ陷ｿ・ｷ郢ｧ繧・旺邵ｺ・｣邵ｺ・ｦ邵ｺ繝ｻ・狗ｸｲ竏夂ｸｺ・ｩ邵ｺ阮吶・if隴√・竊楢怦・･郢ｧ蟲ｨ竊醍ｸｺ繝ｻ
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr)return;
		if (check == player->GetCollisionCollider())
		{
			if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
			{

				player->Damage(mfAttack);
				AttackHitJudgmentflag = true;//陟冶侭笳・ｸｺ・｣邵ｺ貅假ｽ育ｹ晢ｽｼ
			}
		}
	}
	
}

void Enemy::OnExit(Collider* collider, Collider* check)
{
	if (mfHp <= 0)return; auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
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
				isHitAttackSearch = false;//隰ｾ・ｻ隰ｦ繝ｻ蟷戊沂荵昴Ψ郢晢ｽｩ郢ｧ・ｰ

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
