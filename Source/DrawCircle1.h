#pragma once
#include"Object3D.h"
#include"Dxlib.h"
#include<vector>


class DrawCircle1:public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	DrawCircle1(std::string filename,VECTOR centerPos);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~DrawCircle1();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update()override;//更新
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw()override;//描画

	

	std::vector<VERTEX3D>GetVertex()
	{
		std::vector<VERTEX3D>result;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[0]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[1]);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		result.push_back(vertex_[2]);
		return result;



	}

private:
	int graph_handle_; //画像ハンドル


    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VERTEX3D vertex_[3];//頂点情報(最終的に四角で描くので4つ)

	float radius;
	float Maxradius;
	const int div = 48;  // 分割数
	

	std::vector<VERTEX3D> vtx;
	

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	int color = GetColor(255, 0, 0);
	VECTOR center_;
	VECTOR OldPosition;
};