#pragma once
#include"DxLib.h"
#include"Object3D.h"
#include<vector>


class Wall :public Object3D
{
public:
	//centerPos,,,中心座標
	//topLeft... 左上座標
	//bottomRight...右下座標
	Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight);//コンストラクタ

	~Wall();//デストラクタ
	void Update()override;//更新
	void Draw()override;//描画

	std::vector<VERTEX3D>GetVertex()
	{
		std::vector<VERTEX3D>result;
		result.push_back(vertex_[0]);
		result.push_back(vertex_[1]);
		result.push_back(vertex_[2]);
		result.push_back(vertex_[3]);
		return result;



	}

private:
	int graph_handle_; //画像ハンドル

	VERTEX3D vertex_[4];//頂点情報(最終的に四角で描くので4つ)








};