#include "Collider.h"
#include "Object3D.h"
#include "ColliderManager.h"

Collider::Collider(Object3D* parent)
	: mpParentObject(parent)
	, mvPosition(VGet(0.0f, 0.0f, 0.0f))
	, mvPosition2(VGet(0.0f, 0.0f, 0.0f))
	, mfRadius(0.0f)
	, mbDeleteFlag(false)
{
	// ColliderManagerに Add しておく
	ColliderManager::GetInstance()->AddCollider(this);
}

Collider::~Collider()
{

}

void Collider::HitCheck(Collider* check, bool isHit)
{
	
	if (isHit)
	{
		
		// 当たっていた場合 //

		// すでに当たっているかチェック
		auto itr = std::find_if(
			mCollisionList.begin(),
			mCollisionList.end(),
			[&](Collider* col) { return col == check; } // ラムダ式
		);

		if (itr != mCollisionList.end())
		{
			// すでに当たっていた場合 //
			
			// 当たっている状態の処理を呼び出す
			this->mpParentObject->OnEnter(this, check);
		}
		else
		{
			// すでに当たっていなかった場合 //

			// リストに登録しておく
			mCollisionList.push_back(check);//任意のタイミングでしか追加しないようにすっれば

			// 当たった瞬間状態の処理を呼び出す
			this->mpParentObject->OnTrigger(this, check);
			
		}
	}
	else
	{
		// 当たっていなかった場合 //

		// すでに当たっているかチェック
		auto itr = std::find_if(
			mCollisionList.begin(),
			mCollisionList.end(),
			[&](Collider* col) { return col == check; } // ラムダ式
		);

		if (itr != mCollisionList.end())
		{
			// 当たっていた場合 //

			// 離れた瞬間の処理を呼び出す
			this->mpParentObject->OnExit(this, check);

			// 当たっていないのでリストからは除外する
			mCollisionList.erase(itr);
		}
	}
}

void Collider::Update(Collider* check)
{

}

void Collider::Draw()
{

}

void Collider::OnEnter()
{

}

void Collider::OnTrigger()
{

}

void Collider::OnExit()
{

}