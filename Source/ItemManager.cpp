#include"ItemManager.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Buff.h"
#include"BuffManager.h"
#include"Player3D.h"


/*
 * 目的（ItemManagerのItemManager処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
ItemManager::ItemManager()
{
}
ItemManager::~ItemManager()
{
}


/*
 * 目的（ItemManagerのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ItemManager::Update()
{
}


/*
 * 目的（ItemManagerのAddItem処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ItemManager::AddItem(Item::ItemInformation* mItem)
{
    if (mItem == nullptr) return;
    if (Master::inf_class_manager_ == nullptr) return;
	get_itemflag_ = true;
	for (auto itr = item_list_.begin(); itr != item_list_.end(); itr++)
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
			
			if((*mItem).is_log_)Master::inf_class_manager_->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
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

	if ((*mItem).is_log_)Master::inf_class_manager_->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
	item_list_.push_back(mItem);
}


/*
 * 目的（ItemManagerのUseItem処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ItemManager::UseItem(Item::ItemID id)
{
	auto mpplayer = Master::player_;
    Player3D* player = Master::player_;
    if (player == nullptr) return;
	for (auto itr = item_list_.begin(); itr != item_list_.end(); itr++)
	{
		if ((*itr)->ID == id)
		{
			if ((*itr)->Count <= 0) { 
                Master::inf_class_manager_->LogList.push_back(new InfClass(400, (*itr)->Name.c_str(), 4));
			    return;
		    }

			if (id == Item::HIGHHEAL || id == Item::HEAL) {
				Master::sound_manager_->PlaySE(SoundManager::SE_HEAL);
				if (Master::score_manager_ != nullptr) {
					Master::score_manager_->AddUsedPotion();
				}
			}
			if (id == Item::POWER || id == Item::SPEED)Master::sound_manager_->PlaySE(SoundManager::SE_POWER);
			(*itr)->Count -= 1;
			(*itr)->use_ = true;
            Effect(id);
        }
    }
}


/*
 * 目的（ItemManagerのEffect処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ItemManager::Effect(Item::ItemID id)
{
    auto mpplayer = Master::player_;
    Player3D* player = Master::player_;
    if (player == nullptr) return;

	if (id == Item::HEAL)
	{
		player->SetHp(player->GetHp()+30.0f);
	}
	if (id == Item::POWER)
	{
		Master::buff_manager_->AddBuff(new Buff(600, 1.5f,Object3D::StatusState::Status_Attack));
	}
	if (id == Item::HIGHHEAL)
	{
		player->SetHp(player->GetHp()+100.0f);
	}
	if (id == Item::SPEED)
	{
		Master::buff_manager_->AddBuff(new Buff(600,2.0f,Object3D::StatusState::Status_Speed));
	}
}
