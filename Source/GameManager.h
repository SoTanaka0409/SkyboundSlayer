#pragma once
#include "DxLib.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "Config.h"

class GameManager {
public:
    // Difficulty controls enemy stat multipliers.
    enum class Difficulty {
        kEasy,
        kNormal,
        kHard
    };

    // Main game flow state, including shops and boss transition.
    enum class Phase {
        kPhase1,
        kShop1,
        kPhase2,
        kShop2,
        kPhase3,
        kShop3,
        kFadeOutToBoss,
        kFadeInBoss,
        kBoss,
        kClear
    };

private:
    Difficulty difficulty_;
    Phase current_phase_;
    int boss_cutscene_timer_;
    VECTOR cutscene_start_pos_;
    int shop_timer_;
    int fade_alpha_;
    EnemyManager* enemy_manager_;
    VECTOR boss_portal_pos_;

    bool UpdateBossCutscene();
    bool UpdateBossFade();
    void UpdateShopPhase();
    void UpdateBattlePhase();
    void StartShopPhase(Phase nextPhase);
    void StartBossTransition();
    void StartBossGateCutscene();
    void SendShopsIn();
    void SendShopsOut();
    bool AreShopsArrived() const;
    bool IsShopPhase() const;
    bool IsBossFadePhase() const;
    bool IsBossGateTouched() const;
    const char* GetPhaseLabel() const;
    const char* GetPhaseSubLabel() const;
    int GetEnemyCount() const;
    void DrawPhaseHud();
    void DrawShopBanner();
    void DrawBossFade();
    void UpdateDebugControls();
    void DrawDebugPanel();
    void DebugKillEnemies();
    void DebugGoBoss();
    bool IsDebugControlsEnabled() const;
    void SpawnPhaseEnemies();
    void ApplyDifficultyMultipliers(EnemyManager::enemydate& e);

public:
    GameManager(EnemyManager* enemyManager, Difficulty diff = Difficulty::kNormal);
    ~GameManager();

    void Update();
    void Draw();
    void DrawMinimap();

    /*
     * Returns the current game phase.
     * [Input] none
     * [Output] Current Phase
     * [Side effects] none
     */
    Phase GetCurrentPhase() const { return current_phase_; }

    /*
     * Returns remaining shop phase frames.
     * [Input] none
     * [Output] Remaining frame count
     * [Side effects] none
     */
    int GetShopTimer() const { return shop_timer_; }
};

