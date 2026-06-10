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
	//int status = 0;
	//for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	//{
	//	
	//	if ((*itr)->mbGet)status = (*itr)->damage;//今現在装備しているデータの値を表示
	//	
	//}
	//if (Master::mpDebug)DrawFormatString(200, 200, GetColor(255, 255, 255), "%d", status);
	
	
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
			if ((*itr)->damage < date->damage)//もし今現在獲得したおなじIDの武器の攻撃力が基のより高かったら攻撃力を更新する
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
		date->name= "ただの装備";
		break;
	case Weapon::Tag_Weapon2:
		date->price = 200;
		date->name = "普通の装備";
		break;
	case Weapon::Tag_Weapon3:
		date->price = 3200;
		date->name = "強い装備";
		break;
	case Weapon::Tag_Weapon4:
		date->price = 4600;
		date->name = "つおーい装備";
		break;
	case Weapon::Tag_Weapon5:
		date->price = 5000;
		date->name = "最強の装備";
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
	GetDamage();//装備したときに更新する
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