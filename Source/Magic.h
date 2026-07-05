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
	Magic(std::string filename,VECTOR initPos,float r,float damage,float speed,VECTOR movevec,int count,int time);//file,initpos,蜊雁ｾ・謾ｻ謦・鴨,speed,蜷代°縺・婿蜷・count,豸医∴繧区凾髢・
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
	float speed_;
	float mfAttack_chara;
	float attack_;
	float mfMagicSize;
	VECTOR moveVec;
	int DeleteCount;
	int DeleteTime;
	bool mbDelete;
	
};