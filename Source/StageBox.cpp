#include"StageBox.h"


StageBox::StageBox(VECTOR centerPos,VECTOR centerPos2, VECTOR topLeft, VECTOR bottomRight,int mnColor1,int mnColor2,int mnColor3,int mnColor4)

{
	//éËëO
	{
		//ç∂è„
		mVertex[0].pos = VAdd(centerPos, topLeft);
		mVertex[0].norm = VGet(1.0f, 0.0f, 0.0f);//å„Ç≈åvéZÇ∑ÇÈ
		mVertex[0].dif = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[0].spc = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[0].u = 0.0f;
		mVertex[0].v = 0.0f;
		mVertex[0].su = 0.0f;
		mVertex[0].sv = 0.0f;

		//âEè„
		mVertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
		mVertex[1].norm = VGet(1.0f, 0.0f, 0.0f);//å„Ç≈åvéZÇ∑ÇÈ
		mVertex[1].dif = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[1].spc = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[1].u = 1.0f;
		mVertex[1].v = 0.0f;
		mVertex[1].su = 1.0f;
		mVertex[1].sv = 0.0f;

		//ç∂â∫
		mVertex[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
		mVertex[2].norm = VGet(1.0f, 0.0f, 0.0f);//å„Ç≈åvéZÇ∑ÇÈ
		mVertex[2].dif = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[2].spc = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[2].u = 0.0f;
		mVertex[2].v = 1.0f;
		mVertex[2].su = 0.0f;
		mVertex[2].sv = 1.0f;
		//âEâ∫
		mVertex[3].pos = VAdd(centerPos, bottomRight);
		mVertex[3].norm = VGet(1.0f, 0.0f, 0.0f);//å„Ç≈åvéZÇ∑ÇÈ
		mVertex[3].dif = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[3].spc = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[3].u = 1.0f;
		mVertex[3].v = 1.0f;
		mVertex[3].su = 1.0f;
		mVertex[3].sv = 1.0f;
	}
	//âú
	{
		//ç∂è„
		mVertex[4].pos = VAdd(centerPos2, topLeft);
		mVertex[4].norm = VGet(1.0f, 0.0f, 0.0f);//å„Ç≈åvéZÇ∑ÇÈ
		mVertex[4].dif = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[4].spc = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[4].u = 0.0f;
		mVertex[4].v = 0.0f;
		mVertex[4].u = 0.0f;
		mVertex[4].sv = 0.0f;

		//âEè„
		mVertex[5].pos = VAdd(centerPos2, VGet(bottomRight.x, topLeft.y, bottomRight.z));
		mVertex[5].norm = VGet(1.0f, 0.0f, 0.0f);//å„Ç≈åvéZÇ∑ÇÈ
		mVertex[5].dif = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[5].spc = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[5].u = 1.0f;
		mVertex[5].v = 0.0f;
		mVertex[5].su = 1.0f;
		mVertex[5].sv = 0.0f;

		//ç∂â∫
		mVertex[6].pos = VAdd(centerPos2, VGet(topLeft.x, bottomRight.y, topLeft.z));
		mVertex[6].norm = VGet(1.0f, 0.0f, 0.0f);//å„Ç≈åvéZÇ∑ÇÈ
		mVertex[6].dif = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[6].spc = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[6].u = 0.0f;
		mVertex[6].v = 1.0f;
		mVertex[6].su = 0.0f;
		mVertex[6].sv = 1.0f;
		//âEâ∫
		mVertex[7].pos = VAdd(centerPos2, bottomRight);
		mVertex[7].norm = VGet(1.0f, 0.0f, 0.0f);//å„Ç≈åvéZÇ∑ÇÈ
		mVertex[7].dif = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[7].spc = GetColorU8(mnColor1, mnColor2, mnColor3, mnColor4);
		mVertex[7].u = 1.0f;
		mVertex[7].v = 1.0f;
		mVertex[7].su = 1.0f;
		mVertex[7].sv = 1.0f;
	}
}
StageBox::~StageBox()
{

}

void StageBox::Draw()
{
	DrawPolygon3D(mVertex, 8, DX_NONE_GRAPH,true);

}

void StageBox::Update()
{

}


