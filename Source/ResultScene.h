#pragma once
#include "Dxlib.h"
#include "Scene.h"

class ResultScene : public Scene
{
public:
   
    	ResultScene();
   
    	~ResultScene();

   
    	void Initialize();
 
    	void Update();
   
    	void Draw();
        void Finalize();
   

private:
  
/// @details リザルト画面でのユーザー入力（決定ボタン等）を検知し、タイトル画面や次ステージへの遷移トリガーを発火させる
    void HandleReturnInput();

/// @details リザルトUIの基盤となる半透明の背景ウィンドウなどを描画バッファへ登録する
    void DrawResultPanel();

/// @details 「STAGE CLEAR」などの見出しテキストや装飾を描画し、プレイヤーに結果を明示する
    void DrawResultHeader();

/// @details 討伐数やクリアタイム、獲得スコアなどの詳細な成績データを計算・整形し、パネル中央領域へ描画する
    void DrawResultStats();

/// @details 画面下部に「Press Enter to Next」などの操作ガイドを点滅描画し、次のアクションを提示する
    void DrawResultFooter();

    int all_timer_; // スコアのカウントアップ演出や、一定時間経過後の入力受付開始など、リザルト画面全体のアニメーション進行を管理するタイマー
};
