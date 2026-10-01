#include "Collider.h"
#include "Object3D.h"
#include "ColliderManager.h"


/// @brief Colliderの初期化（コンストラクタ）
Collider::Collider(Object3D* parent)
	: m_ParentObject(parent)
	, m_Position(VGet(0.0f, 0.0f, 0.0f))
	, m_Position2(VGet(0.0f, 0.0f, 0.0f))
	, m_Radius(0.0f)
	, m_DeleteFlag(false)
{
	// ColliderManagerに Add しておく
	ColliderManager::GetInstance()->AddCollider(this);
}

Collider::~Collider()
{

}


/// @brief ColliderのHitCheck処理
void Collider::HitCheck(Collider* check, bool isHit)
{
	
	if (isHit)
	{
		
		// 当たっていた場合 //

		// すでに当たっているかチェック
		auto itr = std::find_if(
			m_CollisionList.begin(),
			m_CollisionList.end(),
			[&](Collider* col) { return col == check; } // ラムダ式
		);

		if (itr != m_CollisionList.end())
		{
			// すでに当たっていた場合 //
			
			// 当たっている状態の処理を呼び出す
			this->m_ParentObject->OnEnter(this, check);
		}
		else
		{
			// すでに当たっていなかった場合 //

			// リストに登録しておく
			m_CollisionList.push_back(check);//任意のタイミングでしか追加しないようにすっれば

			// 当たった瞬間状態の処理を呼び出す
			this->m_ParentObject->OnTrigger(this, check);
			
		}
	}
	else
	{
		// 当たっていなかった場合 //

		// すでに当たっているかチェック
		auto itr = std::find_if(
			m_CollisionList.begin(),
			m_CollisionList.end(),
			[&](Collider* col) { return col == check; } // ラムダ式
		);

		if (itr != m_CollisionList.end())
		{
			// 当たっていた場合 //

			// 離れた瞬間の処理を呼び出す
			this->m_ParentObject->OnExit(this, check);

			// 当たっていないのでリストからは除外する
			m_CollisionList.erase(itr);
		}
	}
}


/// @brief Colliderの状態更新処理
void Collider::Update(Collider* check)
{

}


/// @brief Colliderの描画処理
void Collider::Draw()
{

}


/// @brief ColliderのOnEnter処理
void Collider::OnEnter()
{

}


/// @brief ColliderのOnTrigger処理
void Collider::OnTrigger()
{

}


/// @brief ColliderのOnExit処理
void Collider::OnExit()
{

}