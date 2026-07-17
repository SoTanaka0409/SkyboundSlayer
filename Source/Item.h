#pragma once
#pragma once
#include"dxlib.h"
#include<vector>
#include"String"

class Item
{
public:
	enum ItemID
	{
		NONE,
		HEAL,//回復
		POWER,//ちからUP
		HIGHHEAL,//回復強
		SPEED,
		ITEM_MAX,
	};
	typedef struct ItemInformation
	{
		int Count;
		std::string Name;
		ItemID ID;
		std::string effect;
		bool use_ = false;
		int price=100;
		bool is_log_ = true;//trueだったらログに残す

	}Information;


public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Item();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Item();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetItemCount(int count) { Inf.Count = count; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	int GetItemCount() { return Inf.Count; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	std::string GetItemName() { return Inf.Name; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetItem(ItemID  num) { Num = num; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	ItemID GetItem() { return Num; }

private:
	ItemID Num;
	ItemInformation Inf;



};