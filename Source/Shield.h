#pragma once
#include"Dxlib.h"
#include<String>
#include"Object3D.h"
#include"Model.h"

class Shield :public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Shield(std::string filename,VECTOR initPos,int hp);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Shield();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	int GetHp() { return hp_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetHp(int hp) { hp_ = hp; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetSize() { return size_s_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool GetNew() { return NewShield; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetNew(bool New) { NewShield = New; }
private:
	Model* model_;
	
	int hp_;
	float size_s_;//シールドのサイズ
	bool NewShield;//新しく生成されているか

};