#pragma once
#include "DxLib.h"
#include <string>
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "Config.h"

class GameManager {
public:
    /// @brief ゲーム難易度（敵のステータス倍率に影響）
    enum class Difficulty {
        kEasy,
        kNormal,
        kHard
    };

    /// @brief ゲームの進行フェーズ状態
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
    Difficulty m_Difficulty;
    Phase m_CurrentPhase;
    int m_BossCutsceneTimer;
    VECTOR m_CutsceneStartPos;
    int m_ShopTimer;
    int m_FadeAlpha;
    EnemyManager* m_EnemyManager;
    VECTOR m_BossPortalPos;

/// @brief ボス出現カットシーンの更新
    bool UpdateBossCutscene();
/// @brief ボス遷移時のフェード更新
    bool UpdateBossFade();
/// @brief ショップフェーズの更新
    void UpdateShopPhase();
/// @brief バトルフェーズの更新
    void UpdateBattlePhase();
/// @brief ショップフェーズへの移行処理
    void StartShopPhase(Phase nextPhase);
/// @brief ボスフェーズへの遷移開始
    void StartBossTransition();
/// @brief ボスゲート出現のカットシーン開始
    void StartBossGateCutscene();
/// @brief ショップNPCをフィールドに出現させる
    void SendShopsIn();
/// @brief ショップNPCを退場させる
    void SendShopsOut();
/// @brief ショップNPCが到着したか判定する
    bool AreShopsArrived() const;
/// @brief 現在ショップフェーズか判定する
    bool IsShopPhase() const;
/// @brief 現在ボスへのフェード遷移中か判定する
    bool IsBossFadePhase() const;
/// @brief プレイヤーがボスポータルに触れたか判定する
    bool IsBossGateTouched() const;
/// @brief 現在のフェーズ名を取得する
    const char* GetPhaseLabel() const;
/// @brief 現在のフェーズのサブラベル（進行状況など）を取得する
    const char* GetPhaseSubLabel() const;
/// @brief 現在の敵の生存数を取得する
    int GetEnemyCount() const;
/// @brief フェーズ情報のHUDを描画する
    void DrawPhaseHud();
/// @brief ショップフェーズの残り時間バナーを描画する
    void DrawShopBanner();
/// @brief ボス遷移時のフェードを描画する
    void DrawBossFade();
/// @brief デバッグ機能のキー入力更新
    void UpdateDebugControls();
/// @brief デバッグ用UIを描画する
    void DrawDebugPanel();
/// @brief デバッグ機能：現在の敵を全滅させる
    void DebugKillEnemies();
/// @brief デバッグ機能：ボス戦へ直行する
    void DebugGoBoss();
/// @brief デバッグコントロールが有効か判定する
    bool IsDebugControlsEnabled() const;
/// @brief 現在のフェーズに応じた敵をスポーンさせる
    void SpawnPhaseEnemies();
    void SpawnPhase1Enemies(const VECTOR& centerPos);
    void SpawnPhase2Enemies(const VECTOR& centerPos);
    void SpawnPhase3Enemies(const VECTOR& centerPos);
    void SpawnBossEnemy();
    EnemyManager::enemydate MakeEnemyData(
        EnemyManager::enemy_tag tag,
        const std::string& filename,
        const VECTOR& spawnCenter,
        const VECTOR& initPos,
        float hp,
        float speed,
        float attack,
        float hitSize,
        float search1,
        float search2,
        float search3,
        bool isSeparateAnim,
        int money,
        int count) const;
    void AddEnemy(EnemyManager::enemydate enemyData);
    void DrawMinimapFrame(float mapX, float mapY, float mapSize) const;
    VECTOR GetMinimapOffset(VECTOR pos, VECTOR playerPos, float viewRange, float maxDistance) const;
    VECTOR ClampMinimapOffset(VECTOR offset, float maxDistance) const;
    void DrawMinimapPortal(float mapCenterX, float mapCenterY, VECTOR playerPos, float viewRange, float maxDistance) const;
    void DrawMinimapShops(float mapCenterX, float mapCenterY, VECTOR playerPos, float viewRange, float maxDistance) const;
    void DrawMinimapEnemies(float mapX, float mapY, float mapSize, float mapCenterX, float mapCenterY, VECTOR playerPos, float viewRange, float maxDistance) const;
    void DrawMinimapPlayer(float mapCenterX, float mapCenterY) const;
/// @brief 難易度に応じて敵のステータスに倍率をかける
    void ApplyDifficultyMultipliers(EnemyManager::enemydate& e);

public:
/// @brief GameManagerのコンストラクタ
    GameManager(EnemyManager* enemyManager, Difficulty diff = Difficulty::kNormal);
/// @brief GameManagerのデストラクタ
    ~GameManager();

/// @brief ゲームマネージャーの更新処理
    void Update();
/// @brief ゲームマネージャーの描画処理（HUDなど）
    void Draw();
/// @brief ミニマップを描画する
    void DrawMinimap();

    /// @brief 現在のゲームフェーズを取得する
    Phase GetCurrentPhase() const { return m_CurrentPhase; }

    /// @brief ショップフェーズの残りフレーム数を取得する
    int GetShopTimer() const { return m_ShopTimer; }
};

