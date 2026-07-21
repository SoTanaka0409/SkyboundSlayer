#pragma once
#include"dxlib.h"
#include"Object2D.h"
#include"Object3D.h"

// プレイヤーの所持金（通貨・スコア）をカプセル化し、不正な値の混入や増減時のUI演出を一元管理するクラス
class HaveMoneyClass
{
public:
/// @param money (初期所持金)
/// @details 所持金管理クラスを初期化し、初期値のセットアップを行う
    HaveMoneyClass(int money);

/// @details なし（動的確保したリソースが追加された際、メモリリークを防ぐために明示的に破棄する）
    ~HaveMoneyClass();

/// @details 所持金が増減した際の、数字がパラパラと切り替わるようなUIのカウントアニメーションを進行させる
    void Update();

/// @details 現在の所持金（またはアニメーション中の数値）をスクリーン座標上にUIとしてバッファへ登録する
    void Draw();

/// @param money (獲得額)
/// @details 敵撃破時などに呼び出し、値のオーバーフロー（カンスト超えによる負数化バグ）をガードしつつ加算する
    void AddMoney(int money);

/// @param money (消費額)
/// @details ショップ購入時などに呼び出し、値のアンダーフロー（所持金がマイナスになるバグ）を防ぎつつ減算する
    void PullMoney(int money);

/// @return 現在の所持金(int)
    // ショップでのアイテム購入時など、要求コストに対する支払い能力（残高）があるかの判定判定に使用する
    int HaveMoney() { return money_; }

private:
    int money_;          // 意図せぬ外部からの直接書き換えや不正計算を防ぐため、カプセル化された実際の所持金額
    Object3D* parent_;   // 3D空間上で財布の持ち主（プレイヤー等）の頭上にUIを追従させるための親オブジェクト参照
};