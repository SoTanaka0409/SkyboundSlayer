#pragma once
#include"dxlib.h"
#include"ItemManager.h"

#include"EquipmentManager.h"
#include"Texture.h"
#include"list"

class Save
{public:
	enum SaveNumber
	{
		Save_1,
		Save_2,
		Save_3,
		Save_Max,

	};
	struct SaveDate
	{
		SaveNumber num;
		float hp=0;
		float MaxHp=0;
		float attack=0;
		float speed=0;
		int Level=0;
		int xp=0;
		std::list<Item::ItemInformation*>I_date;
		std::list<Equipment::EquipmentDate*>E_date;
		

	};


public:

	//static Save* GetInstance()

	//static void Finalize()
	Save();

	~Save();

	void Draw();
	void Update();

	void GameSave(SaveDate* date);
	
   
	void GameLoad(SaveDate* date);
	void SelectClass();
private:
	Texture* mpTexture;
	std::list<SaveDate*>AllSaveDate;
	
	
	int Select;
	int SelectMax;
	int SelectMin;
};
