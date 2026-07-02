#pragma once
#include"dxlib.h"
#include"Collider.h"
#include"ColliderManager.h"
#include"Object3D.h"
#include"Model.h"

class SphereCollider;
class CapsuleCollider;


class Magic:public Object3D
{
public:
	Magic(std::string filename,VECTOR initPos,float r,float damage,float speed,VECTOR movevec,int count,int time);//file,initpos,半径,攻撃力,speed,向かう方向,count,消える時間
	~Magic();

	void Update();

	void Draw();

	virtual void Move();
	virtual void Death();

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;
protected:
	SphereCollider* mpHitCollider;
	int mnGraphHandle;
	std::string Filename;
	float mfSpeed;
	float mfAttack_chara;
	float mfAttack;
	float mfMagicSize;
	VECTOR moveVec;
	int DeleteCount;
	int DeleteTime;
	bool mbDelete;
	
};