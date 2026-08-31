#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius);
	virtual ~CapsuleCollider();
	void Update(Collider* check) override;
	void Draw() override;
	void OnEnter() override;
	void OnTrigger() override;
	void OnExit() override;

};