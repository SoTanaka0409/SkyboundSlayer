#include "EquipmentManager.h"
#include "Master.h"
#include "InfClass.h"


/*
 * 目的（EquipmentManagerのEquipmentManager処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
EquipmentManager::EquipmentManager()
{
}

EquipmentManager::~EquipmentManager()
{
}


/*
 * 目的（EquipmentManagerのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EquipmentManager::Draw()
{
}


/*
 * 目的（EquipmentManagerのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EquipmentManager::Update()
{
}


/*
 * 目的（EquipmentManagerのAddEquipment処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（EquipmentManagerのWearEquipment処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（EquipmentManagerのGetDamage処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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
