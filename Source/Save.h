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
		std::list<Item::ItemInformation*>I_date;
		std::list<Equipment::EquipmentDate*>E_date;
		

	};


public:

	//static Save* GetInstance()

	//static void Finalize()
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Save();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Save();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void GameSave(SaveDate* date);
	
   
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void GameLoad(SaveDate* date);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SelectClass();
private:
	Texture* texture_;
	std::list<SaveDate*>AllSaveDate;
	
	
	int Select;
	int SelectMax;
	int SelectMin;
};
