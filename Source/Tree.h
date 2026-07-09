#pragma once
#include"Dxlib.h"
#include "string"
#include"Object3D.h"
#include"Model.h"
class SphereCollider;
class CapsuleCollider;

class Tree :public Object3D
{
public:
	Tree(std::string filename,VECTOR initPos,float Size,float getSize,bool HitFlag);

	~Tree();

	void Draw();

	void Update();

	float GetSize() { return mfSize; }

	virtual void OnEnter(Collider* collider, Collider* check)override;
	virtual void OnTrigger(Collider* collider, Collider* check)override;
	virtual void OnExit(Collider* collider, Collider* check)override;


	bool GetHitFlag() { return mnHitFlag; }
private:
	
	CapsuleCollider* mpCapsuleCollider;
	
	Model* model_;
	float mfSize;
	int mnModelHandle; //繧ｹ繝・・繧ｸ繝｢繝・Ν縺ｮ繝上Φ繝峨Ν
	bool mnHitFlag;
};