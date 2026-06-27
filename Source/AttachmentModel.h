#pragma once
#pragma once
#include"DxLib.h"
#include"Object3D.h"

class AttachmentModel :public Object3D
{
public:
	//filename モデルのファイルパス
	//parentModelHandle くっつけたいモデルのハンドル
	//parentFrameIndex  くっつけたいモデル(おやモデル）のハンドル番号

	AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex, VECTOR offsetPos = VGet(0.0f, 0.0f, 0.0f), VECTOR offsetRot = VGet(0.0f, 0.0f, 0.0f));

	~AttachmentModel();

	void Update() override;

	void Draw()override;

	//モデルハンドルの取得
	int GetHandle() { return mnHandle; }

private:
	int mnHandle; //モデルのハンドル
	int mnParentHandle; //親のモデルハンドル
	int mnParentFrameIndex; //親モデルのハンドル番号
	VECTOR mOffsetPos;
	VECTOR mOffsetRot;

};
