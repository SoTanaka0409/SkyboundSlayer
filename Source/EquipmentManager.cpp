#include"EquipmentManager.h"
#include"Master.h"
#include"InfClass.h"
#include"SceneManager.h"
#include"ObjectManager.h"

EquipmentManager::EquipmentManager()
{


}
EquipmentManager::~EquipmentManager()
{

}

void EquipmentManager::Draw()
{
	//	
	//	
	
	
}

void EquipmentManager::Update()
{

}

void EquipmentManager::AddEquipment(Equipment::EquipmentDate* date)
{
	for (auto itr=mDateList.begin();itr!=mDateList.end();itr++)
	{
		if((*itr)->id==date->id)
		{
			if ((*itr)->damage < date->damage)//‚à‚µ¡Œ»ÝŠl“¾‚µ‚½‚¨‚È‚¶ID‚Ì•Ší‚ÌUŒ‚—Í‚ªŠî‚Ì‚æ‚è‚‚©‚Á‚½‚çUŒ‚—Í‚ðXV‚·‚é
			{
				(*itr)->damage = date->damage;
				if(date->isLog)Master::mpInfClassManager->LogList.push_back(new InfClass(400, date->name.c_str(), 1));
			}
			return;
		}
		
	}
	switch (date->id)
	{
	case Weapon::Tag_Weapon1:
		date->price = 1000;
		date->name= "‚½‚¾‚Ì‘•”õ";
		break;
	case Weapon::Tag_Weapon2:
		date->price = 200;
		date->name = "•’Ê‚Ì‘•”õ";
		break;
	case Weapon::Tag_Weapon3:
		date->price = 3200;
		date->name = "‹­‚¢‘•”õ";
		break;
	case Weapon::Tag_Weapon4:
		date->price = 4600;
		date->name = "‚Â‚¨[‚¢‘•”õ";
		break;
	case Weapon::Tag_Weapon5:
		date->price = 5000;
		date->name = "Å‹­‚Ì‘•”õ";
		break;
	default:
		break;
	}
	if (date->isLog)Master::mpInfClassManager->LogList.push_back(new InfClass(400, date->name.c_str(), 1));
	mDateList.push_back(date);

}

void EquipmentManager::WearEquipment(Equipment::EquipmentDate* date)
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
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
	GetDamage();//‘•”õ‚µ‚½‚Æ‚«‚ÉXV‚·‚é
}

float EquipmentManager::GetDamage()
{
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		if ((*itr)->mbGet)
		{
			return (*itr)->damage;
		}
	}
	return 0.0f;
}