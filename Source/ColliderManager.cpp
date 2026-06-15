#include "ColliderManager.h"
#include "Collider.h"
#include"Master.h"

// 静的メンバ変数定義
ColliderManager* ColliderManager::Instance = nullptr;


ColliderManager::ColliderManager()
{

}

ColliderManager::~ColliderManager()
{

}

// 更新
void ColliderManager::Update()
{
    for (auto itr = mColliderList.begin(); itr != mColliderList.end(); ++itr)
    {
        auto itr_check = itr;
        ++itr_check;
        for (; itr_check != mColliderList.end(); ++itr_check)
        {
            (*itr)->Update((*itr_check));
            (*itr_check)->Update((*itr));
        }
    }
}

// 描画
void ColliderManager::Draw()
{
    for (auto itr = mColliderList.begin(); itr != mColliderList.end(); itr++)
    {
        if (Master::mpDebug->Getdebug())
        {
            (*itr)->Draw();
        }
    }
}

// Colliderオブジェクトの追加
void ColliderManager::AddCollider(Collider* Collider)
{
    mColliderList.push_back(Collider);
}

// Colliderオブジェクトの全削除
void ColliderManager::DeleteAllCollider()
{
    for (auto itr = mColliderList.begin(); itr != mColliderList.end(); /*ここは空っぽなので注意*/)
    {
        Collider* temp = *itr;

        // リストから削除
        itr = mColliderList.erase(itr);

        // オブジェクトそのものを削除
        delete temp;
        temp = nullptr;
    }
}

// 削除する必要のあるオブジェクトがあれば削除する
void ColliderManager::DeleteAllColliderIfNeeded()
{
    for (auto itr = mColliderList.begin(); itr != mColliderList.end(); /*ここは空っぽなので注意*/)
    {
        // 破棄フラグが立っていれば削除する
        if ((*itr)->IsDeleteFlag())
        {
            Collider* temp = *itr;

            // リストから削除
            itr = mColliderList.erase(itr);

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