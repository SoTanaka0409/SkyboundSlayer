#include"AttachmentMOdel.h"

AttachmentModel::AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex, VECTOR offsetPos, VECTOR offsetRot)
	:Object3D(VGet(0.0f, 0.0f, 0.0f))//蠎ｧ讓吶・繧｢繧ｿ繝・メ蜈医↓繧医▲縺ｦ螟峨ｏ繧九・縺ｧ蛻晄悄蛹・
	, mnParentHandle(parentModelHandle)
	, mnParentFrameIndex(parentFrameIndex)
	, mOffsetPos(offsetPos)
	, mOffsetRot(offsetRot)
{
	//3d繝｢繝・Ν縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	mnHandle = MV1LoadModel(filename.c_str());

}

AttachmentModel::~AttachmentModel()
{
	//繝｢繝・Ν縺ｮ遐ｴ譽・
	MV1DeleteModel(mnHandle);
}

void AttachmentModel::Update()
{
	//繧｢繧ｿ繝・メ蜈医・繝｢繝・Ν縺ｮ繝輔Ξ繝ｼ繝縺ｮ陦悟・諠・ｱ繧貞叙蠕・
	MATRIX parentMatrix = MV1GetFrameLocalWorldMatrix(mnParentHandle, mnParentFrameIndex);

	// Offsets
	MATRIX offsetMatrix = MGetRotY(mOffsetRot.y);
	offsetMatrix = MMult(offsetMatrix, MGetRotX(mOffsetRot.x));
	offsetMatrix = MMult(offsetMatrix, MGetRotZ(mOffsetRot.z));
	offsetMatrix = MMult(offsetMatrix, MGetTranslate(mOffsetPos));

	MATRIX finalMatrix = MMult(offsetMatrix, parentMatrix);

	//荳翫ｒ蜿門ｾ励＠縺溯｡悟・諠・ｱ繧定・霄ｫ縺ｫ險ｭ螳壹☆繧・
	MV1SetMatrix(mnHandle, finalMatrix);
}

void AttachmentModel::Draw()
{
	//繝｢繝・Ν縺ｮ謠冗判
	MV1DrawModel(mnHandle);

}