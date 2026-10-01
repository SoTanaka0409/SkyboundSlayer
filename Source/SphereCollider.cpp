#include "SphereCollider.h"
#include"CapsuleCollider.h"


/// @brief SphereColliderの初期化（コンストラクタ）
SphereCollider::SphereCollider(Object3D* parent, VECTOR center, float radius)
	: Collider(parent)
{
	m_Position = center;
	m_Radius = radius;
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
				this->m_Position,
				this->m_Radius,
				capsule->m_Position,
				capsule->m_Position2,
				capsule->m_Radius
			);

			HitCheck(check, isHit);
		}

		// 相手がスフィアの場合
		SphereCollider* sphere = dynamic_cast<SphereCollider*>(check);
		if (sphere != nullptr)
		{
			bool isHit = HitCheck_Sphere_Sphere(
				this->m_Position,
				this->m_Radius,
				sphere->m_Position,
				sphere->m_Radius
			);
		

			HitCheck(check, isHit);
		}
	}
}


/// @brief SphereColliderの描画処理
void SphereCollider::Draw()
{
	DrawSphere3D(
		m_Position,
		m_Radius,
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
