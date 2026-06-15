#pragma once
#include "Object3D.h"

class StageBlock : public Object3D
{
public:
	// 中心座標、サイズ(幅・高さ・奥行き)、色、塗りつぶしフラグを指定して生成
	StageBlock(VECTOR initPos, VECTOR size, unsigned int color, int fillFlag = TRUE);
	~StageBlock();

	void Update() override;
	void Draw() override;

private:
	VECTOR mSize;
	unsigned int mColor;
	int mFillFlag; // TRUEで塗りつぶし、FALSEでワイヤーフレーム
};
