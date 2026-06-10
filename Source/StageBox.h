#pragma once
#include<vector>
#include"Object3D.h"
#include"Dxlib.h"


class StageBox
{
public:
	StageBox(VECTOR centerpos,VECTOR centorPos2,VECTOR TopLeft,VECTOR BottomRightint,int mnColor1,int mnColor2,int mnColor3,int mnColoe4);

	~StageBox();

	void Draw();

	void Update();
	std::vector<VERTEX3D>GEtVertex()
	{
		std::vector<VERTEX3D>result;
		result.push_back(mVertex[0]);
		result.push_back(mVertex[1]);
		result.push_back(mVertex[2]);
		result.push_back(mVertex[3]);
		result.push_back(mVertex[4]);
		result.push_back(mVertex[5]);
		result.push_back(mVertex[6]);
		result.push_back(mVertex[7]);
		return result;
	}

private:
	VERTEX3D mVertex[8];//頂点情報(最終的に四角で描くので4つ)

	/*int mnColor1;
	int mnColor2;
	int mnColor3;
	int mnColor4;*/

};