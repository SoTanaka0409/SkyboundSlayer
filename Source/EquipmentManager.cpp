#include "EquipmentManager.h"
#include "Master.h"
#include "InfClass.h"


/// @brief EquipmentManagerの初期化（コンストラクタ）
EquipmentManager::EquipmentManager()
{
}

EquipmentManager::~EquipmentManager()
{
}


/// @brief EquipmentManagerの描画処理
void EquipmentManager::Draw()
{
}


/// @brief EquipmentManagerの状態更新処理
void EquipmentManager::Update()
{
}


/// @brief EquipmentManagerのAddEquipment処理
void EquipmentManager::AddEquipment(Equipment::EquipmentDate* date)
{
	for (auto itr = date_list_.begin(); itr != date_list_.end(); itr++)
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


/// @brief EquipmentManagerのWearEquipment処理
void EquipmentManager::WearEquipment(Equipment::EquipmentDate* date)
{
	for (auto itr = date_list_.begin(); itr != date_list_.end(); itr++)
	{
		(*itr)->get_ = false;
	}
	for (auto itr = date_list_.begin(); itr != date_list_.end(); itr++)
	{
		if ((*itr)->id == date->id)
		{
			(*itr)->get_ = true;
		}
	}
	GetDamage();
}


/// @brief EquipmentManagerのGetDamage処理
float EquipmentManager::GetDamage()
{
	for (auto itr = date_list_.begin(); itr != date_list_.end(); itr++)
	{
		if ((*itr)->get_)
		{
			return static_cast<float>((*itr)->damage);
		}
	}
	return 0.0f;
}
