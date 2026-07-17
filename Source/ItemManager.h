#pragma once
#include"Item.h"
#include"vector"
#include"list"
#include<map>

class ItemManager
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	ItemManager();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~ItemManager();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddItem(Item::ItemInformation* mItem);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void UseItem(Item::ItemID id);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool GetItemFlag() { return get_itemflag_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Effect(Item::ItemID id);


	std::list<Item::Information*>item_list_;
	Item::ItemInformation Information;
private:

	bool get_itemflag_;



};