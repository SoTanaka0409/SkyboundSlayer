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
	mnChance = 30;//アイテムのドロップ確率
	AttackInterval = 60;
	AttackCount = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	SetTagEne(Enemy::boss_stage1);

	//SpeedDownNomber =mnSpeed-
	//モデルの生成
	mpModel->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	mpModel->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	mpModel->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKMAGIC, "Resource/Model/MagicAttack.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Jump Attack.mv1");

	mpModel->SetScale(VGet(4.0f, 4.0f, 4.0f));
	
	mpJumpAttackCoiider = new SphereCollider(this, mvPosition, 400.0f);

	mpDebug = new Debug();
	Item::ItemInformation* itemInfo;
	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 3;
	itemInfo->ID = Item::ItemID::HIGHHEAL;
	itemInfo->Name = "heal";
	itemInfo->isLog = false;
	mpItemManager->AddItem(itemInfo);
	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 3;
	itemInfo->ID = Item::ItemID::SPEED;
	itemInfo->Name = "heal";
	itemInfo->isLog = false;
	mpItemManager->AddItem(itemInfo);
	Weapon::WeaponDate* weaponInf;
	weaponInf = new Weapon::WeaponDate;
	weaponInf->damage = 10;
	weaponInf->filename = "Resource/3D/Sabel.mv1";
	weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon4;
	weaponInf->name = "weapon4";
	weaponInf->isLog = false;
	mpWeaponManager->AddWeapon(weaponInf);

	//mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize, 0.0f)), mfSize);
	//mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 30.0f);
	//mpSerchCollider = new SphereCollider(this, mvPosition, HitSerch);//敵を探知する円
	//mpAttackCollider = new SphereCollider(this, mvPosition, HitAttackSerch);//敵を攻撃し始める円
	//mpStopCollider = new SphereCollider(this, mvPosition, HitStopSerch);//敵との距離を保つ円d
}

EnemyBoss_1::~EnemyBoss_1()
{

}

void EnemyBoss_1::Update()
{
	if (isDead)
	{
		DeathEnemy();
	}
	else
	{
		if (mpModel != nullptr)
		{
		
			//if (mfHp > mfMaxHp / 2)//体力の管理で攻撃方法を変える
			//{
			//	Attack();
			//}
			//else
			//{
			//	Attack2();
			//}
			Attack();
			if (!(mpModel->GetNowState() == ANIMATION_ATTACK) || !(mpModel->GetNowState() == ANIMATION_ATTACKJUMP))
			{
				RotationByMove();
				Move();


			}

			mpModel->Update();
			//mpDH->Update();//drawHpのアップデートを呼ぶ
			CollPositionUpdate();
			mpJumpAttackCoiider->mvPosition = mvPosition;

		}
	}
}

void EnemyBoss_1::Draw()
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

void EnemyBoss_1::Attack()
{
	AnimationState now = mpModel->GetNowState();

	//f ()プレイヤーに近づいた時と書く
	if (AttackCount >= AttackInterval && isHitAttackSearch)
	{
		AttackCount = 0;
		//攻撃モーションに変更
		mpModel->ChangeAnimation(ANIMATION_ATTACKMAGIC);
		//ループはさせない
		mpModel->SetLoop(false);
		//モーション後は待機モーションに戻す
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		//mpModel->SetRotation(mvRotation);
		isHitAttackSearch = false;
		new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, GoPosition, 0, 150);
		new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, VScale(GoPosition,sinf(30)), 0, 150);
		new Magic_Ene("Resource/Damage.png", VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, VScale(GoPosition, cosf(30)), 0, 150);
	}
	if (!(now == ANIMATION_ATTACKMAGIC))
	{
		AttackCount++;
		AttackHitJudgmentflag = false;//当たり判定の復活
	}



}
void EnemyBoss_1::OnTrigger(Collider* collider, Collider* check)
{//当たった瞬間の処理
	if (mfHp <= 0)return; auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	AnimationState now = mpModel->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpJumpAttackCoiider && check->mpParentObject->GetTag() == Tag3D_Player3D)
		{//mpModelの番号もあっている、けどこのif文に入らない
			Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				//if (pPlayer->GetInvisible()) { return; }
				if (now == ANIMATION_ATTACK && !AttackHitJudgmentflag)
				{

					pPlayer->Damage(mfAttack);
					AttackHitJudgmentflag = true;//当たったよー
				}
			}

		}
	}
}

void EnemyBoss_1::DeathEnemy()
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	isDead = true;
	//Delete();
	//Master::TutorialCount++;
	mpModel->ChangeAnimation(ANIMATION_DYING);
	//ループはさせない
	mpModel->SetLoop(false);
	//モーション後は待機モーションに戻す
	mpModel->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	//Master::mpLevelUp->SetXp(Master::mpLevelUp->GetXp() + mfHaveXp);
	if (mpModel->IsAnimationLoopFinish())
	{
		mpInventory->DropInventory(mnChance, mbWeapon, mbItem);
		if (player != nullptr) {
			player->mpHaveMoney->AddMoney(mfHaveMoney);
			player->mpLevelUp->AddXp(mfHaveXp);
		}
		Master::GameClearCount++;
		
		Delete();
		SetDeleteFlag(true);
	}

	mpModel->Draw();
	mpModel->Update();


}

