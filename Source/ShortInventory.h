#pragma once
#include "DxLib.h"
#include "ItemManager.h"
#include "Item.h"

class ShortInventory
{
public:
	ShortInventory();
	~ShortInventory();

	void Update();
	void Draw();

private:
	int mSelectedIndex;
};
