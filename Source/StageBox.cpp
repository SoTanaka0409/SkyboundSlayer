#include"StageBox.h"



/// @brief StageBoxの初期化（コンストラクタ）
StageBox::StageBox(VECTOR centerPos,VECTOR centerPos2, VECTOR topLeft, VECTOR bottomRight,int color1_,int color2_,int color3_,int color4_)

{
	//手前
	{
		//左上
		vertex_[0].pos = VAdd(centerPos, topLeft);
		vertex_[0].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		vertex_[0].dif = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[0].spc = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[0].u = 0.0f;
		vertex_[0].v = 0.0f;
		vertex_[0].su = 0.0f;
		vertex_[0].sv = 0.0f;

		//右上
		vertex_[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
		vertex_[1].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		vertex_[1].dif = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[1].spc = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[1].u = 1.0f;
		vertex_[1].v = 0.0f;
		vertex_[1].su = 1.0f;
		vertex_[1].sv = 0.0f;

		//左下
		vertex_[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
		vertex_[2].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		vertex_[2].dif = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[2].spc = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[2].u = 0.0f;
		vertex_[2].v = 1.0f;
		vertex_[2].su = 0.0f;
		vertex_[2].sv = 1.0f;
		//右下
		vertex_[3].pos = VAdd(centerPos, bottomRight);
		vertex_[3].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		vertex_[3].dif = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[3].spc = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[3].u = 1.0f;
		vertex_[3].v = 1.0f;
		vertex_[3].su = 1.0f;
		vertex_[3].sv = 1.0f;
	}
	//奥
	{
		//左上
		vertex_[4].pos = VAdd(centerPos2, topLeft);
		vertex_[4].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		vertex_[4].dif = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[4].spc = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[4].u = 0.0f;
		vertex_[4].v = 0.0f;
		vertex_[4].u = 0.0f;
		vertex_[4].sv = 0.0f;

		//右上
		vertex_[5].pos = VAdd(centerPos2, VGet(bottomRight.x, topLeft.y, bottomRight.z));
		vertex_[5].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		vertex_[5].dif = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[5].spc = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[5].u = 1.0f;
		vertex_[5].v = 0.0f;
		vertex_[5].su = 1.0f;
		vertex_[5].sv = 0.0f;

		//左下
		vertex_[6].pos = VAdd(centerPos2, VGet(topLeft.x, bottomRight.y, topLeft.z));
		vertex_[6].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		vertex_[6].dif = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[6].spc = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[6].u = 0.0f;
		vertex_[6].v = 1.0f;
		vertex_[6].su = 0.0f;
		vertex_[6].sv = 1.0f;
		//右下
		vertex_[7].pos = VAdd(centerPos2, bottomRight);
		vertex_[7].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
		vertex_[7].dif = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[7].spc = GetColorU8(color1_, color2_, color3_, color4_);
		vertex_[7].u = 1.0f;
		vertex_[7].v = 1.0f;
		vertex_[7].su = 1.0f;
		vertex_[7].sv = 1.0f;
	}
}
StageBox::~StageBox()
{

}


/// @brief StageBoxの描画処理
void StageBox::Draw()
{
	DrawPolygon3D(vertex_, 8, DX_NONE_GRAPH,true);

}


/// @brief StageBoxの状態更新処理
void StageBox::Update()
{

}


