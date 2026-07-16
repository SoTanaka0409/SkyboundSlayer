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
	int GetItemCount() const;
	void ClampSelectedIndex(int itemCount);
	void HandleSelectionInput(int itemCount);
	void UseSelectedItem();
	Item::Information* GetSelectedItem() const;
	void DrawItemPanel(const Item::Information* info);

	int mSelectedIndex;
};
