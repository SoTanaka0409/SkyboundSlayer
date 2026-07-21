#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

// プレイヤーが取得・使用可能な消費アイテムの基底データと、インベントリでの振る舞いを管理するクラス
class Item
{
public:
    // アイテムの種類を定義。追加時はUIアイコン配列や判定ロジックとの不整合を防ぐため必ずITEM_MAXの前に追記する
    enum ItemID
    {
        NONE,
        HEAL,
        POWER,
        HIGHHEAL,
        SPEED,
        ITEM_MAX, // 種類数の算出や、全アイテムを走査する際のループ終端条件（番兵）として使用する
    };

    // UI表示や効果判定に必要なメタデータを束ね、マネージャー側でのデータ受け渡し負荷を軽減する構造体
    struct ItemInformation
    {
        int Count;           // インベントリ内でのスタック数（所持上限の超過判定に使用）
        std::string Name;    // UIのアイテム詳細欄に表示するためのローカライズ名称
        ItemID ID;           // アイテム効果を特定し、使用時のロジック分岐に用いる一意の識別子
        std::string effect;  // プレイヤーに効果を明示するためのフレーバーテキスト
        bool use_ = false;   // 現在このアイテムの効果が適用中か（バフの多重使用制限など）を示すフラグ
        int price = 100;     // ショップでの売買時、ゲームの経済バランスを崩さないための基準価格
        bool is_log_ = true; // 取得・使用時に、画面端のシステムログへ通知テキストを流すかどうかの制御フラグ
    };

public:
/// @details 未初期化のゴミデータによる不正なアイテム使用を防ぐため、安全な初期値（NONE等）をセットアップする
    Item();

/// @details なし（ポインタ等の動的リソースを持たないため、特殊な破棄処理は行わずデフォルトに任せる）
    ~Item();

/// @details 一時的なステータス上昇アイテム（SPEEDなど）の効果時間カウントダウンを進行させる
    void Update();

/// @details プレイヤーのインベントリ画面やショートカットスロット上に、自身のアイコンを描画バッファへ登録する
    void Draw();

/// @param count (新しい所持数)
/// @details 構造体内の所持数(Count)を上書きする
    // 宝箱からの複数個同時取得や、ショップでのまとめ買い時のインベントリ数同期に使用する
    void SetItemCount(int count) { Inf.Count = count; }

/// @return 現在の所持数(int)
    // アイテム使用時に「1個以上持っているか」という消費判定の事前チェックに用いる
    int GetItemCount() { return Inf.Count; }

/// @return アイテム名(std::string)
    // 取得時のシステムログ表示や、インベントリUIの名称欄を構築するために提供する
    std::string GetItemName() { return Inf.Name; }

/// @param num (アイテムID)
/// @details 内部識別子(Num)の更新
    // アイテムのドロップ時など、インスタンスを使い回して別のアイテムに変化させる際に使用する
    void SetItem(ItemID num) { Num = num; }

/// @return アイテムID(ItemID)
    // インベントリからアイテムを使用した際、プレイヤーにどの効果（回復、バフ等）を適用するかを判別する
    ItemID GetItem() { return Num; }

private:
    ItemID Num;          // 検索や比較処理を高速に行うため、文字列ではなく列挙型(Enum)で管理する識別子
    ItemInformation Inf; // 外部からの不正なパラメータ改ざんを防ぐためカプセル化された実データ
};