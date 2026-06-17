#include "GameManager.h"
#include "Player3D.h"
#include "Enemy.h"
#include <cmath>
#include <DxLib.h>

GameManager::GameManager(EnemyManager* enemyManager, Difficulty diff)
    : mpEnemyManager(enemyManager), mDifficulty(diff), mCurrentPhase(Phase::PHASE_1), mShopTimer(0)
{
    // Start the first wave
    SpawnPhaseEnemies();
}

GameManager::~GameManager()
{
}

void GameManager::Update()
{
    // DEBUG: Press '0' to wipe out all enemies in the current phase
    if (CheckHitKey(KEY_INPUT_0)) {
        auto enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
        for (auto enemy : enemies)
		{
			Enemy* e = dynamic_cast<Enemy*>(enemy);
            if (e) {
                e->Damage(e->GetMaxHp()); // Deal max HP damage to trigger death animation
            }
        }
    }

    if (mCurrentPhase == Phase::SHOP_1 || mCurrentPhase == Phase::SHOP_2 || mCurrentPhase == Phase::SHOP_3)
    {
        if (mCurrentPhase != Phase::SHOP_3) {
            mShopTimer--;
            if (mShopTimer <= 0) {
                if (mCurrentPhase == Phase::SHOP_1) {
                    mCurrentPhase = Phase::PHASE_2;
                } else if (mCurrentPhase == Phase::SHOP_2) {
                    mCurrentPhase = Phase::PHASE_3;
                }
                SpawnPhaseEnemies();
            }
        } 
        else
        {
            // SHOP_3: No time limit. Wait for player to enter teleporter.
            auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
            if (p) {
                Player3D* player = dynamic_cast<Player3D*>(p);
                VECTOR playerPos = player->GetPosition();
                
                // Placeholder teleporter position (center of stage, offset)
                VECTOR teleporterPos = VAdd(Config::GetStageCenter(), VGet(0.0f, 0.0f, 800.0f));
                
                float dist = VSize(VSub(playerPos, teleporterPos));
                if (dist < 150.0f) { // 150 radius to enter
                    mCurrentPhase = Phase::BOSS;
                    SpawnPhaseEnemies();
                }
            }
        }
    } else {
        auto enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
        if (enemies.empty())
        {
            if (mCurrentPhase == Phase::PHASE_1) {
                mCurrentPhase = Phase::SHOP_1;
                mShopTimer = 60 * 20; // 20秒
            } else if (mCurrentPhase == Phase::PHASE_2) {
                mCurrentPhase = Phase::SHOP_2;
                mShopTimer = 60 * 20; // 20秒
            } else if (mCurrentPhase == Phase::PHASE_3) {
                mCurrentPhase = Phase::SHOP_3;
                mShopTimer = 60 * 20; // 20秒
            } else if (mCurrentPhase == Phase::BOSS) {
                mCurrentPhase = Phase::CLEAR;
            }
        }
    }
}

void GameManager::Draw()
{
    int fontSize = GetFontSize();
    SetFontSize(30);

    const char* phaseStr = "";
    switch (mCurrentPhase) {
    case Phase::PHASE_1: phaseStr = "Phase 1"; break;
    case Phase::PHASE_2: phaseStr = "Phase 2"; break;
    case Phase::PHASE_3: phaseStr = "Phase 3"; break;
    case Phase::BOSS:    phaseStr = "BOSS Phase"; break;
    case Phase::SHOP_1:  phaseStr = "Shop 1"; break;
    case Phase::SHOP_2:  phaseStr = "Shop 2"; break;
    case Phase::SHOP_3:  phaseStr = "Shop 3"; break;
    case Phase::CLEAR:   phaseStr = "CLEAR!"; break;
    }

    auto enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    int enemyCount = (int)enemies.size();

    DrawFormatString(20, 100, GetColor(255, 255, 255), "Current Phase: %s", phaseStr);
    DrawFormatString(20, 140, GetColor(255, 255, 255), "Enemies Remaining: %d", enemyCount);

    SetFontSize(fontSize);

    if (mCurrentPhase == Phase::SHOP_1 || mCurrentPhase == Phase::SHOP_2 || mCurrentPhase == Phase::SHOP_3) {
        if (mCurrentPhase != Phase::SHOP_3) {
            int seconds = mShopTimer / 60;
            DrawFormatString(1920 / 2 - 150, 50, GetColor(255, 255, 0), "SHOP PHASE - Next Wave in %d s", seconds);
        } else {
            DrawFormatString(1920 / 2 - 350, 50, GetColor(0, 255, 255), "SHOP PHASE - Enter the blue teleporter to start BOSS BATTLE");
            
            // Draw placeholder teleporter
            VECTOR teleporterPos = VAdd(Config::GetStageCenter(), VGet(0.0f, 0.0f, 800.0f));
            DrawCapsule3D(teleporterPos, VAdd(teleporterPos, VGet(0.0f, 200.0f, 0.0f)), 150.0f, 32, GetColor(0, 150, 255), GetColor(0, 150, 255), FALSE);
            DrawSphere3D(VAdd(teleporterPos, VGet(0.0f, 50.0f, 0.0f)), 100.0f, 32, GetColor(0, 255, 255), GetColor(0, 255, 255), FALSE);
        }
    }
}

