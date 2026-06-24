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

class Player3D;
class Shop:public Object3D
{

public:
	enum class ShopState { WAIT_PHASE, WALKING_IN, ARRIVED, WALKING_OUT };

	Shop(std::string filename,VECTOR vec);

	~Shop();

	void Draw();

	void Update();
	void movePosition();

	void StartWalkingIn();
	void StartWalkingOut();

	void Selectclass();
	void Firstclass();

	void Buyclass();

	void Sellclass();
	bool TryPurchase(Player3D* player, int price);

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
	VECTOR mTargetPosition;
	ShopState mShopState;
	bool InShop=false;
	float r;
	int mnShopListCount;
	int Select;
	int SelectMax;
	int SelectMin;

};
