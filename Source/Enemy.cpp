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
	//驛｢譎｢・ｽ・｢驛｢譏ｴ繝ｻ・取刮・ｸ・ｺ繝ｻ・ｮ鬨ｾ蠅難ｽｻ阮吶・
	mpModel = new Model(filename, initPos, isSeparateAnim);
	mpModel->SetScale(VGet(1.3f, 1.3f, 1.3f));
	VinitPos = initPos;
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;

	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize/2, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 30.0f);
	mpSerchCollider = new SphereCollider(this, mvPosition, HitSerch);//髫ｰ・ｨ繝ｻ・ｵ驛｢・ｧ陷ｻ閧ｲ邊滄ｩ墓得・ｽ・･驍ｵ・ｺ陷ｷ・ｶ繝ｻ邇匁咏ｹ晢ｽｻ
	mpAttackCollider = new SphereCollider(this, mvPosition, HitAttackSerch);//髫ｰ・ｨ繝ｻ・ｵ驛｢・ｧ陷ｻ莠･諢幃垈・ｦ郢晢ｽｻ繝ｻ・ｽ髯晏ｶ・ｹ譎｢・ｽ竏ｫ・ｹ・ｧ陷ｿ・･郢晢ｽｻ
	mpStopCollider = new SphereCollider(this, mvPosition, HitStopSerch);//髫ｰ・ｨ繝ｻ・ｵ驍ｵ・ｺ繝ｻ・ｨ驍ｵ・ｺ繝ｻ・ｮ鬮ｴ閧ｴ霎ｨ陞ｻ・ｬ驛｢・ｧ陷代・・ｽ・ｿ隴擾ｽｴ隨・ｽｽ髯ｷﾂ郢晢ｽｻ
	//new DrawHp("",  //鬨ｾ蛹・ｽｽ・ｻ髯ｷ蜑・ｽｸ讓｣蛟ｹ
	//	(VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f))),//髣包ｽｳ繝ｻ・ｭ髯滂ｽ｢郢晢ｽｻ繝ｻ・ｺ繝ｻ・ｧ髫ｶ阮吶・
	  // 驛｢・ｧ繝ｻ・ｳ驛｢譎｢・ｽ・ｩ驛｢・ｧ繝ｻ・､驛｢譎会ｽｹ譎｢・ｽ・ｼ鬨ｾ蠅難ｽｻ阮吶・
	
	
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
			DeathEnemy();//髫ｰ・ｨ繝ｻ・ｵ驛｢・ｧ髮区ｧｫ繝ｻ繝ｻ・ｽ驍ｵ・ｺ雋・ｪ繝ｻ驍ｵ・ｺ鬮ｦ・ｪ遶企豪・ｹ・ｧ繝ｻ・ｯ驛｢譎｢・ｽ・ｪ驛｢・ｧ繝ｻ・｢驍ｵ・ｺ陷ｷ・ｶ繝ｻ迢暦ｽｸ・ｺ雋・∞・ｽ竏ｫ・ｸ・ｺ繝ｻ・ｮ驛｢・ｧ繝ｻ・ｫ驛｢・ｧ繝ｻ・ｦ驛｢譎｢・ｽ・ｳ驛｢譎冗樟繝ｻ螳夲ｽｲ繧・ｽｸ蜻ｻ・ｽ閾･・ｸ・ｺ郢晢ｽｻ
			Attack();
			CollPositionUpdate();
			RotationByMove();
			Move();
			/////髯滓汚・ｽ・ｾm驛｢・ｧ陷ｻ逎ｯ笆｡驍ｵ・ｺ繝ｻ・､髯ｷ繝ｻ・ｽ・ｦ鬨ｾ繝ｻ繝ｻ
		
			mpModel->Update();
			//mpDH->Update();//drawHp驍ｵ・ｺ繝ｻ・ｮ驛｢・ｧ繝ｻ・｢驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢譎冗樟繝ｻ螳壽Τ繝ｻ・ｼ驍ｵ・ｺ繝ｻ・ｶ
			
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
		//髫ｰ・ｾ繝ｻ・ｻ髫ｰ・ｦ郢晢ｽｻ・守坩・ｹ譎｢・ｽ・ｼ驛｢・ｧ繝ｻ・ｷ驛｢譎｢・ｽ・ｧ驛｢譎｢・ｽ・ｳ驍ｵ・ｺ繝ｻ・ｫ髯樊ｺｽ蛻､陝ｲ・ｩ
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//驛｢譎｢・ｽ・ｫ驛｢譎｢・ｽ・ｼ驛｢譎丞ｹｲ郢晢ｽｻ驍ｵ・ｺ髴域喚髮ｷ驍ｵ・ｺ繝ｻ・ｪ驍ｵ・ｺ郢晢ｽｻ
		mpModel->SetLoop(false);
		//驛｢譎｢・ｽ・｢驛｢譎｢・ｽ・ｼ驛｢・ｧ繝ｻ・ｷ驛｢譎｢・ｽ・ｧ驛｢譎｢・ｽ・ｳ髯溷供・ｾ蠕後・髯溯ｼ斐・繝ｻ・ｩ雋・ｽｩ逧ｮ・ｹ譎｢・ｽ・ｼ驛｢・ｧ繝ｻ・ｷ驛｢譎｢・ｽ・ｧ驛｢譎｢・ｽ・ｳ驍ｵ・ｺ繝ｻ・ｫ髫ｰ魃会ｽｽ・ｻ驍ｵ・ｺ郢晢ｽｻ
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
	if (Master::SafePointOn)mvPosition = VinitPos;//驛｢・ｧ郢ｧ繝ｻ・ｼ・ｽ驛｢譎丞ｹｲ・取ｨ抵ｽｹ・ｧ繝ｻ・､驛｢譎｢・ｽ・､驛｢譎｢・ｽ・ｼ驍ｵ・ｺ隰疲ｻゑｽｽ・ｮ霑壼生繝ｻ髯懶ｽｨ繝ｻ・ｰ髯晢ｽｶ繝ｻ・ｯ驍ｵ・ｺ繝ｻ・ｫ驍ｵ・ｺ郢晢ｽｻ繝ｻ迢暦ｽｸ・ｺ繝ｻ・ｪ驛｢・ｧ霑壼生繝ｻ髫ｴ蟶ｶ・ｺ蛟･繝ｻ驛｢・ｧ繝ｻ・ｹ驍ｵ・ｺ繝ｻ・ｫ髫ｰ魃会ｽｽ・ｻ驛｢・ｧ郢晢ｽｻ
	

	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);

	moveVec = VGet(0.0f, 0.0f, 0.0f);
	{
	}
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
		//驛｢・ｧ繝ｻ・ｹ驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢・ｧ繝ｻ・ｸ驍ｵ・ｺ繝ｻ・ｨ驍ｵ・ｺ繝ｻ・ｮ髯溷・萓ｭ隨ｳ繝ｻ・ｹ・ｧ鬮ｮ竏ｵ繝ｻ髯橸ｽｳ陞｢・ｹ繝ｻ蝣､・ｸ・ｺ陷ｷ・ｶ繝ｻ繝ｻ
		hitPos = VGet(0.0f, 0.0f, 0.0f);
		bool isHit = false;


		auto obj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Stage);
		if (obj != nullptr)
		{
			Stage* pStage = dynamic_cast<Stage*>(obj);
			if (pStage != nullptr)
			{
				//驛｢・ｧ繝ｻ・ｹ驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢・ｧ繝ｻ・ｸ驍ｵ・ｺ繝ｻ・ｨ驛｢譎丞ｹｲ・取ｨ抵ｽｹ・ｧ繝ｻ・､驛｢譎｢・ｽ・､驛｢譎｢・ｽ・ｼ驍ｵ・ｺ繝ｻ・ｮ驛｢・ｧ繝ｻ・ｫ驛｢譎丞ｹｲ邵ｺ譎会ｽｹ譎｢・ｽ・ｫ驍ｵ・ｺ隰疲ｻゑｽｽ・ｽ髦ｮ蜷ｮ陞ｺ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ郢晢ｽｻ繝ｻ邇匁捗繝ｻ・ｴ髯ｷ・ｷ郢晢ｽｻ
				if (pStage->CheckHit_Capsule(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
				{
					//髯溷・萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ郢晢ｽｻ繝ｻ迢暦ｽｸ・ｺ繝ｻ・ｧ驍ｵ・ｺ郢ｧ繝ｻ・ｽ蜥ｲ・ｸ・ｺ郢晢ｽｻ郢晢ｽｻ驛｢譎｢・ｽ・ｪ驛｢・ｧ繝ｻ・ｴ驛｢譎｢・ｽ・ｳ驍ｵ・ｺ繝ｻ・ｨ驍ｵ・ｺ繝ｻ・ｮ髫ｰ證ｦ・ｽ・･鬮ｫ證ｦ・ｽ・ｦ髴難ｽ､繝ｻ・ｹ驛｢・ｧ陷ｻ闌ｨ・ｽ・ｱ郢ｧ繝ｻ・ｽ竏ｫ・ｹ・ｧ郢晢ｽｻ
					hitPos = pStage->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),//enemy驍ｵ・ｺ繝ｻ・ｮ鬮｢・ｹ隴取得・ｽ・ｽ髦ｮ蜷ｮ陞ｺ驛｢・ｧ郢晢ｽｻ髯樊ｺｷ・｣・ｼ郢晢ｽｻ)驍ｵ・ｺ繝ｻ・ｨ
						VAdd(mvPosition, VGet(0.0f, -40.0f, 0.0f))//enemy驍ｵ・ｺ繝ｻ・ｮ髯昴・・ｻ・｣繝ｻ・ｽ髣包ｽｳ闕ｵ譏ｶ譌ｺ驍ｵ・ｺ雋・∞・ｽ鬘費ｽｹ・ｧ陜｣・､繝ｻ・ｷ陞｢・ｼ郢晢ｽｻ驍ｵ・ｺ繝ｻ・ｨ驍ｵ・ｺ陷会ｽｱ遯ｶ・ｻ髫ｰ謔ｶ繝ｻ繝ｻ・ｮ郢晢ｽｻ
					);

					//髯溷・萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ雋・ｽｷ隲｢蟷・･懆椶・ｹ繝ｻ螳壽╂隰費ｽｶ隨・ｽｲ驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ驗呻ｽｫ繝ｻ・･
					isHit = true;
				}
			}
		}

		if (isHit)
		{
			//髯懶ｽｨ繝ｻ・ｰ鬯ｮ・ｱ繝ｻ・｢驍ｵ・ｺ繝ｻ・ｫ髮取腸・ｽ・ｿ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ髮弱・・ｽ・ｩ驍ｵ・ｺ郢晢ｽｻ遯ｶ・ｻ驍ｵ・ｺ郢晢ｽｻ繝ｻ邇厄ｽｿ・･繝ｻ・ｶ髫ｲ・ｷ闕ｵ譏ｶ繝ｻ驍ｵ・ｺ陷会ｽｱ遯ｶ・ｻ驍ｵ・ｲ邵ｲ繝ｻ・ｽ雜｣・ｽ・ｧ髫ｶ轣倡函繝ｻ蝣､・ｹ・ｧ繝ｻ・ｹ驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢・ｧ繝ｻ・ｸ驍ｵ・ｺ繝ｻ・ｫ髯ｷ・ｷ陋ｹ・ｻ繝ｻ蜀暦ｽｸ・ｺ陝ｶ蜻ｻ・ｽ繝ｻ
			mvPosition.y = hitPos.y;
		}
		if (isHit == false)
		{
			//鬮ｫ陬懈桶鬮ｫ・ｼ髯懈ｺ倥・
			mvPosition.y += -4.0f;//鬮｣諛ｶ・ｽ・ｽ髣包ｽｳ闕ｵ譏ｶ繝ｻ驛｢・ｧ驕擾ｽｩ雋・ｽｷ繝ｻ・ｺ繝ｻ・ｦ
			if (mvPosition.y <= 0.0f)
			{
				mvPosition.y = oldPosition.y;
			}
		}
		auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
		if (!walls.empty())
		{
			// 髯溷・萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ郢晢ｽｻ繝ｻ邇匁､ｶ遶丞､ｲ・ｽ蟶晏擅繝ｻ・ｿ驍ｵ・ｺ繝ｻ・ｹ驛｢・ｧ郢晢ｽｻ
			// hint: 髴托ｽｴ繝ｻ・ｾ髴托ｽ･繝ｻ・ｶ驍ｵ・ｺ繝ｻ・ｮ髯ｷ繝ｻ・ｽ・ｦ鬨ｾ繝ｻ繝ｻ邵ｲ蝣､・ｸ・ｺ繝ｻ・ｯ1髫ｴ・ｫ陞｢・ｹ郢晢ｽｻ髯橸ｽ｢遶丞､ｲ・ｼ・ｽ驍ｵ・ｺ陋ｹ・ｺ隲､蜥弱♀郢ｧ迚咎｣ｭ驍ｵ・ｺ繝ｻ・ｫ髯具ｽｻ繝ｻ・､髯橸ｽｳ陞｢・ｹ繝ｻ繝ｻ・ｹ・ｧ陟募ｨｯ繝ｻ驍ｵ・ｺ郢晢ｽｻ郢晢ｽｻ驍ｵ・ｺ繝ｻ・ｧ驍ｵ・ｲ郢晢ｽｻ髫ｴ・ｫ陞滂ｽｲ繝ｻ・ｼ闔蛹・ｽｽ・ｻ繝ｻ・･髣包ｽｳ陞ゅ・・ｽ・ｼ陝ｲ・ｨ遶頑･｢・ｽ蜀ｶ萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ郢晢ｽｻ隨ｳ繝ｻ謦ｻ繝ｻ・ｴ髯ｷ・ｷ陋ｹ・ｻ郢晢ｽｻ髯ｷ繝ｻ・ｽ・ｦ鬨ｾ繝ｻ繝ｻ繝ｻ蟶晁箔郢晢ｽｻ遶擾ｽｴ驛｢・ｧ郢晢ｽｻ
			for (int i = 0; i < walls.size(); i++)
			{
				Wall* wall = dynamic_cast<Wall*>(walls.at(i));
				if (wall != nullptr)
				{
					std::vector<VERTEX3D> vertex = wall->GetVertex();

					// 驛｢譎丞ｹｲ・取ｨ抵ｽｹ・ｧ繝ｻ・､驛｢譎｢・ｽ・､驛｢譎｢・ｽ・ｼ驛｢・ｧ髮区ｨ奇ｽ｡竏ｫ・ｹ・ｧ驛｢・ｧ陋ｹ・ｻ遶包ｽｧ驍ｵ・ｺ繝ｻ・ｪ驛｢・ｧ繝ｻ・ｫ驛｢譎丞ｹｲ邵ｺ譎会ｽｹ譎｢・ｽ・ｫ髯懷姓・ｹ譏ｴ繝ｻ髯具ｽｻ繝ｻ・､髯橸ｽｳ陞｢・ｹ遶雁､・ｸ・ｲ遶乗劼・ｽ・｣遶丞｣ｹ繝ｻ髣包ｽｳ髣・ｽｽ繝ｻ・ｧ髮区ｩｸ・ｽ・ｽ繝ｻ・｢驛｢譎・ｺ｢・取㏍・ｹ・ｧ繝ｻ・ｴ驛｢譎｢・ｽ・ｳ驍ｵ・ｺ繝ｻ・ｨ驍ｵ・ｺ繝ｻ・ｮ髯溷・萓ｭ隨ｳ繝ｻ・ｹ・ｧ鬮ｮ竏ｵ繝ｻ髯橸ｽｳ陞｢・ｹ繝ｻ蟶晏距陟募ｨｯ魘ｬ
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



						// 髯橸ｽ｢遶丞｣ｺ繝ｻ髮取腸・ｽ・ｿ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ鬮ｯ・ｦ陟暮ｯ会ｽｿ・･驛｢・ｧ陋ｹ・ｻ遶包ｽｧ驍ｵ・ｺ繝ｻ・ｪ驛｢譎冗函邵ｺ驢搾ｽｹ譎冗樟・取刮・ｹ・ｧ髮区ｧｫ蠕宣辧霈斐・
						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 髯橸ｽ｢遶擾ｽｵ繝ｻ・ｲ繝ｻ・ｿ驍ｵ・ｺ郢晢ｽｻ郢晢ｽｻ驛｢・ｧ繝ｻ・ｯ驛｢譎冗樟・弱・
						float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 鬩募∞・ｽ・ｻ髯ｷ讎奇ｽ｢轣假ｽｩ・ｿ髯ｷ・ｷ闔会ｽ｣郢晢ｽｻ驛｢・ｧ繝ｻ・ｯ驛｢譎冗樟・取刮・ｸ・ｺ繝ｻ・ｮ髯ｷ・ｿ隶主･・ｽｽ・ｯ繝ｻ・ｾ驛｢譎冗函邵ｺ驢搾ｽｹ譎冗樟・取刮・ｸ・ｺ繝ｻ・ｨ驍ｵ・ｲ遶乗劼・ｽ・｣遶丞｣ｹ繝ｻ髮主｢薙・繝ｻ・ｷ陞｢・ｹ遶雁､・ｸ・ｺ繝ｻ・ｮ髯ｷﾂ郢晢ｽｻ繝ｻ・ｩ鬮ｦ・ｪ繝ｻ螳夲ｽｱ蠑ｱ・・・竏ｫ・ｹ・ｧ郢晢ｽｻ
						slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 髯橸ｽ｢遶擾ｽｵ繝ｻ・ｲ繝ｻ・ｿ驍ｵ・ｺ郢晢ｽｻ郢晢ｽｻ驛｢・ｧ繝ｻ・ｯ驛｢譎冗樟・取刮・ｹ・ｧ陞ｳ螟ｲ・ｽ・ｨ髢ｧ・ｲ繝ｻ・ｮ郢晢ｽｻ

						// 髣包ｽｳ髫ｴ魃会ｽｽ・ｦ髯溯ｶ｣・ｽ・ｧ髫ｶ轣倡函繝ｻ螳夲ｽｬ魃会ｽｽ・ｻ驍ｵ・ｺ陷会ｽｱ遯ｶ・ｻ驍ｵ・ｺ闕ｵ譎｢・ｽ闃ｽ讀ｶ遶丞｣ｺ繝ｻ髮取腸・ｽ・ｿ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ髮弱・・ｽ・ｩ驍ｵ・ｺ闕ｳ螂・ｽｽ閧ｲ・ｸ・ｺ郢晢ｽｻ遶企豪・ｸ・ｺ陷ｷ・ｶ繝ｻ繝ｻ
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
	//髴托ｽｴ繝ｻ・ｾ髯懶ｽｨ繝ｻ・ｨ驍ｵ・ｺ繝ｻ・ｮ髯懃軸・ｫ繝ｻ・ｽ・ｻ繝ｻ・｢髯区ｻゑｽｽ・､驍ｵ・ｺ闕ｵ譎｢・ｽ陋ｾﾂ・ｶ繝ｻ・ｮ髫ｶ轣倡函郢晢ｽｻ髯懃軸・ｫ繝ｻ・ｽ・ｻ繝ｻ・｢髯区ｻゑｽｽ・､驍ｵ・ｺ繝ｻ・ｮ髯晢ｽｾ繝ｻ・ｮ髯具ｽｻ郢晢ｽｻ繝ｻ螳夲ｽｱ蠑ｱ・・・竏ｫ・ｹ・ｧ郢晢ｽｻ
	float subAngle = mfTargetAngle - mfAngle;

	//驍ｵ・ｺ郢ｧ繝ｻ・ｽ邇厄ｽｭ繝ｻ・ｽ・ｹ髯ｷ・ｷ闔会ｽ｣・ゑｽｰ驛｢・ｧ陝ｲ・ｨ遶包ｿｽ驛｢・ｧ陋ｹ・ｺ陝・ｿ髯ｷ・ｷ闔会ｽ｣郢晢ｽｻ髯晢ｽｾ繝ｻ・ｮ驍ｵ・ｺ鬲・ｼ夲ｽｽ・ｼ隰・∞・ｽ・ｼ陋帙・・ｽ・ｼ闔牙遜・ｽ・ｺ繝ｻ・ｦ髣比ｼ夲ｽｽ・･髣包ｽｳ陞ゅ・・ｽ・ｼ闔蛹・ｽｽ・ｻ繝ｻ・･髣包ｽｳ陷茨ｽｷ繝ｻ・ｼ陝ｲ・ｨ遶企豪・ｸ・ｺ繝ｻ・ｪ驛｢・ｧ闕ｵ譎｢・ｼ繝ｻ・ｸ・ｺ繝ｻ・ｨ驍ｵ・ｺ陟募ｨｯ繝ｻ驍ｵ・ｺ郢晢ｽｻ郢晢ｽｻ驍ｵ・ｺ陞｢・ｹ遶企・・ｸ・ｺ繝ｻ・ｮ驍ｵ・ｺ繝ｻ・ｧ
	//髯晢ｽｾ繝ｻ・ｮ驍ｵ・ｺ繝ｻ・ｮ髯区ｻゑｽｽ・､驍ｵ・ｺ鬲・ｼ夲ｽｽ・ｼ隰・∞・ｽ・ｼ陋帙・・ｽ・ｼ髯應ｼ夲ｽｽ・ｻ繝ｻ・･髣包ｽｳ陞ゅ・・ｽ・ｼ闔蛹・ｽｽ・ｻ繝ｻ・･髣包ｽｳ陷茨ｽｷ繝ｻ・ｼ陝ｲ・ｨ遶企豪・ｸ・ｺ繝ｻ・ｪ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ郢晢ｽｻ隨ｳ繝ｻ・ｹ・ｧ髢ｾ・･雎ｬ・｣髮弱・・ｽ・｣驍ｵ・ｺ陷ｷ・ｶ繝ｻ繝ｻ
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//鬮ｫ闍難ｽｲ讖ｸ・ｽ・ｺ繝ｻ・ｦ驍ｵ・ｺ繝ｻ・ｮ髯晢ｽｾ繝ｻ・ｮ髯具ｽｻ郢晢ｽｻ繝ｻ螳夲ｿｽ謌翫・郢晢ｽｻ遶願侭繝ｻ髣雁ｨｯ繝ｻ鬮ｴ蜿ｰ・ｻ・｣隨・ｽｼ驍ｵ・ｺ闔会ｽ｣繝ｻ繝ｻ
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
	//髣碑崟螳ｦ鬯ｮ繝ｻ・ｸ・ｺ郢晢ｽｻ遯ｶ・ｻ驍ｵ・ｺ繝ｻ・ｻ驍ｵ・ｺ陷会ｽｱ繝ｻ讓｣蝗馴寞讖ｸ・ｽ・ｺ繝ｻ・ｦ驛｢・ｧ陜｣・､髢ｧ繝ｻ諤弱・・ｺ
	mfAngle = mfTargetAngle - subAngle;

	//髯懃軸・ｫ繝ｻ・ｽ・ｻ繝ｻ・｢髯区ｻゑｽｽ・､驛｢・ｧ陞ｳ螟ｲ・ｽ・ｨ繝ｻ・ｭ髯橸ｽｳ郢晢ｽｻ
	mvRotation.y = mfAngle + DX_PI_F;
	//驛｢譎｢・ｽ・｢驛｢譏ｴ繝ｻ・取刮・ｸ・ｺ繝ｻ・ｫ髣費ｽｨ隴擾ｽｴ遶擾ｽｴ驛｢・ｧ郢晢ｽｻ
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
	//驛｢譎｢・ｽ・ｫ驛｢譎｢・ｽ・ｼ驛｢譎丞ｹｲ郢晢ｽｻ驍ｵ・ｺ髴域喚髮ｷ驍ｵ・ｺ繝ｻ・ｪ驍ｵ・ｺ郢晢ｽｻ
	mpModel->SetLoop(false);
	//驛｢譎｢・ｽ・｢驛｢譎｢・ｽ・ｼ驛｢・ｧ繝ｻ・ｷ驛｢譎｢・ｽ・ｧ驛｢譎｢・ｽ・ｳ髯溷供・ｾ蠕後・髯溯ｼ斐・繝ｻ・ｩ雋・ｽｩ逧ｮ・ｹ譎｢・ｽ・ｼ驛｢・ｧ繝ｻ・ｷ驛｢譎｢・ｽ・ｧ驛｢譎｢・ｽ・ｳ驍ｵ・ｺ繝ｻ・ｫ髫ｰ魃会ｽｽ・ｻ驍ｵ・ｺ郢晢ｽｻ
	//驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢譏ｴ繝ｻ郢晢ｽｻ驛｢・ｧ繝ｻ・ｼ驛｢譎｢・ｽ・ｧ驛｢譏ｴ繝ｻ郢晢ｽｻ髯溯ｼ斐・郢晢ｽｻ髯溯ｼ斐・繝ｻ・ｩ雋・ｽｩ逧ｮ・ｹ譏ｴ繝ｻ郢晢ｽｻ驛｢・ｧ繝ｻ・ｼ驛｢譎｢・ｽ・ｧ驛｢譏ｴ繝ｻ郢晢ｽｻ驍ｵ・ｺ繝ｻ・ｫ髫ｰ魃会ｽｽ・ｻ驍ｵ・ｺ郢晢ｽｻ
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

void Enemy::OnEnter(Collider* collider, Collider* check)//髯晢ｽｾ繝ｻ・ｦ髯句ｹ｢・ｽ・ｴ.
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
			isHitAttackSearch = true;//髫ｰ・ｾ繝ｻ・ｻ髫ｰ・ｦ郢晢ｽｻ陝ｷ謌頑ｲり嵯譏ｴﾎｨ驛｢譎｢・ｽ・ｩ驛｢・ｧ繝ｻ・ｰ

		}
		if (collider == mpStopCollider && player->GetCollisionCollider() == check)
		{
			isHitSerchStop = true;
		}
	}
	//髯溷・萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ郢晢ｽｻ繝ｻ遏ｩ・ｫ・｢隰撰ｽｺ陷・ｽｾ驍ｵ・ｺ繝ｻ・ｮ髯ｷ繝ｻ・ｽ・ｦ鬨ｾ繝ｻ繝ｻ
}

