#include "ColliderManager.h"
#include "Collider.h"
#include"Master.h"
#include "Player3D.h"

/// @brief 静的メンバ変数定義
ColliderManager* ColliderManager::m_Instance = nullptr;


ColliderManager::ColliderManager()
{

}

ColliderManager::~ColliderManager()
{

}

/// @brief 更新
void ColliderManager::Update()
{
    for (auto itr = m_ColliderList.begin(); itr != m_ColliderList.end(); ++itr)
    {
        auto itr_check = itr;
        ++itr_check;
        for (; itr_check != m_ColliderList.end(); ++itr_check)
        {
            (*itr)->Update((*itr_check));
            (*itr_check)->Update((*itr));
        }
    }
}

/// @brief 描画
void ColliderManager::Draw()
{
    if (!Master::m_Debug->Getdebug()) return;

    VECTOR refPos = VGet(0, 0, 0);
    bool useCull = false;
    if (Master::m_Player) {
        refPos = Master::m_Player->GetPosition();
        useCull = true;
    }

    for (auto itr = m_ColliderList.begin(); itr != m_ColliderList.end(); itr++)
    {
        if (useCull) {
            // プレイヤーから距離が一定以上のコライダーはデバッグ描画を省略（重力・処理負荷軽減）
            float distSq = VSquareSize(VSub((*itr)->m_Position, refPos));
            if (distSq > 3000.0f * 3000.0f) {
                continue;
            }
        }
        (*itr)->Draw();
    }
}

/// @brief Colliderオブジェクトの追加
void ColliderManager::AddCollider(Collider* Collider)
{
    m_ColliderList.push_back(Collider);
}

/// @brief Colliderオブジェクトの全削除
void ColliderManager::DeleteAllCollider()
{
    for (auto itr = m_ColliderList.begin(); itr != m_ColliderList.end(); /*ここは空っぽなので注意*/)
    {
        Collider* temp = *itr;

        // リストから削除
        itr = m_ColliderList.erase(itr);

        // オブジェクトそのものを削除
        delete temp;
        temp = nullptr;
    }
}

/// @brief 削除する必要のあるオブジェクトがあれば削除する
void ColliderManager::DeleteAllColliderIfNeeded()
{
    for (auto itr = m_ColliderList.begin(); itr != m_ColliderList.end(); /*ここは空っぽなので注意*/)
    {
        // 破棄フラグが立っていれば削除する
        if ((*itr)->IsDeleteFlag())
        {
            Collider* temp = *itr;

            // リストから削除
            itr = m_ColliderList.erase(itr);

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