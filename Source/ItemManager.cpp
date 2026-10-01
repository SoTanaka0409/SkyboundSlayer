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
	for (auto item : m_ItemList)
	{
		delete item;
	}
	m_ItemList.clear();
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
	if (Master::m_InfClassManager == nullptr) return;

	m_GetItemflag = true;

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
	for (auto itr = m_ItemList.begin(); itr != m_ItemList.end(); ++itr)
	{
		if ((*itr)->ID == mItem->ID)
		{
			(*itr)->Count += mItem->Count;

			if (mItem->m_IsLog)
			{
				Master::m_InfClassManager->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
			}

			// 重複追加時はメモリリーク防止のため引数のオブジェクトを解放
			delete mItem;
			return;
		}
	}

	if (mItem->m_IsLog)
	{
		Master::m_InfClassManager->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
	}

	m_ItemList.push_back(mItem);
}

/// @brief 指定したIDのアイテムを消費・使用する
/// @param id 使用するアイテムの識別ID
void ItemManager::UseItem(Item::ItemID id)
{
	Player3D* player = Master::m_Player;
	if (player == nullptr) return;

	for (auto itr = m_ItemList.begin(); itr != m_ItemList.end(); ++itr)
	{
		if ((*itr)->ID == id)
		{
			if ((*itr)->Count <= 0)
			{
				Master::m_InfClassManager->LogList.push_back(new InfClass(400, (*itr)->Name.c_str(), 4));
				return;
			}

			if (id == Item::HIGHHEAL || id == Item::HEAL)
			{
				Master::m_SoundManager->PlaySE(SoundManager::SE_HEAL);
				if (Master::m_ScoreManager != nullptr)
				{
					Master::m_ScoreManager->AddUsedPotion();
				}
			}

			if (id == Item::POWER || id == Item::SPEED)
			{
				Master::m_SoundManager->PlaySE(SoundManager::SE_POWER);
			}

			(*itr)->Count -= 1;
			(*itr)->m_Use = true;
			Effect(id);
		}
	}
}

/// @brief アイテム使用時の実際の効果（HP回復、攻撃・移動速度バフ付与）を適用する
/// @param id 効果を適用するアイテムの識別ID
void ItemManager::Effect(Item::ItemID id)
{
	Player3D* player = Master::m_Player;
	if (player == nullptr) return;

	if (id == Item::HEAL)
	{
		player->SetHp(player->GetHp() + 30.0f);
	}
	if (id == Item::POWER)
	{
		Master::m_BuffManager->AddBuff(new Buff(600, 1.5f, Object3D::StatusState::Status_Attack));
	}
	if (id == Item::HIGHHEAL)
	{
		player->SetHp(player->GetHp() + 100.0f);
	}
	if (id == Item::SPEED)
	{
		Master::m_BuffManager->AddBuff(new Buff(600, 2.0f, Object3D::StatusState::Status_Speed));
	}
}