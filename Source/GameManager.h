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
    // ゲーム難易度（敵のステータス倍率に影響）
    enum class Difficulty {
        kEasy,
        kNormal,
        kHard
    };

    // ゲームの進行フェーズ状態
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

// ボス出現カットシーンの更新
    bool UpdateBossCutscene();
// ボス遷移時のフェード更新
    bool UpdateBossFade();
// ショップフェーズの更新
    void UpdateShopPhase();
// バトルフェーズの更新
    void UpdateBattlePhase();
// ショップフェーズへの移行処理
    void StartShopPhase(Phase nextPhase);
// ボスフェーズへの遷移開始
    void StartBossTransition();
// ボスゲート出現のカットシーン開始
    void StartBossGateCutscene();
// ショップNPCをフィールドに出現させる
    void SendShopsIn();
// ショップNPCを退場させる
    void SendShopsOut();
// ショップNPCが到着したか判定する
    bool AreShopsArrived() const;
// 現在ショップフェーズか判定する
    bool IsShopPhase() const;
// 現在ボスへのフェード遷移中か判定する
    bool IsBossFadePhase() const;
// プレイヤーがボスポータルに触れたか判定する
    bool IsBossGateTouched() const;
// 現在のフェーズ名を取得する
    const char* GetPhaseLabel() const;
// 現在のフェーズのサブラベル（進行状況など）を取得する
    const char* GetPhaseSubLabel() const;
// 現在の敵の生存数を取得する
    int GetEnemyCount() const;
// フェーズ情報のHUDを描画する
    void DrawPhaseHud();
// ショップフェーズの残り時間バナーを描画する
    void DrawShopBanner();
// ボス遷移時のフェードを描画する
    void DrawBossFade();
// デバッグ機能のキー入力更新
    void UpdateDebugControls();
// デバッグ用UIを描画する
    void DrawDebugPanel();
// デバッグ機能：現在の敵を全滅させる
    void DebugKillEnemies();
// デバッグ機能：ボス戦へ直行する
    void DebugGoBoss();
// デバッグコントロールが有効か判定する
    bool IsDebugControlsEnabled() const;
// 現在のフェーズに応じた敵をスポーンさせる
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
// 難易度に応じて敵のステータスに倍率をかける
    void ApplyDifficultyMultipliers(EnemyManager::enemydate& e);

public:
// GameManagerのコンストラクタ
    GameManager(EnemyManager* enemyManager, Difficulty diff = Difficulty::kNormal);
// GameManagerのデストラクタ
    ~GameManager();

// ゲームマネージャーの更新処理
    void Update();
// ゲームマネージャーの描画処理（HUDなど）
    void Draw();
// ミニマップを描画する
    void DrawMinimap();

    // 現在のゲームフェーズを取得する
    Phase GetCurrentPhase() const { return current_phase_; }

    // ショップフェーズの残りフレーム数を取得する
    int GetShopTimer() const { return shop_timer_; }
};

