#pragma once
#include"Dxlib.h"
#include"Item.h"
#include"vector"
#include"Object2D.h"

class InfClass:public Object2D
{
public:
	
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	InfClass(int Log_timer,std::string Inf,int num);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetPosition(VECTOR pos) { position_ = pos; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool GetElaseFlag() { return elase_flag_; }

private:
	int LogTime;
	std::string LogDate;//logを保存する
	int LogCount;//ログ場所の移動の貯めのカウント
	bool elase_flag_;//ewaseするためのフラグ

	int LogNumber;//使用するログの種類を選択
	
	
};