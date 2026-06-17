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
		HEAL,//‰ñ•œ
		POWER,//‚¿‚©‚çUP
		HIGHHEAL,//‰ñ•œ‹­
		SPEED,
		ITEM_MAX,
	};
	typedef struct ItemInformation
	{
		int Count;
		std::string Name;
		ItemID ID;
		std::string effect;
		bool mbUse = false;
		int price;
		bool isLog = true;//true‚¾‚Á‚½‚çƒƒO‚Éc‚·

	}Information;


public:
	Item();
	~Item();

	void Update();

	void Draw();
	void SetItemCount(int count) { Inf.Count = count; }
	int GetItemCount() { return Inf.Count; }
	std::string GetItemName() { return Inf.Name; }
	void SetItem(ItemID  num) { Num = num; }
	ItemID GetItem() { return Num; }

private:
	ItemID Num;
	ItemInformation Inf;



};