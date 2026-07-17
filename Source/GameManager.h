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
// [入力] なし [出力] bool: カットシーン完了フラグ [副作用] カメラ座標とゲーム状態の更新
    bool UpdateBossCutscene();
// ボス遷移時のフェード更新
// [入力] なし [出力] bool: フェード完了フラグ [副作用] フェードアルファ値の更新
    bool UpdateBossFade();
// ショップフェーズの更新
// [入力] なし [出力] なし [副作用] ショップタイマーの更新とフェーズ移行
    void UpdateShopPhase();
// バトルフェーズの更新
// [入力] なし [出力] なし [副作用] 敵の出現管理とフェーズ移行
    void UpdateBattlePhase();
// ショップフェーズへの移行処理
// [入力] Phase nextPhase: 次のフェーズ [出力] なし [副作用] フェーズ変更、ショップタイマーのリセット
    void StartShopPhase(Phase nextPhase);
// ボスフェーズへの遷移開始
// [入力] なし [出力] なし [副作用] フェーズの変更とフェード状態の初期化
    void StartBossTransition();
// ボスゲート出現のカットシーン開始
// [入力] なし [出力] なし [副作用] カメラフラグの切り替え
    void StartBossGateCutscene();
// ショップNPCをフィールドに出現させる
// [入力] なし [出力] なし [副作用] StatShopオブジェクトの生成と移動
    void SendShopsIn();
// ショップNPCを退場させる
// [入力] なし [出力] なし [副作用] StatShopオブジェクトの退場フラグ設定
    void SendShopsOut();
// ショップNPCが到着したか判定する
// [入力] なし [出力] bool: 到着判定 [副作用] なし
    bool AreShopsArrived() const;
// 現在ショップフェーズか判定する
// [入力] なし [出力] bool: ショップフェーズならtrue [副作用] なし
    bool IsShopPhase() const;
// 現在ボスへのフェード遷移中か判定する
// [入力] なし [出力] bool: 遷移中ならtrue [副作用] なし
    bool IsBossFadePhase() const;
// プレイヤーがボスポータルに触れたか判定する
// [入力] なし [出力] bool: 触れていればtrue [副作用] なし
    bool IsBossGateTouched() const;
// 現在のフェーズ名を取得する
// [入力] なし [出力] const char*: フェーズの文字列 [副作用] なし
    const char* GetPhaseLabel() const;
// 現在のフェーズのサブラベル（進行状況など）を取得する
// [入力] なし [出力] const char*: サブラベルの文字列 [副作用] なし
    const char* GetPhaseSubLabel() const;
// 現在の敵の生存数を取得する
// [入力] なし [出力] int: 敵の数 [副作用] なし
    int GetEnemyCount() const;
// フェーズ情報のHUDを描画する
// [入力] なし [出力] なし [副作用] 画面描画
    void DrawPhaseHud();
// ショップフェーズの残り時間バナーを描画する
// [入力] なし [出力] なし [副作用] 画面描画
    void DrawShopBanner();
// ボス遷移時のフェードを描画する
// [入力] なし [出力] なし [副作用] 画面描画
    void DrawBossFade();
// デバッグ機能のキー入力更新
// [入力] なし [出力] なし [副作用] フェーズの強制変更など
    void UpdateDebugControls();
// デバッグ用UIを描画する
// [入力] なし [出力] なし [副作用] 画面描画
    void DrawDebugPanel();
// デバッグ機能：現在の敵を全滅させる
// [入力] なし [出力] なし [副作用] 敵HPをゼロにする
    void DebugKillEnemies();
// デバッグ機能：ボス戦へ直行する
// [入力] なし [出力] なし [副作用] ボスフェーズへの強制変更
    void DebugGoBoss();
// デバッグコントロールが有効か判定する
// [入力] なし [出力] bool: 有効フラグ [副作用] なし
    bool IsDebugControlsEnabled() const;
// 現在のフェーズに応じた敵をスポーンさせる
// [入力] なし [出力] なし [副作用] Enemyオブジェクトの生成
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
// [入力] EnemyManager::enemydate& e: 敵データ [出力] なし [副作用] e内のパラメータ変更
    void ApplyDifficultyMultipliers(EnemyManager::enemydate& e);

public:
// GameManagerのコンストラクタ
// [入力] EnemyManager* enemyManager, Difficulty diff [出力] なし [副作用] 変数の初期化
    GameManager(EnemyManager* enemyManager, Difficulty diff = Difficulty::kNormal);
// GameManagerのデストラクタ
// [入力] なし [出力] なし [副作用] なし
    ~GameManager();

// ゲームマネージャーの更新処理
// [入力] なし [出力] なし [副作用] 状態更新、フェーズ遷移処理
    void Update();
// ゲームマネージャーの描画処理（HUDなど）
// [入力] なし [出力] なし [副作用] 画面描画
    void Draw();
// ミニマップを描画する
// [入力] なし [出力] なし [副作用] 画面描画
    void DrawMinimap();

    // 現在のゲームフェーズを取得する
    // [入力] なし [出力] Phase: 現在のフェーズ [副作用] なし
    Phase GetCurrentPhase() const { return current_phase_; }

    // ショップフェーズの残りフレーム数を取得する
    // [入力] なし [出力] int: 残りタイマー [副作用] なし
    int GetShopTimer() const { return shop_timer_; }
};

