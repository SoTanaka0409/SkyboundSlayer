#pragma once
#include<vector>
#include"Object3D.h"
#include"Dxlib.h"


class StageBox
{
public:
	StageBox(VECTOR centerpos,VECTOR centorPos2,VECTOR TopLeft,VECTOR BottomRightint,int m_Color1,int m_Color2,int m_Color3,int mnColoe4);
	~StageBox();
	void Draw();
	void Update();
	std::vector<VERTEX3D>GetVertex()
	{
		std::vector<VERTEX3D>result;
  
		result.push_back(m_Vertex[0]);
		result.push_back(m_Vertex[1]);
		result.push_back(m_Vertex[2]);
		result.push_back(m_Vertex[3]);
		result.push_back(m_Vertex[4]);
		result.push_back(m_Vertex[5]);
		result.push_back(m_Vertex[6]);
  		result.push_back(m_Vertex[7]);
		return result;
	}

private:
	VERTEX3D m_Vertex[8];//頂点情報(最終的に四角で描くので4つ)

	/*int m_Color1;
	int m_Color2;
	int m_Color3;
	int m_Color4;*/

};