#include"WeaponManager.h"
//#include"Dino.h"
#include"Master.h"
#include"InfClass.h"
#include"ObjectManager.h"



WeaponManager::WeaponManager()
	
	
{

}
WeaponManager::~WeaponManager()
{

}

//もしすでにそE武器を持ってぁEらreturnする,
void WeaponManager::AddWeapon(Weapon::WeaponDate* date)
{
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		if ((*itr)->id==date->id)
		{
			if ((*itr)->damage < date->damage)//もし今現在獲得したおなじIDの武器の攻Eが基のより高かったら攻Eを更新する
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
		date->price = 500;
		date->name = "ただの剣";
		break;
	case Weapon::Tag_Weapon2:
		date->price = 800;
		date->name = "普通E剣";
		break;
	case Weapon::Tag_Weapon3:
		date->price = 1200;
		date->name = "強ぁE";
		break;
	case Weapon::Tag_Weapon4:
		date->price = 1600;
		date->name = "つおEぁE";
		break;
	case Weapon::Tag_Weapon5:
		date->price = 2000;
		date->name = "最強の剣";
		break;
	default:
		break;
	}
	if (date->isLog)Master::mpInfClassManager->LogList.push_back(new InfClass(400, date->name.c_str(), 1));
	mDateList.push_back(date);
}

void WeaponManager::Update()
{
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		if ((*itr)->mbGet == true)
		{
			filename = (*itr)->filename;//キャラクターごとに持ってぁE武器のモチEのファイルを探ぁE
		}
	

	}



}

void WeaponManager::Draw()
{
	/*for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		DrawFormatString(300 + 50 * count, 200 + 50 + count, GetColor(255, 255, 255), "%s", (*itr)->name);
	}*/
	
}

void WeaponManager::ChangeWeapon(Weapon::WeaponDate* date)
{
	auto mpPlayer = Master::mpPlayer;
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	if (player == nullptr) return;
	
	
	player->mpModel->AddAttachment(date->filename, "mixamorig:RightHand");
	
	player->GetAllStatusState(Object3D::Status_Attack);
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		(*itr)->mbGet = false;//一回すべての武器を外す
	}
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		if ((*itr)->id == date->id)
		{
			(*itr)->mbGet = true;

		}
	}
	
		
	

	
	

}

float WeaponManager::GetDamage()
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






