#include "ShortInventory.h"
#include "Master.h"
#include "InputManager.h"
#include "Config.h"
#include <iterator>

ShortInventory::ShortInventory()
	: mSelectedIndex(0)
{
}

ShortInventory::~ShortInventory()
{
}

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

int ShortInventory::GetItemCount() const
{
	if (!Master::mpItemManager)
	{
		return 0;
	}

	return (int)Master::mpItemManager->mItemList.size();
}

void ShortInventory::ClampSelectedIndex(int itemCount)
{
	if (itemCount <= 0)
	{
		mSelectedIndex = 0;
		return;
	}

	if (mSelectedIndex >= itemCount)
	{
		mSelectedIndex = 0;
	}
	else if (mSelectedIndex < 0)
	{
		mSelectedIndex = itemCount - 1;
	}
}

void ShortInventory::HandleSelectionInput(int itemCount)
{
	if (InputManager::CheckDownKey(KEY_INPUT_RIGHT))
	{
		mSelectedIndex++;
		ClampSelectedIndex(itemCount);
	}

	if (InputManager::CheckDownKey(KEY_INPUT_LEFT))
	{
		mSelectedIndex--;
		ClampSelectedIndex(itemCount);
	}
}

void ShortInventory::UseSelectedItem()
{
	Item::Information* info = GetSelectedItem();
	if (info && info->Count > 0)
	{
		Master::mpItemManager->UseItem(info->ID);
	}
}

Item::Information* ShortInventory::GetSelectedItem() const
{
	if (!Master::mpItemManager)
	{
		return nullptr;
	}

	auto it = Master::mpItemManager->mItemList.begin();
	std::advance(it, mSelectedIndex);
	if (it == Master::mpItemManager->mItemList.end())
	{
		return nullptr;
	}

	return *it;
}

void ShortInventory::Draw()
{
	int itemCount = GetItemCount();
	if (itemCount == 0)
	{
		return;
	}

	ClampSelectedIndex(itemCount);
	Item::Information* info = GetSelectedItem();
	if (!info)
	{
		return;
	}

	DrawItemPanel(info);
}

void ShortInventory::DrawItemPanel(const Item::Information* info)
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
