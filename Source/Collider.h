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
	void SetDeleteFlag(bool flag) { m_DeleteFlag = flag; }
	bool IsDeleteFlag() { return m_DeleteFlag; }

public:
	Object3D* m_ParentObject;

	VECTOR m_Position;
	VECTOR m_Position2;
	float m_Radius;

	bool m_DeleteFlag;

protected:
	std::vector<Collider*> m_CollisionList;	// 衝突しているColliderのリスト
	
};