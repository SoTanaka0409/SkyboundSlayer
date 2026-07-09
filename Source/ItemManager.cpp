#include"ItemManager.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Buff.h"
#include"BuffManager.h"

ItemManager::ItemManager()
{
}
ItemManager::~ItemManager()
{
}

void ItemManager::Update()
{
}

void ItemManager::AddItem(Item::ItemInformation* mItem)
{
    if (mItem == nullptr) return;
    if (Master::mpInfClassManager == nullptr) return;
	mbGetItemflag = true;
	for (auto itr = mItemList.begin(); itr != mItemList.end(); itr++)
	{
		switch (mItem->ID)
		{
		case Item::ItemID::NONE:
			break;
		case Item::ItemID::HEAL:
			mItem->Name = "回復薬";
			mItem->price = 100;
			break;
		case Item::ItemID::POWER:
			mItem->Name = "攻撃力UP";
			mItem->price = 100;
			break;
		case Item::ItemID::HIGHHEAL:
			mItem->Name = "高級回復薬";
			mItem->price = 300;
			break;
		case Item::ItemID::SPEED:
			mItem->Name = "スピードUP";
			mItem->price = 50;
		default:
			break;
		}
		if ((*itr)->ID == mItem->ID)
		{
			(*itr)->Count += mItem->Count;
			
			if((*mItem).isLog)Master::mpInfClassManager->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
            delete mItem; // Prevent memory leak when the item is already in the inventory.
			return;
		}
	}
	
	switch (mItem->ID)
	{
	case Item::ItemID::NONE:
		break;
	case Item::ItemID::HEAL:
		mItem->Name = "回復薬";
		mItem->price = 100;
		break;
	case Item::ItemID::POWER:
		mItem->Name = "攻撃力UP";
		mItem->price = 100;
		break;
	case Item::ItemID::HIGHHEAL:
		mItem->Name = "高級回復薬";
		mItem->price = 300;
		break;
	case Item::ItemID::SPEED:
		mItem->Name = "スピードUP";
		mItem->price = 50;
	default:
		break;
	}

	if ((*mItem).isLog)Master::mpInfClassManager->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
	mItemList.push_back(mItem);
}

void ItemManager::UseItem(Item::ItemID id)
{
	auto mpplayer = Master::mpPlayer;
    Player3D* player = Master::mpPlayer;
    if (player == nullptr) return;
	for (auto itr = mItemList.begin(); itr != mItemList.end(); itr++)
	{
		if ((*itr)->ID == id)
		{
			if ((*itr)->Count <= 0) { 
                Master::mpInfClassManager->LogList.push_back(new InfClass(400, (*itr)->Name.c_str(), 4));
			    return;
		    }

			if (id == Item::HIGHHEAL || id == Item::HEAL)Master::mpSoundManager->PlaySE(SoundManager::SE_HEAL);
			if (id == Item::POWER || id == Item::SPEED)Master::mpSoundManager->PlaySE(SoundManager::SE_POWER);
			(*itr)->Count -= 1;
			(*itr)->mbUse = true;
            Effect(id);
        }
    }
}

void ItemManager::Effect(Item::ItemID id)
{
    auto mpplayer = Master::mpPlayer;
    Player3D* player = Master::mpPlayer;
    if (player == nullptr) return;

	if (id == Item::HEAL)
	{
		player->SetHp(player->GetHp()+30.0f);
	}
	if (id == Item::POWER)
	{
		Master::mpBuffManager->AddBuff(new Buff(600, 1.5f,Object3D::StatusState::Status_Attack));
	}
	if (id == Item::HIGHHEAL)
	{
		player->SetHp(player->GetHp()+100.0f);
	}
	if (id == Item::SPEED)
	{
		Master::mpBuffManager->AddBuff(new Buff(600,2.0f,Object3D::StatusState::Status_Speed));
	}
}
