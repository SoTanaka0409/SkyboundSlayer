#include"StageBox.h"



/// @brief StageBoxの初期化（コンストラクタ）
StageBox::StageBox(VECTOR centerPos,VECTOR centerPos2, VECTOR topLeft, VECTOR bottomRight,int m_Color1,int m_Color2,int m_Color3,int m_Color4)

{
	// 手前
	{
		//左上
		m_Vertex[0].pos = VAdd(centerPos, topLeft);
		m_Vertex[0].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		m_Vertex[0].dif = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[0].spc = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[0].u = 0.0f;
		m_Vertex[0].v = 0.0f;
		m_Vertex[0].su = 0.0f;
		m_Vertex[0].sv = 0.0f;

		//右上
		m_Vertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
		m_Vertex[1].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		m_Vertex[1].dif = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[1].spc = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[1].u = 1.0f;
		m_Vertex[1].v = 0.0f;
		m_Vertex[1].su = 1.0f;
		m_Vertex[1].sv = 0.0f;

		//左下
		m_Vertex[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
		m_Vertex[2].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		m_Vertex[2].dif = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[2].spc = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[2].u = 0.0f;
		m_Vertex[2].v = 1.0f;
		m_Vertex[2].su = 0.0f;
		m_Vertex[2].sv = 1.0f;
		//右下
		m_Vertex[3].pos = VAdd(centerPos, bottomRight);
		m_Vertex[3].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		m_Vertex[3].dif = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[3].spc = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[3].u = 1.0f;
		m_Vertex[3].v = 1.0f;
		m_Vertex[3].su = 1.0f;
		m_Vertex[3].sv = 1.0f;
	}
	// 奥
	{
		//左上
		m_Vertex[4].pos = VAdd(centerPos2, topLeft);
		m_Vertex[4].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		m_Vertex[4].dif = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[4].spc = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[4].u = 0.0f;
		m_Vertex[4].v = 0.0f;
		m_Vertex[4].u = 0.0f;
		m_Vertex[4].sv = 0.0f;

		//右上
		m_Vertex[5].pos = VAdd(centerPos2, VGet(bottomRight.x, topLeft.y, bottomRight.z));
		m_Vertex[5].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		m_Vertex[5].dif = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[5].spc = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[5].u = 1.0f;
		m_Vertex[5].v = 0.0f;
		m_Vertex[5].su = 1.0f;
		m_Vertex[5].sv = 0.0f;

		//左下
		m_Vertex[6].pos = VAdd(centerPos2, VGet(topLeft.x, bottomRight.y, topLeft.z));
		m_Vertex[6].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		m_Vertex[6].dif = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[6].spc = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[6].u = 0.0f;
		m_Vertex[6].v = 1.0f;
		m_Vertex[6].su = 0.0f;
		m_Vertex[6].sv = 1.0f;
		//右下
		m_Vertex[7].pos = VAdd(centerPos2, bottomRight);
		m_Vertex[7].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		m_Vertex[7].dif = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[7].spc = GetColorU8(m_Color1, m_Color2, m_Color3, m_Color4);
		m_Vertex[7].u = 1.0f;
		m_Vertex[7].v = 1.0f;
		m_Vertex[7].su = 1.0f;
		m_Vertex[7].sv = 1.0f;
	}
}
StageBox::~StageBox()
{

}


/// @brief StageBoxの描画処理
void StageBox::Draw()
{
	DrawPolygon3D(m_Vertex, 8, DX_NONE_GRAPH,true);

}


/// @brief StageBoxの状態更新処理
void StageBox::Update()
{

}


