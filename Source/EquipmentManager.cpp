#include "EquipmentManager.h"
#include "InfClass.h"
#include "Master.h"

/// @brief EquipmentManagerクラスのコンストラクタ
EquipmentManager::EquipmentManager()
{
}

/// @brief EquipmentManagerクラスのデストラクタ
EquipmentManager::~EquipmentManager()
{
}

/// @brief 装備関連の描画処理を行う
void EquipmentManager::Draw()
{
}

/// @brief 装備状態やタイマー等の毎フレーム更新処理を行う
void EquipmentManager::Update()
{
}

/// @brief 新しい装備データをリストに追加、または既存の同種装備の性能を上書き更新する
/// @param date 追加・更新する装備データ構造体へのポインタ
void EquipmentManager::AddEquipment(Equipment::EquipmentDate* date)
{
	for (auto itr = date_list_.begin(); itr != date_list_.end(); ++itr)
	{
		if ((*itr)->id == date->id)
		{
			if ((*itr)->damage < date->damage)
			{
				(*itr)->damage = date->damage;
				if (date->is_log_)
				{
					Master::inf_class_manager_->LogList.push_back(new InfClass(400, date->name.c_str(), 1));
				}
			}
			return;
		}
	}

	switch (date->id)
	{
	case Equipment::Tag_Equipment1:
		date->price = 1000;
		date->name = "ただの装備";
		break;
	case Equipment::Tag_Equipment2:
		date->price = 200;
		date->name = "普通の装備";
		break;
	case Equipment::Tag_Equipment3:
		date->price = 3200;
		date->name = "強い装備";
		break;
	case Equipment::Tag_Equipment4:
		date->price = 4600;
		date->name = "つおーい装備";
		break;
	case Equipment::Tag_Equipment5:
		date->price = 5000;
		date->name = "最強の装備";
		break;
	default:
		break;
	}

	if (date->is_log_)
	{
		Master::inf_class_manager_->LogList.push_back(new InfClass(400, date->name.c_str(), 1));
	}
	date_list_.push_back(date);
}

/// @brief 指定した装備をプレイヤーに装着（装備状態を有効化）する
/// @param date 装着する装備データのポインタ
void EquipmentManager::WearEquipment(Equipment::EquipmentDate* date)
{
	for (auto itr = date_list_.begin(); itr != date_list_.end(); ++itr)
	{
		(*itr)->get_ = ((*itr)->id == date->id);
	}

	GetDamage();
}

/// @brief 現在装着（有効化）されている装備の攻撃補正値（ダメージ）を取得する
/// @return float 装着中装備のダメージ補正値（未装着時は0.0f）
float EquipmentManager::GetDamage()
{
	for (auto itr = date_list_.begin(); itr != date_list_.end(); ++itr)
	{
		if ((*itr)->get_)
		{
			return static_cast<float>((*itr)->damage);
		}
	}
	return 0.0f;
}