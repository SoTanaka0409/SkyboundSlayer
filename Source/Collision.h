#pragma once
#include "DxLib.h"
#include"Player3D.h"
#include"EnemyManager.h"
#include"Object3D.h"
#include"vector"
#include"list"

class Collision
{
public:
	enum Tag_Coll
	{
		/*Coll_Player3D,
		Coll_Enemy3D,
		Coll_Stage3D,
		Coll_Wall3D,
		Coll_Tree3D,
		COll__Max,*/
		Coll_Charactor=1000,
	};
	//キャラクターの当たり判定を書いていく
public:
	//virtual void CallBack() = 0;

	Collision();

	~Collision();

	void Update();
	void Draw();

	/*void SphireColl();
	void CapsuleColl();*/

	Tag_Coll GetTag() { return CollTag; }
	void SetTag(Tag_Coll tag) { CollTag = tag; }
	
	std::list<float>mSizeList;
private:
	//Object3D* mpObject3D;
	Tag_Coll CollTag;
  /*  Player3D* Coll_P;
	Enemy* Coll_E;*/
	

	
};

class SphererCollider
{
public:

private:
	VECTOR mvPOsition;
	float mfRadius;
	Object3D* mpObject;
};