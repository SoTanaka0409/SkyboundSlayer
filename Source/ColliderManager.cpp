#include "ColliderManager.h"
#include "Collider.h"
#include"Master.h"

// 静的メンバ変数定義
ColliderManager* ColliderManager::instance_ = nullptr;


ColliderManager::ColliderManager()
{

}

ColliderManager::~ColliderManager()
{

}

// 更新
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void ColliderManager::Update()
{
    for (auto itr = collider_list_.begin(); itr != collider_list_.end(); ++itr)
    {
        auto itr_check = itr;
        ++itr_check;
        for (; itr_check != collider_list_.end(); ++itr_check)
        {
            (*itr)->Update((*itr_check));
            (*itr_check)->Update((*itr));
        }
    }
}

// 描画
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void ColliderManager::Draw()
{
    for (auto itr = collider_list_.begin(); itr != collider_list_.end(); itr++)
    {
        if (Master::debug_->Getdebug())
        {
            (*itr)->Draw();
        }
    }
}

// Colliderオブジェクトの追加
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void ColliderManager::AddCollider(Collider* Collider)
{
    collider_list_.push_back(Collider);
}

// Colliderオブジェクトの全削除
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void ColliderManager::DeleteAllCollider()
{
    for (auto itr = collider_list_.begin(); itr != collider_list_.end(); /*ここは空っぽなので注意*/)
    {
        Collider* temp = *itr;

        // リストから削除
        itr = collider_list_.erase(itr);

        // オブジェクトそのものを削除
        delete temp;
        temp = nullptr;
    }
}

// 削除する必要のあるオブジェクトがあれば削除する
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void ColliderManager::DeleteAllColliderIfNeeded()
{
    for (auto itr = collider_list_.begin(); itr != collider_list_.end(); /*ここは空っぽなので注意*/)
    {
        // 破棄フラグが立っていれば削除する
        if ((*itr)->IsDeleteFlag())
        {
            Collider* temp = *itr;

            // リストから削除
            itr = collider_list_.erase(itr);

            // オブジェクトそのものを削除
            delete temp;
            temp = nullptr;
        }
        else
        {
            // 次の要素へ進める
            itr++;
        }
    }
}