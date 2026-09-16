#include "SphereCollider.h"
#include"CapsuleCollider.h"


/// @brief SphereColliderの初期化（コンストラクタ）
SphereCollider::SphereCollider(Object3D* parent, VECTOR center, float radius)
	: Collider(parent)
{
	position_ = center;
	radius_ = radius;
}

SphereCollider::~SphereCollider()
{

}


/// @brief SphereColliderの状態更新処理
void SphereCollider::Update(Collider* check)
{
	if (check != nullptr)
	{
		// 相手がカプセルの場合
		CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(check);
		if (capsule != nullptr)
		{
			bool isHit = HitCheck_Sphere_Capsule(
				this->position_,
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
			bool isHit = HitCheck_Sphere_Sphere(
				this->position_,
				this->radius_,
				sphere->position_,
				sphere->radius_
			);
		

			HitCheck(check, isHit);
		}
	}
}


/// @brief SphereColliderの描画処理
void SphereCollider::Draw()
{
	DrawSphere3D(
		position_,
		radius_,
		4,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}


/// @brief SphereColliderのOnEnter処理
void SphereCollider::OnEnter()
{

}


/// @brief SphereColliderのOnTrigger処理
void SphereCollider::OnTrigger()
{

}


/// @brief SphereColliderのOnExit処理
void SphereCollider::OnExit()
{

}
