#pragma once
#include"dxlib.h"
#include"Object3D.h"
#include"Texture.h"


class SphereCollider;
class CapsuleCollider;


class StageCollider :public Object3D
{
public:
	StageCollider();

	~StageCollider();

	void Draw();

	void Update();

	virtual void OnEnter(Collider* collider, Collider* check);
	virtual void OnTrigger(Collider* collider, Collider* check);
	virtual void OnExit(Collider* collider, Collider* check);

	void ColliderUpdate();
private:
	SphereCollider* mpGoBossCollider;

	
	
	Texture* mpTexture;
	Texture* mpTexture2;
	int Count;
	int Time;
};