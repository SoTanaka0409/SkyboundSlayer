#include "SphereCollider.h"
#include"CapsuleCollider.h"

SphereCollider::SphereCollider(Object3D* parent, VECTOR center, float radius)
	: Collider(parent)
{
	mvPosition = center;
	mfRadius = radius;
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Update(Collider* check)
{
	if (check != nullptr)
	{
		// 相手がカプセルの場合
		CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(check);
		if (capsule != nullptr)
		{
			bool isHit = HitCheck_Sphere_Capsule(
				this->mvPosition,
				this->mfRadius,
				capsule->mvPosition,
				capsule->mvPosition2,
				capsule->mfRadius
			);

			HitCheck(check, isHit);
		}

		// 相手がスフィアの場合
		SphereCollider* sphere = dynamic_cast<SphereCollider*>(check);
		if (sphere != nullptr)
		{
			bool isHit = HitCheck_Sphere_Sphere(
				this->mvPosition,
				this->mfRadius,
				sphere->mvPosition,
				sphere->mfRadius
			);
		

			HitCheck(check, isHit);
		}
	}
}

void SphereCollider::Draw()
{
	DrawSphere3D(
		mvPosition,
		mfRadius,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

void SphereCollider::OnEnter()
{

}

void SphereCollider::OnTrigger()
{

}

void SphereCollider::OnExit()
{

}
