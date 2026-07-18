#pragma once
#include "Dxlib.h"
#include "Scene.h"

// ゲーム起動時のタイトルシーン。メニュー選択、カメラ演出、および「Press Start」等のUI表示を管理する
class TitleScene : public Scene
{
public:
    // 入力: なし / 出力: なし / 副作用: タイトルシーン固有の変数初期化を行う
    TitleScene();

    // 入力: なし / 出力: なし / 副作用: 確保したリソースを解放する
    ~TitleScene();

    // 入力: なし / 出力: なし
    // 副作用: タイトル用画像や背景音楽のロード、カメラ演出の初期化を行う
    void Initialize();

    // 入力: なし / 出力: なし
    // 副作用: カメラ演出の更新、メニュー入力の監視、および点滅UIタイマーの進行を行う
    void Update();

    // 入力: なし / 出力: なし
    // 副作用: 背景、タイトルロゴ、メニュー項目、操作ガイドなどの描画バッファへの登録を行う
    void Draw();

    // 入力: なし / 出力: なし
    // 副作用: シーン遷移時に画像ハンドル等を破棄し、VRAMメモリリークを防ぐ
    void Finalize();

private:
    // 入力: なし / 出力: なし / 副作用: タイトルロゴ周りを旋回するカメラの角度(camera_angle_)を更新し、視覚演出を進行させる
    void UpdateTitleCamera();

    // 入力: なし / 出力: なし / 副作用: ゲーム開始やルール説明などの選択肢に対するユーザー入力を検知し、遷移先を決定する
    void HandleMenuInput();

    // 入力: なし / 出力: なし / 副作用: 「Press Start」等のテキストのアルファ値を増減させ、点滅演出を進行させる
    void UpdatePromptBlink();

    // 入力: なし / 出力: なし / 副作用: シーン全体の背景画像または3D空間を描画する
    void DrawSceneBackground();

    // 入力: なし / 出力: なし / 副作用: タイトルロゴ画像をスクリーン中央へ描画する
    void DrawTitlePanel();

    // 入力: なし / 出力: なし / 副作用: 「Start」「Rule」等のメニューリストを描画する
    void DrawMenuPanel();

    // 入力: なし / 出力: なし / 副作用: 点滅中であれば操作ガイドのテキストをスクリーン下部へ描画する
    void DrawPrompt();

    // 入力: mx, my (マウス座標) / 出力: ホバー中か(bool) / 副作用: なし
    // マウスカーソルが「Start」ボタン範囲内にあるかを判定し、視覚フィードバック（拡大や色変更）に用いる
    bool IsHoverStart(int mx, int my) const;

    // 入力: mx, my (マウス座標) / 出力: ホバー中か(bool) / 副作用: なし
    // マウスカーソルが「Rule」ボタン範囲内にあるかを判定する
    bool IsHoverRule(int mx, int my) const;

    float camera_angle_; // タイトル画面を一周するカメラの現在の回転角
    int color_fade_;     // 点滅演出やフェードイン・アウト用の現在のアルファ値
    bool color_flag_;    // 点滅演出の増減方向（加算か減算か）を切り替えるフラグ
};