#pragma once
#include "SceneGame.h"
#include "Dxlib.h"
#include "Master.h"
#include "Texture.h"
#include "EnemyManager.h"
#include "GameManager.h"
#include "PauseMenu.h"

// 3D空間をベースとしたメインゲームループを管理するシーン。ステージの構築、アクターのロード、環境構築を統括する
class GameScene : public SceneGame
{
private:
    PauseMenu* pause_menu_;
    Texture* texture_;  // ロード画面や警告UIなどで使用する2Dテクスチャ（汎用背景等）
    Texture* texture2_; // 汎用UIやエフェクト用のサブテクスチャ

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

/// @details ロード完了フラグ(is_load_flag_)を監視し、完了後は各アクターやゲームマネージャーのロジックを進行させる
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

    float warning_radius_; // ボス出現時など、特定エリアの接近警告エフェクトを描画するための現在の半径
    float max_radius_ = 520.0f; // 警告エフェクトが広がる最大範囲（この値に達すると点滅等の演出へ移行）

    float load_timer_;     // アセットの非同期読み込み中にプログレスバーやロード演出を進行させるためのタイマー
    float load_count_;     // 読み込みが完了したアセットの数（ロード進捗率の計算用）
    bool is_load_flag_;    // 全リソースの準備が完了し、Updateによるメインロジックを開始してよいかを示すフラグ

    VECTOR pos_ = { 200, 400 }; // 画面ロード時や警告UI表示時の基準となるスクリーン2D座標
    VECTOR size_ = { 600, 100 }; // UI要素（プログレスバーや背景パネル）のピクセルサイズ
};
