#pragma once
#include"Dxlib.h"
#include"vector"
#include"WeaponManager.h"
#include"ItemManager.h"
#include"Texture.h"
#include"Object3D.h"
#include"EquipmentManager.h"

class Inventory
{
public:

	enum InventoryName
	{
	
		In_Item,
		In_Weapon,
		In_Equipment,
		In_back,
		In_Max,

	};
	
public:
	Inventory(Object3D::Tag3D tag);

	~Inventory();

	void Draw();

	void Update();
	void SelectClass();
	void Select1();
	void SelectItem();
	void SelectWeapon();
	void SelectEquipment();

	

	void DropInventory(int chance,bool onWeapon,bool onItem);//itemをdropする確率  falseだったら確定でdropしない
	
	

	std::vector<InventoryName>NameList;
	ItemManager* mpItemManager;
	WeaponManager* mpWeaponManager;
	EquipmentManager* mpEquipmentManager;
private:
	Texture* mpTexture;
	
	static const int WIDTH = 9;
	static const int HEIGHT = 4;
	InventoryName list;
	Object3D* Inv_Obj;

	int FontColor = GetColor(128, 128, 128);
	
	bool mbPlayer;
	int Select;
	int SelectMax;
	int SelectMin;

	int count;

	bool InventoryOn;//どのインベントリを探っxっているか

	int mnFirst_Y= 400;
	int mnFirst_X=300;

	
};

