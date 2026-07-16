#include "EquipmentManager.h"
#include "Master.h"
#include "InfClass.h"

EquipmentManager::EquipmentManager()
{
}

EquipmentManager::~EquipmentManager()
{
}

void EquipmentManager::Draw()
{
}

void EquipmentManager::Update()
{
}

void EquipmentManager::AddEquipment(Equipment::EquipmentDate* date)
{
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		if ((*itr)->id == date->id)
		{
			if ((*itr)->damage < date->damage)
			{
				(*itr)->damage = date->damage;
				if (date->isLog)
				{
					Master::mpInfClassManager->LogList.push_back(new InfClass(400, date->name.c_str(), 1));
				}
			}
			return;
		}
	}

	switch (date->id)
	{
	case Equipment::Tag_Equipment1:
		date->price = 1000;
		date->name = "‚½‚¾‚Ì‘•”õ";
		break;
	case Equipment::Tag_Equipment2:
		date->price = 200;
		date->name = "•’Ê‚Ì‘•”õ";
		break;
	case Equipment::Tag_Equipment3:
		date->price = 3200;
		date->name = "‹­‚¢‘•”õ";
		break;
	case Equipment::Tag_Equipment4:
		date->price = 4600;
		date->name = "‚Â‚¨[‚¢‘•”õ";
		break;
	case Equipment::Tag_Equipment5:
		date->price = 5000;
		date->name = "Å‹­‚Ì‘•”õ";
		break;
	default:
		break;
	}

	if (date->isLog)
	{
		Master::mpInfClassManager->LogList.push_back(new InfClass(400, date->name.c_str(), 1));
	}
	mDateList.push_back(date);
}

void EquipmentManager::WearEquipment(Equipment::EquipmentDate* date)
{
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		(*itr)->mbGet = false;
	}
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		if ((*itr)->id == date->id)
		{
			(*itr)->mbGet = true;
		}
	}
	GetDamage();
}

float EquipmentManager::GetDamage()
{
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		if ((*itr)->mbGet)
		{
			return static_cast<float>((*itr)->damage);
		}
	}
	return 0.0f;
}
