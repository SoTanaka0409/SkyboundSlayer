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

//繧ゅ＠縺吶〒縺ｫ縺昴・豁ｦ蝎ｨ繧呈戟縺｣縺ｦ縺・◆繧詠eturn縺吶ｋ,
void WeaponManager::AddWeapon(Weapon::WeaponDate* date)
{
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		if ((*itr)->id==date->id)
		{
			if ((*itr)->damage < date->damage)//繧ゅ＠莉顔樟蝨ｨ迯ｲ蠕励＠縺溘♀縺ｪ縺露D縺ｮ豁ｦ蝎ｨ縺ｮ謾ｻ謦・鴨縺悟渕縺ｮ繧医ｊ鬮倥°縺｣縺溘ｉ謾ｻ謦・鴨繧呈峩譁ｰ縺吶ｋ
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
		date->name = "縺溘□縺ｮ蜑｣";
		break;
	case Weapon::Tag_Weapon2:
		date->price = 800;
		date->name = "譎ｮ騾壹・蜑｣";
		break;
	case Weapon::Tag_Weapon3:
		date->price = 1200;
		date->name = "蠑ｷ縺・殴";
		break;
	case Weapon::Tag_Weapon4:
		date->price = 1600;
		date->name = "縺､縺翫・縺・殴";
		break;
	case Weapon::Tag_Weapon5:
		date->price = 2000;
		date->name = "譛蠑ｷ縺ｮ蜑｣";
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
			filename = (*itr)->filename;//繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺斐→縺ｫ謖√▲縺ｦ縺・ｋ豁ｦ蝎ｨ縺ｮ繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繧呈爾縺・
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
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	if (player == nullptr) return;
	
	//GetDamage(date);
	
	player->mpModel->AddAttachment(date->filename, "mixamorig:RightHandIndex1");
	
	player->GetAllStatusState(Object3D::Status_Attack);
	for (auto itr = mDateList.begin(); itr != mDateList.end(); itr++)
	{
		(*itr)->mbGet = false;//荳蝗槭☆縺ｹ縺ｦ縺ｮ豁ｦ蝎ｨ繧貞､悶☆
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






