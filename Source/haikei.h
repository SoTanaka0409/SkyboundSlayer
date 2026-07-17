#pragma once
#pragma once
#include"DxLib.h"
#include"Texture.h"
#include"Object2D.h"
#include"SceneManager.h"




class haikei : public Object2D
{

public:
	//コンストラクタ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	haikei(VECTOR initPos, std::string filename);
	//デストラクタ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~haikei();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update()override;//更新

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw()override;//描画
private:
	Texture* texture_;
};