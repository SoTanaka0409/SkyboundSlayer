#include"EnemyManager.h"
#include"Enemy3D.h"
#include"Enemy3D_AT.h"
#include"EnemyBoss_1.h"
#include"EnemyMonster.h"
#include"Master.h"
#include"SceneManager.h"
#include"Scene.h"
#include"ObjectManager.h"


/*
 * 目的（EnemyManagerのEnemyManager処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
EnemyManager::EnemyManager()

{
	

}

EnemyManager::~EnemyManager()
{

}


/*
 * 目的（EnemyManagerのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyManager::Draw()
{
	

}


/*
 * 目的（EnemyManagerのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyManager::Update()
{
}




/*
 * 目的（EnemyManagerのNewEnemyList処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyManager::NewEnemyList(enemydate date)
{
	auto pObjList = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Object);
	auto GetSafeSpawnPos = [&](VECTOR center, VECTOR range) -> VECTOR {
		VECTOR bestPos = center;
		if (range.x <= 0 || range.y <= 0 || range.z <= 0) return center;
		for (int retry = 0; retry < 50; ++retry) {
			int x = (int)center.x + (rand() % (int)range.x - (int)range.x / 2);
			int y = (int)center.y + (rand() % (int)range.y);
			int z = (int)center.z + (rand() % (int)range.z - (int)range.z / 2);
			VECTOR pos = VGet((float)x, (float)y, (float)z);
			bool safe = true;
			for (auto obj : pObjList) {
				VECTOR objPos = obj->GetPosition();
				float dx = pos.x - objPos.x;
				float dz = pos.z - objPos.z;
				float distSq = dx * dx + dz * dz;
				if (distSq < 500.0f * 500.0f) { // 500 units safe radius
					safe = false;
					break;
				}
			}
			if (safe) return pos;
			bestPos = pos;
		}
		return bestPos;
	};
	
	if (date.tag ==night_stage1)
	{
		
		for (int i = 0; i < date.Count; i++)
		{
			VECTOR initpos = GetSafeSpawnPos(date.spawnCenter, date.initPos);
			ene_list_.push_back( new Enemy3D_AT(date.filename, initpos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.attack,date.money, date.is_separate_anim_));
		}
	}
	if (date.tag == archerl_stage1)
	{
		for (int i = 0; i < date.Count; i++)
		{
			VECTOR initpos = GetSafeSpawnPos(date.spawnCenter, date.initPos);
			ene_list_.push_back(new Enemy3D(date.filename, initpos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3,date.money, date.is_separate_anim_));
		}
	}
	if (date.tag == boss_stage1)
	{
		ene_list_.push_back(new EnemyBoss_1(date.filename, date.initPos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3,date.money, date.is_separate_anim_));

	}
	if (date.tag == night_Defo)
	{
		ene_list_.push_back(new Enemy3D_AT("Resource/3Dモデル/キャラクターとアニメーション/01_人型キャラクターモデル.mv1", date.initPos, 20, 5.0f, 60.0f, 1000, 100, 100, 3, date.money, true));
	}
	if (date.tag == monster_stage1)
	{
		for (int i = 0; i < date.Count; i++)
		{
			VECTOR initpos = GetSafeSpawnPos(date.spawnCenter, date.initPos);
			ene_list_.push_back(new EnemyMonster(date.filename, initpos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.money, date.is_separate_anim_));
		}
	}
	
}


		