void GameManager::ApplyDifficultyMultipliers(EnemyManager::enemydate& e)
{
    float statMultiplier = 1.0f;
    float countMultiplier = 1.0f;

    switch (mDifficulty) {
    case Difficulty::EASY:
        statMultiplier = 0.8f;
        countMultiplier = 0.8f;
        break;
    case Difficulty::NORMAL:
        statMultiplier = 1.0f;
        countMultiplier = 1.0f;
        break;
    case Difficulty::HARD:
        statMultiplier = 1.5f;
        countMultiplier = 1.5f;
        break;
    }

    e.hp = static_cast<int>(e.hp * statMultiplier);
    e.attack = static_cast<int>(e.attack * statMultiplier);
    // boss count should remain 1 usually
    if (e.tag != EnemyManager::boss_stage1) {
        e.Count = static_cast<int>(std::ceil(e.Count * countMultiplier));
        if (e.Count < 1) e.Count = 1;
    }
}

void GameManager::SpawnPhaseEnemies()
{
    auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
    Player3D* player = dynamic_cast<Player3D*>(p);
    // 敵がステージから外れて落下・埋没しないように、Configのステージ中心座標を湧き位置の基準とする
    VECTOR centerPos = Config::GetStageCenter();
    if (player != nullptr)
    {
        VECTOR playercenterPos = player->GetPosition();
    }
    VECTOR centerbackPos = VAdd(Config::GetStageCenter(), VGet(0, 0, -2000));

    if (mCurrentPhase == Phase::PHASE_1) {
        // Wave 1
        EnemyManager::enemydate e;
        e.filename = "Resource/Model/T.mv1";
        e.spawnCenter = centerPos;
        e.initPos = VGet(3000.0f, 100.0f, 3000.0f);
        e.hp = 20;
        e.speed = 3.0f;
        e.attack = 2.0f;
        e.HitSize = 60.0f;
        e.Serch1 = 1000.0f;
        e.Serch2 = 100.0f;
        e.Serch3 = 100.0f;
        e.isSeparateAnim = true;
        e.xp = 30.0f;
        e.money = 200;
        e.tag = EnemyManager::night_stage1;
        e.Count = 10;
        
        ApplyDifficultyMultipliers(e);
        mpEnemyManager->NewEnemyList(e);
    }
    else if (mCurrentPhase == Phase::PHASE_2) {
        // Wave 2: 魔法兵士
        EnemyManager::enemydate e1;
        e1.filename = "Resource/Model/T.mv1";
        e1.spawnCenter = centerPos;
        e1.initPos = VGet(8000.0f, 100.0f, 8000.0f);
        e1.hp = 20;
        e1.speed = 3.0f;
        e1.attack = 2.0f;
        e1.HitSize = 60.0f;
        e1.money = 200;
        e1.Serch1 = 2000.0f;
        e1.Serch2 = 1000.0f;
        e1.Serch3 = 1000.0f;
        e1.isSeparateAnim = true;
        e1.xp = 30.0f;
        e1.tag = EnemyManager::archerl_stage1;
        e1.Count = 8;
        
        ApplyDifficultyMultipliers(e1);
        mpEnemyManager->NewEnemyList(e1);

        // Wave 2: 近接剣士
        EnemyManager::enemydate e2;
        e2.filename = "Resource/Model/T.mv1";
        e2.spawnCenter = centerPos;
        e2.initPos = VGet(2000.0f, 100.0f, 2000.0f);
        e2.hp = 20;
        e2.speed = 4.0f;
        e2.attack = 3.0f;
        e2.HitSize = 60.0f;
        e2.Serch1 = 1000.0f;
        e2.Serch2 = 100.0f;
        e2.Serch3 = 100.0f;
        e2.isSeparateAnim = true;
        e2.xp = 30.0f;
        e2.money = 200;
        e2.tag = EnemyManager::night_stage1;
        e2.Count = 6;

        ApplyDifficultyMultipliers(e2);
        mpEnemyManager->NewEnemyList(e2);
    }
    else if (mCurrentPhase == Phase::PHASE_3) {
        // Wave 3: 重量級代用
        EnemyManager::enemydate e_heavy;
        e_heavy.filename = "Resource/Model/monster.mv1";
        e_heavy.spawnCenter = centerPos;
        e_heavy.initPos = VGet(12000.0f, 100.0f, 12000.0f);
        e_heavy.hp = 100;
        e_heavy.speed = 2.0f;
        e_heavy.attack = 5.0f;
        e_heavy.HitSize = 100.0f;
        e_heavy.Serch1 = 1000.0f;
        e_heavy.Serch2 = 100.0f;
        e_heavy.Serch3 = 100.0f;
        e_heavy.isSeparateAnim = true;
        e_heavy.xp = 100.0f;
        e_heavy.money = 500;
        e_heavy.tag = EnemyManager::monster_stage1;
        e_heavy.Count = 5;

        ApplyDifficultyMultipliers(e_heavy);
        mpEnemyManager->NewEnemyList(e_heavy);

        // Wave 3: 魔法兵士
        EnemyManager::enemydate e_magic;
        e_magic.filename = "Resource/Model/T.mv1";
        e_magic.spawnCenter = centerPos;
        e_magic.initPos = VGet(6000.0f,100.0f, 6000.0f);
        e_magic.hp = 20;
        e_magic.speed = 3.0f;
        e_magic.attack = 2.0f;
        e_magic.HitSize = 60.0f;
        e_magic.Serch1 = 2000.0f;
        e_magic.Serch2 = 1000.0f;
        e_magic.Serch3 = 1000.0f;
        e_magic.isSeparateAnim = true;
        e_magic.xp = 30.0f;
        e_magic.money = 200;
        e_magic.tag = EnemyManager::archerl_stage1;
        e_magic.Count = 5;

        ApplyDifficultyMultipliers(e_magic);
        mpEnemyManager->NewEnemyList(e_magic);

        // Wave 3: 近接兵士
        EnemyManager::enemydate e_melee;
        e_melee.filename = "Resource/Model/T.mv1";
        e_melee.spawnCenter = centerPos;
        e_melee.initPos = VGet(6000.0f, 100.0f, 6000.0f);
        e_melee.hp = 20;
        e_melee.speed = 3.0f;
        e_melee.attack = 2.0f;
        e_melee.HitSize = 60.0f;
        e_melee.Serch1 = 1000.0f;
        e_melee.Serch2 = 100.0f;
        e_melee.Serch3 = 100.0f;
        e_melee.isSeparateAnim = true;
        e_melee.xp = 30.0f;
        e_melee.money = 200;
        e_melee.tag = EnemyManager::night_stage1;
        e_melee.Count = 5;

        ApplyDifficultyMultipliers(e_melee);
        mpEnemyManager->NewEnemyList(e_melee);
    }
    else if (mCurrentPhase == Phase::BOSS) {
        EnemyManager::enemydate e2;
        e2.filename = "Resource/3D/Boss1.mv1";
        e2.spawnCenter = centerPos;
        e2.initPos = VGet(4000.0f, 100.0f, 4000.0f);
        e2.hp = 300;
        e2.speed = 10.0f;
        e2.attack = 20.0f;
        e2.HitSize = 300.0f;
        e2.Serch1 = 2000.0f;
        e2.Serch2 = 1000.0f;
        e2.Serch3 = 1000.0f;
        e2.money = 3000;
        e2.isSeparateAnim = true;
        e2.xp = 300.0f;
        e2.tag = EnemyManager::boss_stage1;
        e2.Count = 1;

        ApplyDifficultyMultipliers(e2);
        mpEnemyManager->NewEnemyList(e2);
    }
}
