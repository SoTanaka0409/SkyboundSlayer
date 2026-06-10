#pragma once
#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(Object3D* parent, VECTOR center, float radius);
	virtual ~SphereCollider();

	void Update(Collider* check) override;
	void Draw() override;

	void OnEnter() override;
	void OnTrigger() override;
	void OnExit() override;

};