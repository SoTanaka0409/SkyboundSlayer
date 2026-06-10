#pragma once
#include"Object3D.h"
#include"Dxlib.h"
#include<vector>


class DrawCircle1:public Object3D
{
public:
	DrawCircle1(std::string filename,VECTOR centerPos);

	~DrawCircle1();

	void Update()override;//更新
	void Draw()override;//描画

	

	std::vector<VERTEX3D>GetVertex()
	{
		std::vector<VERTEX3D>result;
		result.push_back(mVertex[0]);
		result.push_back(mVertex[1]);
		result.push_back(mVertex[2]);
		return result;



	}

private:
	int mnGraphHandle; //画像ハンドル


	VERTEX3D mVertex[3];//頂点情報(最終的に四角で描くので4つ)

	float radius;
	float Maxradius;
	const int div = 48;  // 分割数
	

	std::vector<VERTEX3D> vtx;
	

	int color = GetColor(255, 0, 0);
	VECTOR mnCenter;
	VECTOR OldPosition;
};