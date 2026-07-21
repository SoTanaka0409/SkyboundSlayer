#pragma once
#include "DxLib.h"
#include "Item.h"
#include <list>
#include <map>
#include <vector>

/// @brief インベントリ内の所持アイテムの追加・使用・効果適用を一括管理するクラス
class ItemManager
{
public:
	/// @brief ItemManagerのコンストラクタ
	ItemManager();

	/// @brief ItemManagerのデストラクタ
	~ItemManager();

	/// @brief アイテム状態や使用タイマー等の毎フレーム更新処理を行う
	void Update();

	/// @brief 新しいアイテムを所持リストに追加（または所持数を増加）する
	/// @param mItem 追加するアイテム情報構造体へのポインタ
	void AddItem(Item::ItemInformation* mItem);

	/// @brief 指定したIDのアイテムを1つ消費・使用する
	/// @param id 使用するアイテムの識別ID
	void UseItem(Item::ItemID id);

	/// @brief アイテムの獲得フラグ状態を取得する
	/// @return bool アイテムを獲得した場合はtrue
	bool GetItemFlag() { return get_itemflag_; }

	/// @brief アイテム使用時の実際のステータス回復・バフ付与等の効果処理を実行する
	/// @param id 効果を適用するアイテムの識別ID
	void Effect(Item::ItemID id);

public:
	std::list<Item::ItemInformation*> item_list_; ///< 所持しているアイテム情報のリスト
	Item::ItemInformation Information;             ///< 作業・参照用アイテム情報構造体

private:
	bool get_itemflag_;                            ///< アイテム獲得イベント発生検知フラグ
};