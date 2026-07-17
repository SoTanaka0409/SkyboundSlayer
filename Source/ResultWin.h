#pragma once
#include "Dxlib.h"
#include "Scene.h"

class ResultWin : public Scene
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	ResultWin();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	~ResultWin();

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
    	void HandleReturnInput();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawResultPanel();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawResultHeader();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawResultStats();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	void DrawResultFooter();

	int all_timer_;
};
