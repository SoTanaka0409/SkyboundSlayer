#pragma once
#pragma once
#include"DxLib.h"
#include"Object3D.h"

class AttachmentModel :public Object3D
{
public:
	//filename 繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繝代せ
	//parentModelHandle 縺上▲縺､縺代◆縺・Δ繝・Ν縺ｮ繝上Φ繝峨Ν
	//parentFrameIndex  縺上▲縺､縺代◆縺・Δ繝・Ν(縺翫ｄ繝｢繝・Ν・峨・繝上Φ繝峨Ν逡ｪ蜿ｷ

	AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex, VECTOR offsetPos = VGet(0.0f, 0.0f, 0.0f), VECTOR offsetRot = VGet(0.0f, 0.0f, 0.0f));

	~AttachmentModel();

	void Update() override;

	void Draw()override;

	//繝｢繝・Ν繝上Φ繝峨Ν縺ｮ蜿門ｾ・
	int GetHandle() { return mnHandle; }

private:
	int mnHandle; //繝｢繝・Ν縺ｮ繝上Φ繝峨Ν
	int mnParentHandle; //隕ｪ縺ｮ繝｢繝・Ν繝上Φ繝峨Ν
	int mnParentFrameIndex; //隕ｪ繝｢繝・Ν縺ｮ繝上Φ繝峨Ν逡ｪ蜿ｷ
	VECTOR mOffsetPos;
	VECTOR mOffsetRot;

};
