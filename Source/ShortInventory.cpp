#include "ShortInventory.h"
#include "Master.h"
#include "InputManager.h"
#include "ScreenSize.h"
#include "Config.h"

ShortInventory::ShortInventory() 
    : mSelectedIndex(0)
{

}

ShortInventory::~ShortInventory()
{
}

void ShortInventory::Update()
{
    if (!Master::mpItemManager) return;

    int itemCount = Master::mpItemManager->mItemList.size();
    if (itemCount == 0) return;

    if (InputManager::CheckDownKey(KEY_INPUT_RIGHT))
    {
        mSelectedIndex++;
        if (mSelectedIndex >= itemCount) mSelectedIndex = 0;
    }
    if (InputManager::CheckDownKey(KEY_INPUT_LEFT))
    {
        mSelectedIndex--;
        if (mSelectedIndex < 0) mSelectedIndex = itemCount - 1;
    }

    // Use Item (UP arrow)
    if (InputManager::CheckDownKey(KEY_INPUT_UP))
    {
        auto it = Master::mpItemManager->mItemList.begin();
        std::advance(it, mSelectedIndex);
        if (it != Master::mpItemManager->mItemList.end())
        {
            Item::Information* info = *it;
            if (info && info->Count > 0)
            {
                Master::mpItemManager->UseItem(info->ID);
            }
        }
    }
}

void ShortInventory::Draw()
{
    if (!Master::mpItemManager) return;
    
    int itemCount = Master::mpItemManager->mItemList.size();
    if (itemCount == 0) return;

    // Boundary check just in case
    if (mSelectedIndex >= itemCount) mSelectedIndex = 0;

    auto it = Master::mpItemManager->mItemList.begin();
    std::advance(it, mSelectedIndex);
    if (it != Master::mpItemManager->mItemList.end())
    {
        Item::Information* info = *it;
        if (info)
        {
            // Draw background or box
            int boxX = Config::ScreenWidth - 250;
            int boxY = Config::ScreenHeight - 100;
            DrawBox(boxX, boxY, boxX + 230, boxY + 80, GetColor(0, 0, 0), TRUE);
            DrawBox(boxX, boxY, boxX + 230, boxY + 80, GetColor(255, 255, 255), FALSE);

            // Draw Item Name and Count
            DrawFormatString(boxX + 40, boxY + 20, GetColor(255, 255, 255), "%s", info->Name.c_str());
            DrawFormatString(boxX + 40, boxY + 50, GetColor(255, 255, 255), "x %d", info->Count);

            // Draw arrows to indicate scrollable
            DrawFormatString(boxX + 10, boxY + 30, GetColor(200, 200, 0), "<-");
            DrawFormatString(boxX + 200, boxY + 30, GetColor(200, 200, 0), "->");
            DrawFormatString(boxX + 10, boxY + 5, GetColor(100, 255, 100), "[UP] Use");
        }
    }
}
