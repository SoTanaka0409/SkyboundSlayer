#include"AttachmentMOdel.h"
#include"Master.h"

AttachmentModel::AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex, VECTOR offsetPos, VECTOR offsetRot)
	:Object3D(VGet(0.0f, 0.0f, 0.0f))
	, parent_handle_(parentModelHandle)
	, parent_frame_index_(parentFrameIndex)
	, offset_pos_(offsetPos)
	, offset_rot_(offsetRot)
{
	handle_ = Master::resource_manager_->LoadModel(filename.c_str());

}

AttachmentModel::~AttachmentModel()
{
	MV1DeleteModel(handle_);
}

void AttachmentModel::Update()
{
	MATRIX parentMatrix = MV1GetFrameLocalWorldMatrix(parent_handle_, parent_frame_index_);

	MATRIX offsetMatrix = MGetRotY(offset_rot_.y);
	offsetMatrix = MMult(offsetMatrix, MGetRotX(offset_rot_.x));
	offsetMatrix = MMult(offsetMatrix, MGetRotZ(offset_rot_.z));
	offsetMatrix = MMult(offsetMatrix, MGetTranslate(offset_pos_));

	MATRIX finalMatrix = MMult(offsetMatrix, parentMatrix);

	MV1SetMatrix(handle_, finalMatrix);
}

void AttachmentModel::Draw()
{
	MV1DrawModel(handle_);

}
