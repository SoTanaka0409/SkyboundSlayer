#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Object3D.h"

CapsuleCollider::CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius)
	: Collider(parent)
{
	position_ = pos1;
	position2_ = pos2;
	radius_ = radius;
}

CapsuleCollider::~CapsuleCollider()
{

}

void CapsuleCollider::Update(Collider* check)
{
	if (check != nullptr)
	{
		// 相手がカプセルの場合
		CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(check);
		
		if (capsule != nullptr)
		{
			bool isHit = HitCheck_Capsule_Capsule(
				this->position_,
				this->position2_,
				this->radius_,
				capsule->position_,
				capsule->position2_,
				capsule->radius_
			);

			HitCheck(check, isHit);
		}

		// 相手がスフィアの場合
		SphereCollider* sphere = dynamic_cast<SphereCollider*>(check);
		if (sphere != nullptr)
		{
			bool isHit = HitCheck_Sphere_Capsule(
				sphere->position_,
				sphere->radius_,
				this->position_,
				this->position2_,
				this->radius_
			);

			HitCheck(check, isHit);
		}
	}
}

void CapsuleCollider::Draw()
{
	DrawCapsule3D(
		position_,
		position2_,
		radius_,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

void CapsuleCollider::OnEnter()
{

}

void CapsuleCollider::OnTrigger()
{

}

void CapsuleCollider::OnExit()
{

}
