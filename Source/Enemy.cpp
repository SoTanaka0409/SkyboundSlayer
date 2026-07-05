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
	, hp_(hp)
	, speed_(speed)
	, mbInvisible(false)
	, mfAngle(0.0f)
	, mfTargetAngle(0.0f)
	, mfSize(HitSize)
	, HitSerch(Serch1)
	, HitAttackSerch(Serch2)
	, HitStopSerch(Serch3)
	, mfHaveXp(xp)
	, AlgHit(20)
	, attack_(attack)
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
	, max_hp_(hp)
	,mfHaveMoney(money)
	//,Animation(false)
{
	SetTag(Object3D::Tag3D_Enemy3D);
	//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・｢鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ繝ｻ蜿門旭繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬯ｨ・ｾ陟・屮・ｽ・ｻ髦ｮ蜷ｶ繝ｻ
	model_ = new Model(filename, initPos, isSeparateAnim);
	model_->SetScale(VGet(1.3f, 1.3f, 1.3f));
	VinitPos = initPos;
	max_hp_ = hp_;
	mfNormalSpeed = speed_;

	mpCapsuleCollider = new CapsuleCollider(this, position_, VAdd(position_, VGet(0.0f, mfSize/2, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, model_->GetAttachmentPosition(), 50.0f);
	mpSerchCollider = new SphereCollider(this, position_, HitSerch);//鬮ｫ・ｰ繝ｻ・ｨ郢晢ｽｻ繝ｻ・ｵ鬩幢ｽ｢繝ｻ・ｧ髯ｷ・ｻ髢ｧ・ｲ驍頑ｻ・ｽｩ蠅灘ｾ励・・ｽ繝ｻ・･鬩搾ｽｵ繝ｻ・ｺ髯ｷ・ｷ繝ｻ・ｶ郢晢ｽｻ驍・戟ﾂ蜥擾ｽｹ譎｢・ｽ・ｻ
	mpAttackCollider = new SphereCollider(this, position_, HitAttackSerch);//鬮ｫ・ｰ繝ｻ・ｨ郢晢ｽｻ繝ｻ・ｵ鬩幢ｽ｢繝ｻ・ｧ髯ｷ・ｻ闔�・･隲｢蟷・梭繝ｻ・ｦ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｽ鬮ｯ譎渉・ｶ繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ遶擾ｽｫ繝ｻ・ｹ繝ｻ・ｧ髯ｷ・ｿ繝ｻ・･驛｢譎｢・ｽ・ｻ
	mpStopCollider = new SphereCollider(this, position_, HitStopSerch);//鬮ｫ・ｰ繝ｻ・ｨ郢晢ｽｻ繝ｻ・ｵ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｨ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬯ｮ・ｴ髢ｧ・ｴ髴趣ｽｨ髯橸ｽｻ繝ｻ・ｬ鬩幢ｽ｢繝ｻ・ｧ髯ｷ莉｣繝ｻ繝ｻ・ｽ繝ｻ・ｿ髫ｴ謫ｾ・ｽ・ｴ髫ｨ繝ｻ・ｽ・ｽ鬮ｯ・ｷ・つ驛｢譎｢・ｽ・ｻ
	//new DrawHp("",  //鬯ｨ・ｾ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｻ鬮ｯ・ｷ陷代・・ｽ・ｸ隶難ｽ｣陋滂ｽｹ
	//	(VAdd(position_, VGet(0.0f, 100.0f, 0.0f))),//鬮｣蛹・ｽｽ・ｳ郢晢ｽｻ繝ｻ・ｭ鬮ｯ貊ゑｽｽ・｢驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｧ鬮ｫ・ｶ髦ｮ蜷ｶ繝ｻ
	  // 鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｳ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｩ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・､鬩幢ｽ｢隴惹ｼ夲ｽｽ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬯ｨ・ｾ陟・屮・ｽ・ｻ髦ｮ蜷ｶ繝ｻ
	
	
}

Enemy::~Enemy()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
	
}

void Enemy::Update()
{
	
		
	
	
		if (model_ != nullptr)
		{
			DeathEnemy();//鬮ｫ・ｰ繝ｻ・ｨ郢晢ｽｻ繝ｻ・ｵ鬩幢ｽ｢繝ｻ・ｧ鬮ｮ蛹ｺ・ｧ・ｫ郢晢ｽｻ郢晢ｽｻ繝ｻ・ｽ鬩搾ｽｵ繝ｻ・ｺ髮九・ﾂ・ｪ郢晢ｽｻ鬩搾ｽｵ繝ｻ・ｺ鬯ｮ・ｦ繝ｻ・ｪ驕ｶ莨∬ｱｪ繝ｻ・ｹ繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｯ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｪ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・｢鬩搾ｽｵ繝ｻ・ｺ髯ｷ・ｷ繝ｻ・ｶ郢晢ｽｻ霑｢證ｦ・ｽ・ｸ繝ｻ・ｺ髮九・竏槭・・ｽ遶擾ｽｫ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｫ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｦ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬩幢ｽ｢隴主・讓溽ｹ晢ｽｻ陞ｳ螟ｲ・ｽ・ｲ郢ｧ繝ｻ・ｽ・ｸ陷ｻ・ｻ繝ｻ・ｽ髢ｾ・･繝ｻ・ｸ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
			Attack();
			CollPositionUpdate();
			RotationByMove();
			Move();
			/////鬮ｯ貊捺ｱ壹・・ｽ繝ｻ・ｾm鬩幢ｽ｢繝ｻ・ｧ髯ｷ・ｻ騾趣ｽｯ隨・ｽ｡鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・､鬮ｯ・ｷ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｦ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ
		
			model_->Update();
			//mpDH->Update();//drawHp鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・｢鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴主・讓溽ｹ晢ｽｻ陞ｳ螢ｽﾎ､郢晢ｽｻ繝ｻ・ｼ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｶ
			
		}

	
}

void Enemy::Draw()
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

void Enemy::AttackList()
{
	

}

void Enemy::Attack()
{
	AnimationState now = model_->GetNowState();
	
	if (AttackCount >= AttackInterval&& isHitAttackSearch)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		AttackCount = 0;
		//鬮ｫ・ｰ繝ｻ・ｾ郢晢ｽｻ繝ｻ・ｻ鬮ｫ・ｰ繝ｻ・ｦ驛｢譎｢・ｽ・ｻ繝ｻ螳亥擠繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮ｯ讓奇ｽｺ・ｽ陋ｻ・､髯晢ｽｲ繝ｻ・ｩ
		model_->ChangeAnimation(ANIMATION_ATTACK);
		//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｫ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驛｢譎｢・ｽ・ｻ鬩搾ｽｵ繝ｻ・ｺ鬮ｴ蝓溷繭鬮ｮ・ｷ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
		model_->SetLoop(false);
		//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・｢鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬮ｯ貅ｷ萓帙・・ｾ陟募ｾ後・鬮ｯ貅ｯ・ｼ譁舌・郢晢ｽｻ繝ｻ・ｩ髮九・・ｽ・ｩ騾ｧ・ｮ繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮ｫ・ｰ鬲・ｼ夲ｽｽ・ｽ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);
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
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK||now==ANIMATION_ATTACKMAGIC||now==ANIMATION_ATTACKJUMP)return;
	if (Master::SafePointOn)position_ = VinitPos;//鬩幢ｽ｢繝ｻ・ｧ驛｢・ｧ郢晢ｽｻ繝ｻ・ｼ繝ｻ・ｽ鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ繝ｻ蜿厄ｽｨ謚ｵ・ｽ・ｹ繝ｻ・ｧ郢晢ｽｻ繝ｻ・､鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・､鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩搾ｽｵ繝ｻ・ｺ髫ｰ逍ｲ・ｻ繧托ｽｽ・ｽ繝ｻ・ｮ髴大｣ｼ逕溽ｹ晢ｽｻ鬮ｯ諛ｶ・ｽ・ｨ郢晢ｽｻ繝ｻ・ｰ鬮ｯ譎｢・ｽ・ｶ郢晢ｽｻ繝ｻ・ｯ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ霑｢證ｦ・ｽ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩幢ｽ｢繝ｻ・ｧ髴大｣ｼ逕溽ｹ晢ｽｻ鬮ｫ・ｴ陝ｶ・ｶ繝ｻ・ｺ陋滂ｽ･郢晢ｽｻ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｹ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮ｫ・ｰ鬲・ｼ夲ｽｽ・ｽ繝ｻ・ｻ鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ
	

	auto mpPlayer = Master::mpPlayer;
	Player3D* pPlayer = Master::mpPlayer;

	moveVec = VGet(0.0f, 0.0f, 0.0f);
	
	{
		if (isHitSerchStop) { speed_ = 0; } else { speed_ = mfNormalSpeed; }
		
		GoPosition = VSub(pPlayer->GetPosition(), position_);
		GoPosition.y = 0.0f; // Fix Y axis movement
		if (VSquareSize(GoPosition) > 0.0001f) GoPosition = VNorm(GoPosition);
		moveVec = GoPosition;

		bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
		if (isMove) { model_->ChangeAnimation(ANIMATION_RUN); mfTargetAngle = atan2f(moveVec.x, moveVec.z); }
		else { model_->ChangeAnimation(ANIMATION_NEUTRAL); }

		VECTOR oldPosition = position_;
		position_ = VAdd(position_, VScale(moveVec, speed_));
		//////////////////////////////////////////////////////

		VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
		//鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｹ鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｸ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｨ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ貅ｷ繝ｻ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｹ繝ｻ・ｧ鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢繝ｻ・ｹ郢晢ｽｻ陜｣・､繝ｻ・ｸ繝ｻ・ｺ髯ｷ・ｷ繝ｻ・ｶ郢晢ｽｻ郢晢ｽｻ
		hitPos = VGet(0.0f, 0.0f, 0.0f);
		bool isHit = false;
		// 地形（ステージ）に沿う処理
		TerrainFollow(0.0f, 150.0f, 40.0f, 150.0f, -40.0f, 4.0f);
		auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
		if (!walls.empty())
		{
			// 鬮ｯ貅ｷ繝ｻ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ驍・戟・､・ｶ驕ｶ荳橸ｽ､・ｲ繝ｻ・ｽ陝ｶ譎乗套郢晢ｽｻ繝ｻ・ｿ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｹ鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ
			// hint: 鬮ｴ謇假ｽｽ・ｴ郢晢ｽｻ繝ｻ・ｾ鬮ｴ謇假ｽｽ・･郢晢ｽｻ繝ｻ・ｶ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ・ｷ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｦ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ驍ｵ・ｲ陜｣・､繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｯ1鬮ｫ・ｴ繝ｻ・ｫ髯橸ｽ｢繝ｻ・ｹ驛｢譎｢・ｽ・ｻ鬮ｯ讖ｸ・ｽ・｢驕ｶ荳橸ｽ､・ｲ繝ｻ・ｼ繝ｻ・ｽ鬩搾ｽｵ繝ｻ・ｺ髯具ｽｹ繝ｻ・ｺ髫ｲ・､陷･蠑ｱ笙驛｢・ｧ霑壼虫・｣・ｭ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮ｯ蜈ｷ・ｽ・ｻ郢晢ｽｻ繝ｻ・､鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢繝ｻ・ｹ郢晢ｽｻ郢晢ｽｻ繝ｻ・ｹ繝ｻ・ｧ髯溷供・ｨ・ｯ郢晢ｽｻ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｧ鬩搾ｽｵ繝ｻ・ｲ驛｢譎｢・ｽ・ｻ鬮ｫ・ｴ繝ｻ・ｫ髯樊ｻゑｽｽ・ｲ郢晢ｽｻ繝ｻ・ｼ髣費｣ｰ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｻ郢晢ｽｻ繝ｻ・･鬮｣蛹・ｽｽ・ｳ髯槭ｅ繝ｻ繝ｻ・ｽ繝ｻ・ｼ髯晢ｽｲ繝ｻ・ｨ驕ｶ鬆托ｽ･・｢繝ｻ・ｽ陷・ｶ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ髫ｨ・ｳ郢晢ｽｻ隰ｦ・ｻ郢晢ｽｻ繝ｻ・ｴ鬮ｯ・ｷ繝ｻ・ｷ髯具ｽｹ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ鬮ｯ・ｷ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｦ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ郢晢ｽｻ陝ｶ譎∫ｮ秘Δ譎｢・ｽ・ｻ驕ｶ謫ｾ・ｽ・ｴ鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ
			for (int i = 0; i < walls.size(); i++)
			{
				Wall* wall = walls.at(i)->CastTo<Wall>();
				if (wall != nullptr)
				{
					std::vector<VERTEX3D> vertex = wall->GetVertex();

					// 鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ繝ｻ蜿厄ｽｨ謚ｵ・ｽ・ｹ繝ｻ・ｧ郢晢ｽｻ繝ｻ・､鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・､鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧ鬮ｮ蛹ｺ・ｨ螂・ｽｽ・｡遶擾ｽｫ繝ｻ・ｹ繝ｻ・ｧ鬩幢ｽ｢繝ｻ・ｧ髯具ｽｹ繝ｻ・ｻ驕ｶ蛹・ｽｽ・ｧ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｫ鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驍ｵ・ｺ隴惹ｼ夲ｽｽ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｫ鬮ｯ諛ｷ蟋薙・・ｹ隴擾ｽｴ郢晢ｽｻ鬮ｯ蜈ｷ・ｽ・ｻ郢晢ｽｻ繝ｻ・､鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢繝ｻ・ｹ驕ｶ髮・｣ｰ・､繝ｻ・ｸ繝ｻ・ｲ驕ｶ荵怜款繝ｻ・ｽ繝ｻ・｣驕ｶ荳橸ｽ｣・ｹ郢晢ｽｻ鬮｣蛹・ｽｽ・ｳ鬮｣繝ｻ・ｽ・ｽ郢晢ｽｻ繝ｻ・ｧ鬮ｮ蛹ｺ・ｩ・ｸ繝ｻ・ｽ繝ｻ・ｽ郢晢ｽｻ繝ｻ・｢鬩幢ｽ｢隴弱・・ｺ・｢繝ｻ蜿悶渚繝ｻ・ｹ繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｴ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｨ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ貅ｷ繝ｻ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｹ繝ｻ・ｧ鬯ｮ・ｮ遶擾ｽｵ郢晢ｽｻ鬮ｯ讖ｸ・ｽ・ｳ髯橸ｽ｢繝ｻ・ｹ郢晢ｽｻ陝ｶ譎剰ｷ晞辧蜍滂ｽｨ・ｯ鬲假ｽｬ
					if (HitCheck_Capsule_Triangle(
						position_,
						VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
						HitCheck_Capsule_Triangle(
							position_,
							VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
							80.0f,
							vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
						)
					{



						// 鬮ｯ讖ｸ・ｽ・｢驕ｶ荳橸ｽ｣・ｺ郢晢ｽｻ鬮ｮ蜿冶・繝ｻ・ｽ繝ｻ・ｿ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬯ｮ・ｯ繝ｻ・ｦ髯滓坩・ｯ莨夲ｽｽ・ｿ繝ｻ・･鬩幢ｽ｢繝ｻ・ｧ髯具ｽｹ繝ｻ・ｻ驕ｶ蛹・ｽｽ・ｧ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩幢ｽ｢隴主・蜃ｽ驍ｵ・ｺ鬩｢謳ｾ・ｽ・ｹ隴主・讓溘・蜿門旭繝ｻ・ｹ繝ｻ・ｧ鬮ｮ蛹ｺ・ｧ・ｫ陟募ｮ｣霎ｧ髴域鱒繝ｻ
						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 鬮ｯ讖ｸ・ｽ・｢驕ｶ謫ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｲ郢晢ｽｻ繝ｻ・ｿ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｯ鬩幢ｽ｢隴主・讓溘・蠑ｱ繝ｻ
						float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 鬯ｩ蜍溪・繝ｻ・ｽ繝ｻ・ｻ鬮ｯ・ｷ隶主･・ｽｽ・｢霓｣蛛・ｽｽ・ｩ繝ｻ・ｿ鬮ｯ・ｷ繝ｻ・ｷ髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｯ鬩幢ｽ｢隴主・讓溘・蜿門旭繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ・ｷ繝ｻ・ｿ髫ｶ荳ｻ・･繝ｻ・ｽ・ｽ繝ｻ・ｯ郢晢ｽｻ繝ｻ・ｾ鬩幢ｽ｢隴主・蜃ｽ驍ｵ・ｺ鬩｢謳ｾ・ｽ・ｹ隴主・讓溘・蜿門旭繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｨ鬩搾ｽｵ繝ｻ・ｲ驕ｶ荵怜款繝ｻ・ｽ繝ｻ・｣驕ｶ荳橸ｽ｣・ｹ郢晢ｽｻ鬮ｮ荳ｻ・｢阮吶・郢晢ｽｻ繝ｻ・ｷ髯橸ｽ｢繝ｻ・ｹ驕ｶ髮・｣ｰ・､繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ・ｷ・つ驛｢譎｢・ｽ・ｻ郢晢ｽｻ繝ｻ・ｩ鬯ｮ・ｦ繝ｻ・ｪ郢晢ｽｻ陞ｳ螟ｲ・ｽ・ｱ陟托ｽｱ繝ｻ繝ｻ繝ｻ遶擾ｽｫ繝ｻ・ｹ繝ｻ・ｧ驛｢譎｢・ｽ・ｻ
						slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 鬮ｯ讖ｸ・ｽ・｢驕ｶ謫ｾ・ｽ・ｵ郢晢ｽｻ繝ｻ・ｲ郢晢ｽｻ繝ｻ・ｿ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｯ鬩幢ｽ｢隴主・讓溘・蜿門旭繝ｻ・ｹ繝ｻ・ｧ髯橸ｽｳ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｨ鬮｢・ｧ繝ｻ・ｲ郢晢ｽｻ繝ｻ・ｮ驛｢譎｢・ｽ・ｻ

						// 鬮｣蛹・ｽｽ・ｳ鬮ｫ・ｴ鬲・ｼ夲ｽｽ・ｽ繝ｻ・ｦ鬮ｯ貅ｯ・ｶ・｣繝ｻ・ｽ繝ｻ・ｧ鬮ｫ・ｶ霓｣蛟｡蜃ｽ郢晢ｽｻ陞ｳ螟ｲ・ｽ・ｬ鬲・ｼ夲ｽｽ・ｽ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ髣包ｽｵ隴趣ｽ｢繝ｻ・ｽ髣・ｽｽ隶・ｶ驕ｶ荳橸ｽ｣・ｺ郢晢ｽｻ鬮ｮ蜿冶・繝ｻ・ｽ繝ｻ・ｿ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬮ｮ蠑ｱ繝ｻ繝ｻ・ｽ繝ｻ・ｩ鬩搾ｽｵ繝ｻ・ｺ髣包ｽｳ陞ゅ・・ｽ・ｽ髢ｧ・ｲ繝ｻ・ｸ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ驕ｶ莨∬ｱｪ繝ｻ・ｸ繝ｻ・ｺ髯ｷ・ｷ繝ｻ・ｶ郢晢ｽｻ郢晢ｽｻ
						position_ = oldPosition;
						position_ = VAdd(position_, VScale(slide, speed_));
					}
				}
			}
		}




		model_->SetPosition(position_);


	}
}

void Enemy::RotationByMove()
{
	//鬮ｴ謇假ｽｽ・ｴ郢晢ｽｻ繝ｻ・ｾ鬮ｯ諛ｶ・ｽ・ｨ郢晢ｽｻ繝ｻ・ｨ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ諛・ｻｸ繝ｻ・ｫ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｻ郢晢ｽｻ繝ｻ・｢鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽ繝ｻ・､鬩搾ｽｵ繝ｻ・ｺ髣包ｽｵ隴趣ｽ｢繝ｻ・ｽ髯具ｽｾ・つ繝ｻ・ｶ郢晢ｽｻ繝ｻ・ｮ鬮ｫ・ｶ霓｣蛟｡蜃ｽ驛｢譎｢・ｽ・ｻ鬮ｯ諛・ｻｸ繝ｻ・ｫ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｻ郢晢ｽｻ繝ｻ・｢鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽ繝ｻ・､鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻ繝ｻ・ｮ鬮ｯ蜈ｷ・ｽ・ｻ驛｢譎｢・ｽ・ｻ郢晢ｽｻ陞ｳ螟ｲ・ｽ・ｱ陟托ｽｱ繝ｻ繝ｻ繝ｻ遶擾ｽｫ繝ｻ・ｹ繝ｻ・ｧ驛｢譎｢・ｽ・ｻ
	float subAngle = mfTargetAngle - mfAngle;

	//鬩搾ｽｵ繝ｻ・ｺ驛｢・ｧ郢晢ｽｻ繝ｻ・ｽ驍・私・ｽ・ｭ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｹ鬮ｯ・ｷ繝ｻ・ｷ髣比ｼ夲ｽｽ・｣繝ｻ繧托ｽｽ・ｰ鬩幢ｽ｢繝ｻ・ｧ髯晢ｽｲ繝ｻ・ｨ驕ｶ蛹・ｽｿ・ｽ鬩幢ｽ｢繝ｻ・ｧ髯具ｽｹ繝ｻ・ｺ髯敖繝ｻ・ｿ鬮ｯ・ｷ繝ｻ・ｷ髣比ｼ夲ｽｽ・｣驛｢譎｢・ｽ・ｻ鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻ繝ｻ・ｮ鬩搾ｽｵ繝ｻ・ｺ鬯ｲ繝ｻ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｼ髫ｰ繝ｻ竏槭・・ｽ繝ｻ・ｼ髯句ｸ吶・繝ｻ・ｽ繝ｻ・ｼ髣皮甥驕懊・・ｽ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・･鬮｣蛹・ｽｽ・ｳ髯槭ｅ繝ｻ繝ｻ・ｽ繝ｻ・ｼ髣費｣ｰ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｻ郢晢ｽｻ繝ｻ・･鬮｣蛹・ｽｽ・ｳ髯ｷ闌ｨ・ｽ・ｷ郢晢ｽｻ繝ｻ・ｼ髯晢ｽｲ繝ｻ・ｨ驕ｶ莨∬ｱｪ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩幢ｽ｢繝ｻ・ｧ髣包ｽｵ隴趣ｽ｢繝ｻ・ｼ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｨ鬩搾ｽｵ繝ｻ・ｺ髯溷供・ｨ・ｯ郢晢ｽｻ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵ繝ｻ・ｺ髯橸ｽ｢繝ｻ・ｹ驕ｶ莨√・繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｧ
	//鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻ繝ｻ・ｮ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽ繝ｻ・､鬩搾ｽｵ繝ｻ・ｺ鬯ｲ繝ｻ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｼ髫ｰ繝ｻ竏槭・・ｽ繝ｻ・ｼ髯句ｸ吶・繝ｻ・ｽ繝ｻ・ｼ鬮ｯ諛会ｽｼ螟ｲ・ｽ・ｽ繝ｻ・ｻ郢晢ｽｻ繝ｻ・･鬮｣蛹・ｽｽ・ｳ髯槭ｅ繝ｻ繝ｻ・ｽ繝ｻ・ｼ髣費｣ｰ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｻ郢晢ｽｻ繝ｻ・･鬮｣蛹・ｽｽ・ｳ髯ｷ闌ｨ・ｽ・ｷ郢晢ｽｻ繝ｻ・ｼ髯晢ｽｲ繝ｻ・ｨ驕ｶ莨∬ｱｪ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｹ繝ｻ・ｧ鬮｢・ｾ繝ｻ・･髮趣ｽｬ繝ｻ・｣鬮ｮ蠑ｱ繝ｻ繝ｻ・ｽ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ髯ｷ・ｷ繝ｻ・ｶ郢晢ｽｻ郢晢ｽｻ
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//鬯ｮ・ｫ髣埼屮・ｽ・ｲ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻ繝ｻ・ｮ鬮ｯ蜈ｷ・ｽ・ｻ驛｢譎｢・ｽ・ｻ郢晢ｽｻ陞ｳ螟ｲ・ｿ・ｽ隰檎ｿｫ繝ｻ驛｢譎｢・ｽ・ｻ驕ｶ鬘倅ｾｭ郢晢ｽｻ鬮｣髮・ｽｨ・ｯ郢晢ｽｻ鬯ｮ・ｴ陷ｿ・ｰ繝ｻ・ｻ繝ｻ・｣髫ｨ繝ｻ・ｽ・ｼ鬩搾ｽｵ繝ｻ・ｺ髣比ｼ夲ｽｽ・｣郢晢ｽｻ郢晢ｽｻ
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
	//鬮｣遒大ｴ溯楜・ｦ鬯ｯ・ｮ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ驕ｯ・ｶ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ髯ｷ莨夲ｽｽ・ｱ郢晢ｽｻ隶難ｽ｣陜鈴ｦｴ蟇櫁ｮ厄ｽｸ繝ｻ・ｽ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩幢ｽ｢繝ｻ・ｧ髯懶ｽ｣繝ｻ・､鬮｢・ｧ郢晢ｽｻ隲､蠑ｱ繝ｻ繝ｻ・ｺ
	mfAngle = mfTargetAngle - subAngle;

	//鬮ｯ諛・ｻｸ繝ｻ・ｫ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｻ郢晢ｽｻ繝ｻ・｢鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽ繝ｻ・､鬩幢ｽ｢繝ｻ・ｧ髯橸ｽｳ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｨ郢晢ｽｻ繝ｻ・ｭ鬮ｯ讖ｸ・ｽ・ｳ驛｢譎｢・ｽ・ｻ
	rotation_.y = mfAngle + DX_PI_F;
	//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・｢鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ繝ｻ蜿門旭繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮｣雋ｻ・ｽ・ｨ髫ｴ謫ｾ・ｽ・ｴ驕ｶ謫ｾ・ｽ・ｴ鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ
	model_->SetRotation(rotation_);


}

void Enemy::Damage(float damage)
{

	hp_ -= damage;
	if(hp_ <= 0)
	{
		hp_ = 0;
		isDead = true;
	}
}



void Enemy::DeathEnemy()
{
	if (!isDead)return;
	
	model_->ChangeAnimation(ANIMATION_DYING);
	//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｫ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢隴惹ｸ橸ｽｹ・ｲ驛｢譎｢・ｽ・ｻ鬩搾ｽｵ繝ｻ・ｺ鬮ｴ蝓溷繭鬮ｮ・ｷ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
	model_->SetLoop(false);
	//鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・｢鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬮ｯ貅ｷ萓帙・・ｾ陟募ｾ後・鬮ｯ貅ｯ・ｼ譁舌・郢晢ｽｻ繝ｻ・ｩ髮九・・ｽ・ｩ騾ｧ・ｮ繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮ｫ・ｰ鬲・ｼ夲ｽｽ・ｽ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
	//鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｼ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬮ｯ貅ｯ・ｼ譁舌・驛｢譎｢・ｽ・ｻ鬮ｯ貅ｯ・ｼ譁舌・郢晢ｽｻ繝ｻ・ｩ髮九・・ｽ・ｩ騾ｧ・ｮ繝ｻ・ｹ隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｼ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮ｫ・ｰ鬲・ｼ夲ｽｽ・ｽ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
	model_->SetLoopFinishState(ANIMATION_MAX);
	
	Delete();
	if (model_->IsAnimationLoopFinish())
	{
		GiveRewards();
		if (Master::TutorialFlag) { Master::TutorialCount++; }
		
		
		SetDeleteFlag(true);
	}
	
	model_->Update();
	
	
}

void Enemy::DeathColliderPosition()
{
	VECTOR pos = VGet(10000, 10000, 10000);
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->position_ = pos;
		mpCapsuleCollider->position2_ = pos;
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->position_ = pos;
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->position_ = pos;
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->SetDeleteFlag(true);
		mpStopCollider->position_ = pos;
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->position_ = pos;
	}

}

void Enemy::OnEnter(Collider* collider, Collider* check)//鬮ｯ譎｢・ｽ・ｾ郢晢ｽｻ繝ｻ・ｦ鬮ｯ蜿･・ｹ・｢繝ｻ・ｽ繝ｻ・ｴ.
{
	auto mpPlayer = Master::mpPlayer;
	if (hp_ <= 0)return;
	if (collider == mpCapsuleCollider && check->parent_object_->GetTag() == Tag3D_Obj)
	{
		
		position_ = VinitPos;
	}
	
	if (check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::mpPlayer;
		if (player == nullptr)return;
	
		if (collider == mpSerchCollider &&player->GetCollisionCollider()==check)
		{
			isHitSearch = true;
		}
		if (collider == mpAttackCollider && player->GetCollisionCollider() == check)
		{
			isHitAttackSearch = true;//鬮ｫ・ｰ繝ｻ・ｾ郢晢ｽｻ繝ｻ・ｻ鬮ｫ・ｰ繝ｻ・ｦ驛｢譎｢・ｽ・ｻ髯晢ｽｷ隰碁�托ｽｲ繧雁ｵｯ隴擾ｽｴ・趣ｽｨ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｩ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｰ

		}
		if (collider == mpStopCollider && player->GetCollisionCollider() == check)
		{
			isHitSerchStop = true;
		}
	}
	//鬮ｯ貅ｷ繝ｻ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ驕擾ｽｩ繝ｻ・ｫ繝ｻ・｢髫ｰ謦ｰ・ｽ・ｺ髯ｷ繝ｻ・ｽ・ｾ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬮ｯ・ｷ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｦ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ
}

void Enemy::OnTrigger(Collider* collider, Collider* check)
{//鬮ｯ貅ｷ繝ｻ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ髮九・・ｽ・ｽ髫ｶﾂ繝ｻ・ｪ鬯ｯ・ｮ繝ｻ・｢鬮ｦ・ｮ陷ｷ・ｶ郢晢ｽｻ鬮ｯ・ｷ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｦ鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ
	if (hp_ <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = model_->GetNowState();
	if (collider == mpAttachCollider && check->parent_object_->GetTag() == Tag3D_Player3D)
	{//mpModel鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬯ｨ・ｾ繝ｻ・｡郢晢ｽｻ繝ｻ・ｪ鬮ｯ・ｷ繝ｻ・ｿ郢晢ｽｻ繝ｻ・ｷ鬩幢ｽ｢繝ｻ・ｧ驛｢・ｧ郢晢ｽｻ隴鯉ｽｺ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ郢晢ｽｻ霑｢證ｦ・ｽ・ｸ繝ｻ・ｲ驕ｶ荳橸ｽ､繧托ｽｽ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｩ鬩搾ｽｵ繝ｻ・ｺ鬮ｦ・ｮ陷ｷ・ｶ郢晢ｽｻif鬮ｫ・ｴ遶丞｣ｹ繝ｻ驕ｶ鬆托ｽ･・｢隲､・ｦ郢晢ｽｻ繝ｻ・･鬩幢ｽ｢繝ｻ・ｧ髯晢ｽｲ繝ｻ・ｨ驕ｶ莨√・繝ｻ・ｸ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
		Player3D* player = Master::mpPlayer;
		if (player == nullptr)return;
		if (check == player->GetCollisionCollider())
		{
			if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
			{

				player->Damage(attack_);
				AttackHitJudgmentflag = true;//鬮ｯ貅ｷ繝ｻ關難ｽｭ髫ｨ・ｳ郢晢ｽｻ繝ｻ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ髮九・竏槭・・ｽ髢ｧ・ｲ繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ
			}
		}
	}
	
}

void Enemy::OnExit(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return; auto mpPlayer = Master::mpPlayer;
	AnimationState now = model_->GetNowState();
	if (check->parent_object_->GetTag() == Tag3D_Player3D)
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
				isHitAttackSearch = false;//鬮ｫ・ｰ繝ｻ・ｾ郢晢ｽｻ繝ｻ・ｻ鬮ｫ・ｰ繝ｻ・ｦ驛｢譎｢・ｽ・ｻ髯晢ｽｷ隰碁�托ｽｲ繧雁ｵｯ隴擾ｽｴ・趣ｽｨ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｩ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｰ

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
		mpCapsuleCollider->position_ = position_;
		mpCapsuleCollider->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
		
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->position_ = model_->GetAttachmentPosition();
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->position_ = position_;
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->position_ = VAdd(position_, VGet(0.0f, mfSize / 2, 0.0f));
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->position_ = VAdd(position_, VGet(0.0f, mfSize / 2, 0.0f));
	}
	
	


	
	
}

void Enemy::Delete()
{
	
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->SetDeleteFlag(true);
		mpCapsuleCollider = nullptr;
	}
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->SetDeleteFlag(true);
		mpAttachCollider = nullptr;
	}
	if (mpSerchCollider != nullptr)
	{
		mpSerchCollider->SetDeleteFlag(true);
		mpSerchCollider = nullptr;
	}
	if (mpStopCollider != nullptr)
	{
		mpStopCollider->SetDeleteFlag(true);
		mpStopCollider = nullptr;
	}
	if (mpAttackCollider != nullptr)
	{
		mpAttackCollider->SetDeleteFlag(true);
		mpAttackCollider = nullptr;
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



