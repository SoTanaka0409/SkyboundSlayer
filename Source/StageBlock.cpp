#include "StageBlock.h"

StageBlock::StageBlock(VECTOR initPos, VECTOR size, unsigned int color, int fillFlag)
	: Object3D(initPos)
	, mSize(size)
	, mColor(color)
	, mFillFlag(fillFlag)
{
	// ステージ用のタグや壁用のタグなどをセットする
	SetTag(Tag3D_Stage);
}

StageBlock::~StageBlock()
{
}

void StageBlock::Update()
{
	
}

void StageBlock::Draw()
{
	// 描画フラグがOFFの場合は描画しない
	if (!IsDrawFlag()) return;

	// initPos を中心点として、XYZの各方向に mSize の半分ずつ広げた頂点を計算

	VECTOR pos1 = VGet(mvPosition.x - mSize.x / 2.0f, mvPosition.y - mSize.y / 2.0f, mvPosition.z - mSize.z / 2.0f);
	VECTOR pos2 = VGet(mvPosition.x + mSize.x / 2.0f, mvPosition.y + mSize.y / 2.0f, mvPosition.z + mSize.z / 2.0f);

	// DxLibの DrawCube3D を使って 3Dの直方体を描画
	DrawCube3D(pos1, pos2, mColor, mColor, mFillFlag);
}
