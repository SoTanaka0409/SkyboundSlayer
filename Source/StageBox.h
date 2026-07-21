#pragma once
#include<vector>
#include"Object3D.h"
#include"Dxlib.h"


class StageBox
{
public:
	StageBox(VECTOR centerpos,VECTOR centorPos2,VECTOR TopLeft,VECTOR BottomRightint,int color1_,int color2_,int color3_,int mnColoe4);
	~StageBox();
	void Draw();
	void Update();
	std::vector<VERTEX3D>GetVertex()
	{
		std::vector<VERTEX3D>result;
  
		result.push_back(vertex_[0]);
		result.push_back(vertex_[1]);
		result.push_back(vertex_[2]);
		result.push_back(vertex_[3]);
		result.push_back(vertex_[4]);
		result.push_back(vertex_[5]);
		result.push_back(vertex_[6]);
  		result.push_back(vertex_[7]);
		return result;
	}

private:
	VERTEX3D vertex_[8];//頂点情報(最終的に四角で描くので4つ)

	/*int color1_;
	int color2_;
	int color3_;
	int color4_;*/

};