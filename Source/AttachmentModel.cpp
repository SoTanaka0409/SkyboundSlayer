#include"AttachmentMOdel.h"
#include"Master.h"

AttachmentModel::AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex, VECTOR offsetPos, VECTOR offsetRot)
	:Object3D(VGet(0.0f, 0.0f, 0.0f))
	, mnParentHandle(parentModelHandle)
	, mnParentFrameIndex(parentFrameIndex)
	, mOffsetPos(offsetPos)
	, mOffsetRot(offsetRot)
{
	mnHandle = Master::mpResourceManager->LoadModel(filename.c_str());

}

AttachmentModel::~AttachmentModel()
{
	MV1DeleteModel(mnHandle);
}

void AttachmentModel::Update()
{
	MATRIX parentMatrix = MV1GetFrameLocalWorldMatrix(mnParentHandle, mnParentFrameIndex);

	MATRIX offsetMatrix = MGetRotY(mOffsetRot.y);
	offsetMatrix = MMult(offsetMatrix, MGetRotX(mOffsetRot.x));
	offsetMatrix = MMult(offsetMatrix, MGetRotZ(mOffsetRot.z));
	offsetMatrix = MMult(offsetMatrix, MGetTranslate(mOffsetPos));

	MATRIX finalMatrix = MMult(offsetMatrix, parentMatrix);

	MV1SetMatrix(mnHandle, finalMatrix);
}

void AttachmentModel::Draw()
{
	MV1DrawModel(mnHandle);

}
