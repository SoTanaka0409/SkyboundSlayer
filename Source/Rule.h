#pragma once
#include "Dxlib.h"
#include "Scene.h"
#include "InputManager.h"

// ゲームの遊び方や操作説明を表示するチュートリアル（ルール）シーン。複数ページの表示と入力遷移を管理する
class Rule : public Scene
{
public:
/// @details シーン遷移時の基底セットアップを行う
    Rule();

/// @details 派生クラスで確保したリソースを安全に解放するため、仮想デストラクタとして定義する
    ~Rule();

/// @details ルール説明用の画像ハンドルのVRAMロードや、現在のページ番号の初期化を行う
    void Initialize();

/// @details 現在のページ番号(page_)に応じて、背景・対応する説明コンテンツ・ナビゲーションUIを描画する
    void Draw();

/// @details プレイヤーの入力状態を毎フレーム監視し、ページ送りや前のシーン（タイトル等）への遷移を処理する
    void Update();

/// @details シーン離脱時に画像ハンドル等の動的リソースを破棄し、VRAMのメモリリークを防ぐ
    void Finalize();

private:
/// @details 左右キーやボタン入力によるページ遷移(page_の増減)と、境界値（範囲外アクセス防止）のクランプ処理を行う
    void HandlePageInput();

/// @details 全ページで共通となる背景を描画し、ページ切り替え時の視覚的な途切れやチラつきを防ぐ
    void DrawRuleBackground();

/// @details 1ページ目（基本操作など）の専用画像やテキストをバッファへ登録する
    void DrawRulePage1();

/// @details 2ページ目（応用システムなど）の専用画像やテキストをバッファへ登録する
    void DrawRulePage2();

/// @details 画面下部に「次へ」「戻る」などの操作ガイドを描画し、ユーザーに現在可能なアクションを明示する
    void DrawRuleFooter();

    int page_;         // 現在表示している操作説明のページインデックス（入力に応じて変動する）
    int bg_handle1_;   // 1ページ目のコンテンツを表示するための画像ハンドル
    int bg_handle2_;   // 2ページ目のコンテンツを表示するための画像ハンドル
};