#include "EnemyManager.h"
#include "Enemy3D.h"
#include "Enemy3D_AT.h"
#include "EnemyBoss_1.h"
#include "EnemyMonster.h"
#include "Master.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"

/// @brief EnemyManagerクラスのコンストラクタ
EnemyManager::EnemyManager()
{
}

/// @brief EnemyManagerクラスのデストラクタ
EnemyManager::~EnemyManager()
{
}

/// @brief 敵全体の管理描画処理を行う
void EnemyManager::Draw()
{
}

/// @brief 敵全体の管理毎フレーム更新処理を行う
void EnemyManager::Update()
{
}

/// @brief 新しい敵の生成リクエストを受け取り、障害物を避けた安全な位置を計算して一括生成・登録する
/// @param date 生成する敵のパラメータ構造体（タグ、出現位置、HP、モデルパス等）
void EnemyManager::NewEnemyList(enemydate date)
{
	auto pObjList = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Object);

	// 障害物オブジェクト（Tag3D_Object）の周り500 unit以内を避けて安全なスポーン座標を返すラムダ式
	auto GetSafeSpawnPos = [&](VECTOR center, VECTOR range) -> VECTOR {
		VECTOR bestPos = center;
		if (range.x <= 0 || range.y <= 0 || range.z <= 0) return center;

		for (int retry = 0; retry < 50; ++retry)
		{
			int x = (int)center.x + (rand() % (int)range.x - (int)range.x / 2);
			int y = (int)center.y + (rand() % (int)range.y);
			int z = (int)center.z + (rand() % (int)range.z - (int)range.z / 2);
			VECTOR pos = VGet((float)x, (float)y, (float)z);
			bool safe = true;

			for (auto obj : pObjList)
			{
				VECTOR objPos = obj->GetPosition();
				float dx = pos.x - objPos.x;
				float dz = pos.z - objPos.z;
				float distSq = dx * dx + dz * dz;

				if (distSq < 500.0f * 500.0f) // 半径500 unitの安全領域判定
				{
					safe = false;
					break;
				}
			}

			if (safe) return pos;
			bestPos = pos;
		}

		return bestPos;
		};

	if (date.tag == night_stage1)
	{
		for (int i = 0; i < date.Count; i++)
		{
			VECTOR initpos = GetSafeSpawnPos(date.spawnCenter, date.initPos);
			ene_list_.push_back(new Enemy3D_AT(date.filename, initpos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.attack, date.money, date.is_separate_anim_));
		}
	}

	if (date.tag == archerl_stage1)
	{
		for (int i = 0; i < date.Count; i++)
		{
			VECTOR initpos = GetSafeSpawnPos(date.spawnCenter, date.initPos);
			ene_list_.push_back(new Enemy3D(date.filename, initpos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.money, date.is_separate_anim_));
		}
	}

	if (date.tag == boss_stage1)
	{
		ene_list_.push_back(new EnemyBoss_1(date.filename, date.initPos, date.hp, date.speed, date.HitSize, date.Serch1, date.Serch2, date.Serch3, date.money, date.is_separate_anim_));
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