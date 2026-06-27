#include"AttachmentMOdel.h"

AttachmentModel::AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex, VECTOR offsetPos, VECTOR offsetRot)
	:Object3D(VGet(0.0f, 0.0f, 0.0f))//座標はアタッチ先によって変わるので初期化
	, mnParentHandle(parentModelHandle)
	, mnParentFrameIndex(parentFrameIndex)
	, mOffsetPos(offsetPos)
	, mOffsetRot(offsetRot)
{
	//3dモデルの読み込み
	mnHandle = MV1LoadModel(filename.c_str());

}

AttachmentModel::~AttachmentModel()
{
	//モデルの破棄
	MV1DeleteModel(mnHandle);
}

void AttachmentModel::Update()
{
	//アタッチ先のモデルのフレームの行列情報を取得
	MATRIX parentMatrix = MV1GetFrameLocalWorldMatrix(mnParentHandle, mnParentFrameIndex);

	// Offsets
	MATRIX offsetMatrix = MGetRotY(mOffsetRot.y);
	offsetMatrix = MMult(offsetMatrix, MGetRotX(mOffsetRot.x));
	offsetMatrix = MMult(offsetMatrix, MGetRotZ(mOffsetRot.z));
	offsetMatrix = MMult(offsetMatrix, MGetTranslate(mOffsetPos));

	MATRIX finalMatrix = MMult(offsetMatrix, parentMatrix);

	//上を取得した行列情報を自身に設定する
	MV1SetMatrix(mnHandle, finalMatrix);
}

void AttachmentModel::Draw()
{
	//モデルの描画
	MV1DrawModel(mnHandle);

}