#pragma once
#pragma once
#include "DxLib.h"
#include <vector>

class Object3D;

class Collider
{
public:
	Collider(Object3D* parent);
	virtual ~Collider();

	virtual void Update(Collider* check);
	virtual void Draw();

	virtual void OnEnter();
	virtual void OnTrigger();
	virtual void OnExit();

	void HitCheck(Collider* check, bool isHit);

	void SetDeleteFlag(bool flag) { mbDeleteFlag = flag; }
	bool IsDeleteFlag() { return mbDeleteFlag; }

public:
	Object3D* mpParentObject;

	VECTOR mvPosition;
	VECTOR mvPosition2;
	float mfRadius;

	bool mbDeleteFlag;

protected:
	std::vector<Collider*> mCollisionList;	// è’ìÀÇµÇƒÇ¢ÇÈColliderÇÃÉäÉXÉg
	
};