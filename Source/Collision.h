#pragma once
#include "DxLib.h"
#include <vector>
#include <list>

// 衝突判定対象の識別タグ。どのグループ同士が衝突可能かを判定するフィルタリングに使用する
enum class CollisionTag
{
    Character = 1000,
};

// オブジェクトの衝突判定情報を管理する基底クラス。各種判定（球、カプセル等）はこのクラスを継承または保持する
class Collision
{
public:
    // 入力: なし / 出力: なし
    // 副作用: 衝突判定情報を初期化する
    Collision();

    // 入力: なし / 出力: なし
    // 副作用: 特になし
    virtual ~Collision();

    // 入力: なし / 出力: なし
    // 副作用: 衝突マネージャー等の更新処理に伴い、判定位置や状態を更新する
    virtual void Update();

    // 入力: なし / 出力: なし
    // 副作用: デバッグ表示として、現在の当たり判定範囲をスクリーン上に可視化する
    virtual void Draw();

    CollisionTag GetTag() const { return coll_tag_; }
    void SetTag(CollisionTag tag) { coll_tag_ = tag; }

    std::list<float> size_list_; // 判定範囲のパラメータ（複数判定を持つ場合等に使用）

private:
    CollisionTag coll_tag_;
};

