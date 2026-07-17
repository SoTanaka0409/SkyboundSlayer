#pragma once
#include "Dxlib.h"
#include "Scene.h"

class TitleScene : public Scene
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	TitleScene();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	~TitleScene();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void Initialize();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void Update();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void Draw();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void Finalize();

private:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void UpdateTitleCamera();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void HandleMenuInput();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void UpdatePromptBlink();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawSceneBackground();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawTitlePanel();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawMenuPanel();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawPrompt();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	bool IsHoverStart(int mx, int my) const;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	bool IsHoverRule(int mx, int my) const;

	float camera_angle_;
	int color_fade_;
	bool color_flag_;
};
