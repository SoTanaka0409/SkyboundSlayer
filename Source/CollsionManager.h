#pragma once
#include"dxlib.h"
#include"Object3D.h"
#include"Collision.h"

class CollsionManager
{
public:
	CollsionManager();

	~CollsionManager();

	void Update();

	void CollSphere(VECTOR center1, float centerR, VECTOR pos1, VECTOR pos2, float R);
	void CollCapsule(VECTOR center1, float centerR, VECTOR pos1, VECTOR pos2, float R);

	void AddCollObject(Collision* coll);

	std::vector<Collision*>GetObject3DCollListByTag(Collision::Tag_Coll tag);
	void Draw();
private:
	std::list<Object3D*>mCollList;
	Object3D* mpObject3D;
	std::list<Collision*>mColl;
	bool mbOkCollision;
	bool Hit;
	
};