void Enemy::OnTrigger(Collider* collider, Collider* check)
{//髯溷・萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ雋・ｽｽ隶・ｪ鬯ｮ・｢髦ｮ蜷ｶ繝ｻ髯ｷ繝ｻ・ｽ・ｦ鬨ｾ繝ｻ繝ｻ
	if (mfHp <= 0)return; auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	AnimationState now = mpModel->GetNowState();
	if (collider == mpAttachCollider && check->mpParentObject->GetTag() == Tag3D_Player3D)
	{//mpModel驍ｵ・ｺ繝ｻ・ｮ鬨ｾ・｡繝ｻ・ｪ髯ｷ・ｿ繝ｻ・ｷ驛｢・ｧ郢ｧ繝ｻ譌ｺ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驍ｵ・ｺ郢晢ｽｻ繝ｻ迢暦ｽｸ・ｲ遶丞､ゑｽｸ・ｺ繝ｻ・ｩ驍ｵ・ｺ髦ｮ蜷ｶ繝ｻif髫ｴ竏壹・遶頑･｢諤ｦ繝ｻ・･驛｢・ｧ陝ｲ・ｨ遶企・・ｸ・ｺ郢晢ｽｻ
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr)return;
		if (check == player->GetCollisionCollider())
		{
			if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
			{

				player->Damage(mfAttack);
				AttackHitJudgmentflag = true;//髯溷・萓ｭ隨ｳ繝ｻ・ｸ・ｺ繝ｻ・｣驍ｵ・ｺ雋・∞・ｽ閧ｲ・ｹ譎｢・ｽ・ｼ
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
				isHitAttackSearch = false;//髫ｰ・ｾ繝ｻ・ｻ髫ｰ・ｦ郢晢ｽｻ陝ｷ謌頑ｲり嵯譏ｴﾎｨ驛｢譎｢・ｽ・ｩ驛｢・ｧ繝ｻ・ｰ

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
