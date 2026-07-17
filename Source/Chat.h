#pragma once
#include"dxlib.h"
#include"string"
#include"Texture.h"

class Chat
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Chat();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Chat();
	
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw(std::string chatdate);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Reset();
private: 
	Texture* texture_;
	int first_font_;
	int fontsize;
};