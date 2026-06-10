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
        if (Instance == nullptr)
        {
            Instance = new ColliderManager();
        }

        return Instance;
    }

    static void Finalize()
    {
        if (Instance != nullptr)
        {
            delete Instance;
        }
    }

public:
    // コライダー追加
    void AddCollider(Collider* Collider);

    // コライダー全削除
    void DeleteAllCollider();

    // 削除する必要のあるオブジェクトがあれば削除する
    // note: 全てのオブジェクトの更新が終わった後に呼び出す
    void DeleteAllColliderIfNeeded();

    //// 指定したタグのコライダーを取得
    //// note: 該当するオブジェクトが複数ある場合、最初に見つけたオブジェクトを返す
    //Collider* GetColliderByTag(Collider::Tag tag);

    //// 指定したタグのコライダーのリストを取得
    //// note: 該当するオブジェクトが複数ある場合、リスト化して全てのオブジェクトを返す
    //std::vector<Collider*> GetColliderListByTag(Collider::Tag tag);

private:
    std::list<Collider*> mColliderList;    // コライダーを管理するリスト

    static ColliderManager* Instance;
};