#include "Collider.h"
#include "Object3D.h"
#include "ColliderManager.h"


/*
 * 目的（ColliderのCollider処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Collider::Collider(Object3D* parent)
	: parent_object_(parent)
	, position_(VGet(0.0f, 0.0f, 0.0f))
	, position2_(VGet(0.0f, 0.0f, 0.0f))
	, radius_(0.0f)
	, delete_flag_(false)
{
	// ColliderManagerに Add しておく
	ColliderManager::GetInstance()->AddCollider(this);
}

Collider::~Collider()
{

}


/*
 * 目的（ColliderのHitCheck処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Collider::HitCheck(Collider* check, bool isHit)
{
	
	if (isHit)
	{
		
		// 当たっていた場合 //

		// すでに当たっているかチェック
		auto itr = std::find_if(
			collision_list_.begin(),
			collision_list_.end(),
			[&](Collider* col) { return col == check; } // ラムダ式
		);

		if (itr != collision_list_.end())
		{
			// すでに当たっていた場合 //
			
			// 当たっている状態の処理を呼び出す
			this->parent_object_->OnEnter(this, check);
		}
		else
		{
			// すでに当たっていなかった場合 //

			// リストに登録しておく
			collision_list_.push_back(check);//任意のタイミングでしか追加しないようにすっれば

			// 当たった瞬間状態の処理を呼び出す
			this->parent_object_->OnTrigger(this, check);
			
		}
	}
	else
	{
		// 当たっていなかった場合 //

		// すでに当たっているかチェック
		auto itr = std::find_if(
			collision_list_.begin(),
			collision_list_.end(),
			[&](Collider* col) { return col == check; } // ラムダ式
		);

		if (itr != collision_list_.end())
		{
			// 当たっていた場合 //

			// 離れた瞬間の処理を呼び出す
			this->parent_object_->OnExit(this, check);

			// 当たっていないのでリストからは除外する
			collision_list_.erase(itr);
		}
	}
}


/*
 * 目的（ColliderのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Collider::Update(Collider* check)
{

}


/*
 * 目的（ColliderのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Collider::Draw()
{

}


/*
 * 目的（ColliderのOnEnter処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Collider::OnEnter()
{

}


/*
 * 目的（ColliderのOnTrigger処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Collider::OnTrigger()
{

}


/*
 * 目的（ColliderのOnExit処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Collider::OnExit()
{

}