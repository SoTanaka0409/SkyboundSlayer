#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"

class SphereCollider;

class StatShop : public Object3D
{
public:
	enum class ShopState { WAIT_PHASE, WALKING_IN, ARRIVED, WALKING_OUT };
	StatShop(std::string filename, VECTOR vec);
	~StatShop();

	void Draw() override;
	void Update() override;

	void StartWalkingIn();
	void StartWalkingOut();
	void movePosition();

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
	
	VECTOR mTargetPosition;
	VECTOR mvStartPosition;
	ShopState mShopState;

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

	// Icons
	int mnIconMaxHpHandle;
	int mnIconAttackHandle;
	int mnIconSpeedHandle;
	int mnIconEvasionDistHandle;
	int mnIconEvasionInvHandle;
};
