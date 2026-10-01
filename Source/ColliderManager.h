#pragma once
#include <list>

class Collider;

class ColliderManager
{
public:
    ColliderManager();
    ~ColliderManager();
    void Update();
    void Draw();

    static ColliderManager* GetInstance()
    {
        if (m_Instance == nullptr)
        {
            m_Instance = new ColliderManager();
        }

        return m_Instance;
    }

    static void Finalize()
    {
        if (m_Instance != nullptr)
        {
            delete m_Instance;
        }
    }

public:
    /// @brief コライダー追加
    void AddCollider(Collider* Collider);

    /// @brief コライダー全削除
    void DeleteAllCollider();

    /// @brief 削除する必要のあるオブジェクトがあれば削除する
    /// @brief note: 全てのオブジェクトの更新が終わった後に呼び出す
    void DeleteAllColliderIfNeeded();

    //// 指定したタグのコライダーを取得
    //// note: 該当するオブジェクトが複数ある場合、最初に見つけたオブジェクトを返す

    //// 指定したタグのコライダーのリストを取得
    //// note: 該当するオブジェクトが複数ある場合、リスト化して全てのオブジェクトを返す

private:
    std::list<Collider*> m_ColliderList;    // コライダーを管理するリスト

    static ColliderManager* m_Instance;
};