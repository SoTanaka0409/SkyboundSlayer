#pragma once
#include"DxLib.h"
#include"Object3D.h"
#include"string"
#include"Model.h"

class Bush :public Object3D
{
public:
	Bush(std::string StageModelName,VECTOR initPos,int size);

	~Bush();

	void Draw()override;

	void Update()override;
	//ステージとカプセル型との当たり判定
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	//ステージと線分との当たり判定
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);
	VECTOR CheckHit_LineDebug(VECTOR pos1, VECTOR pos2);

	float GetSize() { return mnSize; }

private:
	Model* mpModel;
	int mnModelHandle; //ステージモデルのハンドル
	int mnCollisionHandle;;//ステージの子リジョンモデルのハンドル
	float mnSize;

};