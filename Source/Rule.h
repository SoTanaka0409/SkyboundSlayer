#pragma once
#include"Dxlib.h"
#include"Scene.h"
#include"InputManager.h"

class Rule :public Scene
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	Rule();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	~Rule();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void Initialize();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void Finalize();

private:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void HandlePageInput();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawRuleBackground();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawRulePage1();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawRulePage2();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawRuleFooter();
	int page_;
	int bg_handle1_;
	int bg_handle2_;
};
