#pragma once
#include"dxlib.h"
#include"ItemManager.h"
#include"WeaponManager.h"
#include"EquipmentManager.h"
#include"Item.h"
#include"Weapon.h"
#include"Equipment.h"
#include"vector"
#include"Object3D.h"
#include"Model.h";
#include"Texture.h"

class SphereCollider;
class CapsuleCollider;

class Shop:public Object3D
{

public:
	Shop(std::string filename,VECTOR vec);

	~Shop();

	void Draw();

	void Update();
	void movePosition();

	void Selectclass();
	void Firstclass();

	void Buyclass();

	void Sellclass();

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	

private:
	Model* mpModel;
	ItemManager* mpItemManager;
	WeaponManager* mpWeaponManager;
	EquipmentManager* mpEquipmentManager;
	SphereCollider* mpShopIn;
	SphereCollider* mpSafeZoon;
	Texture* mpTexture;
	Texture* mpTexture2;
	
	
	int Y;
	int X;
	VECTOR oldPosition;
	bool InShop=false;
	float r;
	int mnShopListCount;
	int Select;
	int SelectMax;
	int SelectMin;

};