#pragma once
#include "SceneGame.h"
#include "Dxlib.h"
#include "Master.h"
#include "Texture.h"
#include "EnemyManager.h"
#include "GameManager.h"
#include "PauseMenu.h"

/// @brief 3D空間をベースとしたメインゲームループを管理するシーン。ステージの構築、アクターのロード、環境構築を統括する
class GameScene : public SceneGame
{
private:
    PauseMenu* m_PauseMenu;
    Texture* m_Texture;  // ロード画面や警告UIなどで使用する2Dテクスチャ（汎用背景等）
    Texture* m_Texture2; // 汎用UIやエフェクト用のサブテクスチャ

public:
/// @details 3Dゲームシーン固有のタイマーや状態フラグを安全な初期値にセットアップする
    GameScene();

/// @details 派生シーンとして確保したリソースを確実に解放するための仮想デストラクタ
    ~GameScene();

/// @details 3Dモデル、CSVからのステージ配置、環境光などのセットアップを開始し、ロード画面へ移行する
    void Initialize();

/// @details シーン離脱時（タイトルへの帰還等）に、動的生成したアクターや3Dリソースを破棄してメモリリークを防ぐ
    void Finalize();

/// @details 3D空間（地形・キャラ）の描画パスを実行後、Zバッファを無視して手前に2D UI（ロードバー等）を描画する
    void Draw();

/// @details ロード完了フラグ(m_IsLoadFlag)を監視し、完了後は各アクターやゲームマネージャーのロジックを進行させる
    void Update();

private:
/// @details 3D空間の光源（ディレクショナルライト等）、フォグ、カメラの初期クリップ面を設定する
    void SetupEnvironment();
/// @details プレイヤーや中核となる敵AIなど、ゲーム進行に必須な動的エンティティをメモリに生成する
    void CreateInitialActors();
/// @details 地形モデルのロードと、コリジョンマネージャーへの当たり判定用メッシュの登録を行う
    void CreateStage();
/// @details CSVから座標データを読み込み、木や障害物などの静的オブジェクトをレベルデザイン通りに自動配置する
    void LoadStageObjectsFromCsv();
/// @details プレイヤーの視点に追従する巨大な天球モデルを配置し、空間の果て（背景）を表現する
    void CreateSkyBox();
/// @details デバッグビルド時のみ、XZ平面に距離感把握用のグリッド線を描画バッファへ登録する
    void DrawDebugGrid();

    float m_WarningRadius; // ボス出現時など、特定エリアの接近警告エフェクトを描画するための現在の半径
    float m_MaxRadius = 520.0f; // 警告エフェクトが広がる最大範囲（この値に達すると点滅等の演出へ移行）

    float m_LoadTimer;     // アセットの非同期読み込み中にプログレスバーやロード演出を進行させるためのタイマー
    float m_LoadCount;     // 読み込みが完了したアセットの数（ロード進捗率の計算用）
    bool m_IsLoadFlag;    // 全リソースの準備が完了し、Updateによるメインロジックを開始してよいかを示すフラグ

    VECTOR m_Pos = { 200, 400 }; // 画面ロード時や警告UI表示時の基準となるスクリーン2D座標
    VECTOR m_Size = { 600, 100 }; // UI要素（プログレスバーや背景パネル）のピクセルサイズ
};
