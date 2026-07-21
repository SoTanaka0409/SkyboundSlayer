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
        if (instance_ == nullptr)
        {
            instance_ = new ColliderManager();
        }

        return instance_;
    }

    static void Finalize()
    {
        if (instance_ != nullptr)
        {
            delete instance_;
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

    //// 指定したタグのコライダーのリストを取得
    //// note: 該当するオブジェクトが複数ある場合、リスト化して全てのオブジェクトを返す

private:
    std::list<Collider*> collider_list_;    // コライダーを管理するリスト

    static ColliderManager* instance_;
};