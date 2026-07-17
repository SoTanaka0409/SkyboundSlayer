#pragma once
#include<vector>
#include"Object3D.h"
#include"Dxlib.h"


class StageBox
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	StageBox(VECTOR centerpos,VECTOR centorPos2,VECTOR TopLeft,VECTOR BottomRightint,int color1_,int color2_,int color3_,int mnColoe4);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~StageBox();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();
	std::vector<VERTEX3D>GEtVertex()
	{
		std::vector<VERTEX3D>result;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[0]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[1]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[2]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[3]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[4]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[5]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[6]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[7]);
		return result;
	}

private:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VERTEX3D vertex_[8];//頂点情報(最終的に四角で描くので4つ)

	/*int color1_;
	int color2_;
	int color3_;
	int color4_;*/

};