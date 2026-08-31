#include "ItemManager.h"
#include "Buff.h"
#include "BuffManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Player3D.h"

/// @brief ItemManagerクラスのコンストラクタ
ItemManager::ItemManager()
{
}

/// @brief ItemManagerクラスのデストラクタ
ItemManager::~ItemManager()
{
}

/// @brief アイテム状態やタイマー等の毎フレーム更新処理を行う
void ItemManager::Update()
{
}

/// @brief 新しいアイテムを所持リストに追加、または所持数を加算する
/// @param mItem 追加するアイテム情報構造体へのポインタ
void ItemManager::AddItem(Item::ItemInformation* mItem)
{
	if (mItem == nullptr) return;
	if (Master::inf_class_manager_ == nullptr) return;

	get_itemflag_ = true;

	// アイテムIDに応じた名称および基本価格の設定
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
		break;
	default:
		break;
	}

	// 既存の所持アイテムリスト内に同種アイテムが存在するか確認
	for (auto itr = item_list_.begin(); itr != item_list_.end(); ++itr)
	{
		if ((*itr)->ID == mItem->ID)
		{
			(*itr)->Count += mItem->Count;

			if (mItem->is_log_)
			{
				Master::inf_class_manager_->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
			}

			// 重複追加時はメモリリーク防止のため引数のオブジェクトを解放
			delete mItem;
			return;
		}
	}

	if (mItem->is_log_)
	{
		Master::inf_class_manager_->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
	}

	item_list_.push_back(mItem);
}

/// @brief 指定したIDのアイテムを消費・使用する
/// @param id 使用するアイテムの識別ID
void ItemManager::UseItem(Item::ItemID id)
{
	Player3D* player = Master::player_;
	if (player == nullptr) return;

	for (auto itr = item_list_.begin(); itr != item_list_.end(); ++itr)
	{
		if ((*itr)->ID == id)
		{
			if ((*itr)->Count <= 0)
			{
				Master::inf_class_manager_->LogList.push_back(new InfClass(400, (*itr)->Name.c_str(), 4));
				return;
			}

			if (id == Item::HIGHHEAL || id == Item::HEAL)
			{
				Master::sound_manager_->PlaySE(SoundManager::SE_HEAL);
				if (Master::score_manager_ != nullptr)
				{
					Master::score_manager_->AddUsedPotion();
				}
			}

			if (id == Item::POWER || id == Item::SPEED)
			{
				Master::sound_manager_->PlaySE(SoundManager::SE_POWER);
			}

			(*itr)->Count -= 1;
			(*itr)->use_ = true;
			Effect(id);
		}
	}
}

/// @brief アイテム使用時の実際の効果（HP回復、攻撃・移動速度バフ付与）を適用する
/// @param id 効果を適用するアイテムの識別ID
void ItemManager::Effect(Item::ItemID id)
{
	Player3D* player = Master::player_;
	if (player == nullptr) return;

	if (id == Item::HEAL)
	{
		player->SetHp(player->GetHp() + 30.0f);
	}
	if (id == Item::POWER)
	{
		Master::buff_manager_->AddBuff(new Buff(600, 1.5f, Object3D::StatusState::Status_Attack));
	}
	if (id == Item::HIGHHEAL)
	{
		player->SetHp(player->GetHp() + 100.0f);
	}
	if (id == Item::SPEED)
	{
		Master::buff_manager_->AddBuff(new Buff(600, 2.0f, Object3D::StatusState::Status_Speed));
	}
}