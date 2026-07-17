#pragma once
#include"dxlib.h"
#include"string"
#include"Object2D.h"
#include"Texture.h"
#include"iostream"
#include"vector"
class Combo:public Object2D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Combo();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Combo();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void HitAddCombo();

private:
	int combo_;
	int handle;
	Texture* texture0_;
	Texture* texture1_;
	Texture* texture2_;
	Texture* texture3_;
	Texture* texture4_;
	Texture* texture5_;
	Texture* texture6_;
	Texture* texture7_;
	Texture* texture8_;
	Texture* texture9_;

};