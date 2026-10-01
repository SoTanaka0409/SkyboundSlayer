#include"AttachmentMOdel.h"
#include"Master.h"

AttachmentModel::AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex, VECTOR offsetPos, VECTOR offsetRot)
	:Object3D(VGet(0.0f, 0.0f, 0.0f))
	, m_ParentHandle(parentModelHandle)
	, m_ParentFrameIndex(parentFrameIndex)
	, m_OffsetPos(offsetPos)
	, m_OffsetRot(offsetRot)
{
	m_Handle = Master::m_ResourceManager->LoadModel(filename.c_str());

}

AttachmentModel::~AttachmentModel()
{
	MV1DeleteModel(m_Handle);
}

void AttachmentModel::Update()
{
	MATRIX parentMatrix = MV1GetFrameLocalWorldMatrix(m_ParentHandle, m_ParentFrameIndex);

	MATRIX offsetMatrix = MGetRotY(m_OffsetRot.y);
	offsetMatrix = MMult(offsetMatrix, MGetRotX(m_OffsetRot.x));
	offsetMatrix = MMult(offsetMatrix, MGetRotZ(m_OffsetRot.z));
	offsetMatrix = MMult(offsetMatrix, MGetTranslate(m_OffsetPos));

	MATRIX finalMatrix = MMult(offsetMatrix, parentMatrix);

	MV1SetMatrix(m_Handle, finalMatrix);
}

void AttachmentModel::Draw()
{
	MV1DrawModel(m_Handle);

}
