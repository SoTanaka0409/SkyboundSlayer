#pragma once
#include"Item.h"
#include"vector"
#include"list"
#include<map>

class ItemManager
{
public:
	ItemManager();

	~ItemManager();

	void Update();

	void AddItem(Item::ItemInformation* mItem);
	void UseItem(Item::ItemID id);

	bool GetItemFlag() { return mbGetItemflag; }
	void Effect(Item::ItemID id);


	std::list<Item::Information*>mItemList;
	Item::ItemInformation Information;
private:

	bool mbGetItemflag;



};