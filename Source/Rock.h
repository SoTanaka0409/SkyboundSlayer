#pragma once
#include"dxlib.h"
#include"Object3D.h"
#include"string"
#include"Model.h"

class SphereCollider;
class CapsuleCollider;


class Rock:public Object3D
{
public:
	Rock(std::string filename,VECTOR pos,float r,float High,float size);
	~Rock();

	void Draw();

	void Update();

	virtual void OnEnter(Collider* collider, Collider* check);
	virtual void OnTrigger(Collider* collider, Collider* check);
	virtual void OnExit(Collider* collider, Collider* check);
private:
	Model* model_;
	CapsuleCollider* mpCapsuleCollider;
	float mfHigh;
};