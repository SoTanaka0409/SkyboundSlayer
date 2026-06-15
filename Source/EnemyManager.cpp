#include"EnemyManager.h"
#include"Enemy3D.h"
#include"Enemy3D_AT.h"
#include"EnemyBoss_1.h"
#include"EnemyMonster.h"

EnemyManager::EnemyManager()

{
	

}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Draw()
{
	

}

void EnemyManager::Update()
{
}



void EnemyManager::NewEnemyList(enemydate date)
{
	
	if (date.tag ==night_stage1)
	{
		
		for (int i = 0; i < date.Count; i++)
		{
			
			
			int x = (int)date.spawnCenter.x + (rand() % (int)date.initPos.x - (int)date.initPos.x / 2);
			int y = (int)date.spawnCenter.y + (rand() % (int)date.initPos.y);
			int z = (int)date.spawnCenter.z + (rand() % (int)date.initPos.z - (int)date.initPos.z / 2);
			
			VECTOR initpos = VGet(x, y,z);
			mpEneList.push_back( new Enemy3D_AT(date.filename, initpos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.attack, date.xp,date.money, date.isSeparateAnim));
		}
	}
	if (date.tag == archerl_stage1)
	{
		for (int i = 0; i < date.Count; i++)
		{
			
			int x = (int)date.spawnCenter.x + (rand() % (int)date.initPos.x - (int)date.initPos.x / 2);
			int y = (int)date.spawnCenter.y + (rand() % (int)date.initPos.y);
			int z = (int)date.spawnCenter.z + (rand() % (int)date.initPos.z - (int)date.initPos.z / 2);
			VECTOR initpos = VGet(x, y, z);
		
			mpEneList.push_back(new Enemy3D(date.filename, initpos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.xp,date.money, date.isSeparateAnim));
		}
	}
	if (date.tag == boss_stage1)
	{
		mpEneList.push_back(new EnemyBoss_1(date.filename, date.initPos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.xp,date.money, date.isSeparateAnim));

	}
	if (date.tag == night_Defo)
	{
		mpEneList.push_back(new Enemy3D_AT("Resource/Model/T.mv1", date.initPos, 20, 5.0f, 60.0f, 1000, 100, 100, 3, 100, date.money, true));
	}
	if (date.tag == monster_stage1)
	{
		for (int i = 0; i < date.Count; i++)
		{
			int x = (int)date.spawnCenter.x + (rand() % (int)date.initPos.x - (int)date.initPos.x / 2);
			int y = (int)date.spawnCenter.y + (rand() % (int)date.initPos.y);
			int z = (int)date.spawnCenter.z + (rand() % (int)date.initPos.z - (int)date.initPos.z / 2);
			VECTOR initpos = VGet(x, y, z);
			mpEneList.push_back(new EnemyMonster(date.filename, initpos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.xp, date.money, date.isSeparateAnim));
		}
	}
	//‘‚â‚µ‚Ä‚¢‚­
}


		

