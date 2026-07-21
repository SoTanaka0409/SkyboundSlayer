#include "ShortInventory.h"
#include "Master.h"
#include "InputManager.h"
#include "Config.h"
#include <iterator>


/// @brief ShortInventoryの初期化（コンストラクタ）
ShortInventory::ShortInventory()
	: selected_index_(0)
{
}

ShortInventory::~ShortInventory()
{
}


/// @brief ShortInventoryの状態更新処理
void ShortInventory::Update()
{
	int itemCount = GetItemCount();
	if (itemCount == 0)
	{
		return;
	}

	ClampSelectedIndex(itemCount);
	HandleSelectionInput(itemCount);

	if (InputManager::CheckDownKey(KEY_INPUT_R))
	{
		UseSelectedItem();
	}
}


/// @brief ShortInventoryのGetItemCount処理
int ShortInventory::GetItemCount() const
{
	if (!Master::item_manager_)
	{
		return 0;
	}

	return (int)Master::item_manager_->item_list_.size();
}


/// @brief ShortInventoryのClampSelectedIndex処理
void ShortInventory::ClampSelectedIndex(int itemCount)
{
	if (itemCount <= 0)
	{
		selected_index_ = 0;
		return;
	}

	if (selected_index_ >= itemCount)
	{
		selected_index_ = 0;
	}
	else if (selected_index_ < 0)
	{
		selected_index_ = itemCount - 1;
	}
}


/// @brief ShortInventoryのHandleSelectionInput処理
void ShortInventory::HandleSelectionInput(int itemCount)
{
	if (InputManager::CheckDownKey(KEY_INPUT_RIGHT))
	{
		selected_index_++;
		ClampSelectedIndex(itemCount);
	}

	if (InputManager::CheckDownKey(KEY_INPUT_LEFT))
	{
		selected_index_--;
		ClampSelectedIndex(itemCount);
	}
}


/// @brief ShortInventoryのUseSelectedItem処理
void ShortInventory::UseSelectedItem()
{
	Item::ItemInformation* info = GetSelectedItem();
	if (info && info->Count > 0)
	{
		Master::item_manager_->UseItem(info->ID);
	}
}


/// @brief ShortInventoryのGetSelectedItem処理
Item::ItemInformation* ShortInventory::GetSelectedItem() const
{
	if (!Master::item_manager_)
	{
		return nullptr;
	}

	auto it = Master::item_manager_->item_list_.begin();
	std::advance(it, selected_index_);
	if (it == Master::item_manager_->item_list_.end())
	{
		return nullptr;
	}

	return *it;
}


/// @brief ShortInventoryの描画処理
void ShortInventory::Draw()
{
	int itemCount = GetItemCount();
	if (itemCount == 0)
	{
		return;
	}

	ClampSelectedIndex(itemCount);
	Item::ItemInformation* info = GetSelectedItem();
	if (!info)
	{
		return;
	}

	DrawItemPanel(info);
}


/// @brief ShortInventoryのDrawItemPanel処理
void ShortInventory::DrawItemPanel(const Item::ItemInformation* info)
{
	const int boxW = 300;
	const int boxH = 74;
	const int boxX = Config::ScreenWidth - boxW - 28;
	const int boxY = Config::ScreenHeight - boxH - 28;
	const int panel = GetColor(18, 17, 20);
	const int panelLight = GetColor(38, 35, 38);
	const int gold = GetColor(198, 154, 64);
	const int goldDark = GetColor(98, 73, 32);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(boxX - 6, boxY - 4, boxX + boxW + 6, boxY + boxH + 6, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawBox(boxX, boxY, boxX + boxW, boxY + boxH, panel, TRUE);
	DrawBox(boxX + 6, boxY + 6, boxX + boxW - 6, boxY + 12, panelLight, TRUE);
	DrawLine(boxX, boxY, boxX + boxW, boxY, gold, 1);
	DrawLine(boxX, boxY + boxH, boxX + boxW, boxY + boxH, goldDark, 1);
	DrawLine(boxX, boxY, boxX, boxY + boxH, goldDark, 1);
	DrawLine(boxX + boxW, boxY, boxX + boxW, boxY + boxH, gold, 1);

	DrawFormatString(boxX + 18, boxY + 12, GetColor(245, 226, 174), "ITEM");
	DrawFormatString(boxX + 78, boxY + 12, GetColor(238, 238, 238), "%s", info->Name.c_str());
	DrawFormatString(boxX + 78, boxY + 42, GetColor(205, 210, 216), "x %d", info->Count);
	DrawFormatString(boxX + 18, boxY + 42, GetColor(100, 255, 100), "R USE");
	DrawFormatString(boxX + 238, boxY + 42, GetColor(200, 200, 0), "< >");
}
