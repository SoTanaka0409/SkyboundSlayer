#pragma once
#include"Object3D.h"
#include<string>
#include"Debugh.h"



class Stage :public Object3D
{
public:
	Stage(VECTOR initPos,std::string StageModelName, std::string stageCollsionModelName, VECTOR scale = VGet(-1.0f, -1.0f, -1.0f));

	~Stage();

	void Update()override;

	void Draw()override;

	//ステージとカプセル型との当たり判定
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	//ステージと線分との当たり判定
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);
	VECTOR CheckHit_LineDebug(VECTOR pos1, VECTOR pos2);


private:
	Debug* mpDebug;
	int mnModelHandle; //ステージモデルのハンドル
	int mnCollisionHandle;;//ステージの子リジョンモデルのハンドル



};