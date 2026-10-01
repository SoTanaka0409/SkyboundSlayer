#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Object3D.h"


/// @brief CapsuleColliderの初期化（コンストラクタ）
CapsuleCollider::CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius)
	: Collider(parent)
{
	m_Position = pos1;
	m_Position2 = pos2;
	m_Radius = radius;
}

CapsuleCollider::~CapsuleCollider()
{

}


/// @brief CapsuleColliderの状態更新処理
void CapsuleCollider::Update(Collider* check)
{
	if (check != nullptr)
	{
		// 相手がカプセルの場合
		CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(check);
		
		if (capsule != nullptr)
		{
			bool isHit = HitCheck_Capsule_Capsule(
				this->m_Position,
				this->m_Position2,
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
			bool isHit = HitCheck_Sphere_Capsule(
				sphere->m_Position,
				sphere->m_Radius,
				this->m_Position,
				this->m_Position2,
				this->m_Radius
			);

			HitCheck(check, isHit);
		}
	}
}


/// @brief CapsuleColliderの描画処理
void CapsuleCollider::Draw()
{
	DrawCapsule3D(
		m_Position,
		m_Position2,
		m_Radius,
		4,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}


/// @brief CapsuleColliderのOnEnter処理
void CapsuleCollider::OnEnter()
{

}


/// @brief CapsuleColliderのOnTrigger処理
void CapsuleCollider::OnTrigger()
{

}


/// @brief CapsuleColliderのOnExit処理
void CapsuleCollider::OnExit()
{

}
