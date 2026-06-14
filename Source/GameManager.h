#pragma once
#include "DxLib.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "Config.h"

class GameManager {
public:
    enum class Difficulty {
        EASY,
        NORMAL,
        HARD
    };

    enum class Phase {
        PHASE_1,
        SHOP_1,
        PHASE_2,
        SHOP_2,
        PHASE_3,
        SHOP_3,
        BOSS,
        CLEAR
    };

private:
    Difficulty mDifficulty;
    Phase mCurrentPhase;
    int mShopTimer;
    EnemyManager* mpEnemyManager;

    void SpawnPhaseEnemies();
    void ApplyDifficultyMultipliers(EnemyManager::enemydate& e);

public:
    GameManager(EnemyManager* enemyManager, Difficulty diff = Difficulty::NORMAL);
    ~GameManager();

    void Update();
    void Draw();

    Phase GetCurrentPhase() const { return mCurrentPhase; }
    int GetShopTimer() const { return mShopTimer; }
};
