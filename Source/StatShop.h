#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"

class SphereCollider;

class StatShop : public Object3D
{
public:
	StatShop(std::string filename, VECTOR vec);
	~StatShop();

	void Draw() override;
	void Update() override;

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
	void SelectClass();
	void BuyClass();
	int GetCost(int level);

	Model* mpModel;
	SphereCollider* mpShopIn;
	SphereCollider* mpSafeZoon;
	
	int mnSelect;
	int mnSelectMax;
	int mnSelectMin;

	int mnBgImageHandle;
	bool mbOldMouseDown;

	// Upgrade Levels
	int mnLevelMaxHp;
	int mnLevelAttack;
	int mnLevelSpeed;
	int mnLevelEvasionSpeed;
	int mnLevelEvasionInvincibility;
};
