#include "SphereCollider.h"
#include"CapsuleCollider.h"


/*
 * 目的（SphereColliderのSphereCollider処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
SphereCollider::SphereCollider(Object3D* parent, VECTOR center, float radius)
	: Collider(parent)
{
	position_ = center;
	radius_ = radius;
}

SphereCollider::~SphereCollider()
{

}


/*
 * 目的（SphereColliderのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（SphereColliderのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SphereCollider::Draw()
{
	DrawSphere3D(
		position_,
		radius_,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}


/*
 * 目的（SphereColliderのOnEnter処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SphereCollider::OnEnter()
{

}


/*
 * 目的（SphereColliderのOnTrigger処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SphereCollider::OnTrigger()
{

}


/*
 * 目的（SphereColliderのOnExit処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SphereCollider::OnExit()
{

}
