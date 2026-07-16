#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"

class Player3D;
class SphereCollider;

class StatShop : public Object3D
{
public:
	enum class ShopState
	{
		WAIT_PHASE,
		WALKING_IN,
		ARRIVED,
		WALKING_OUT
	};

	StatShop(std::string filename, VECTOR vec);
	~StatShop();

	void Draw() override;
	void Update() override;

	void StartWalkingIn();
	void StartWalkingOut();
	void movePosition();
	bool IsArrived() const { return mShopState == ShopState::ARRIVED; }

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
	bool CanUpdateShop() const;
	void CloseShopIfPhaseEnding();
	void UpdateShopMenu();
	void HandleShopCloseInput();
	void DrawShopMenu(Player3D* player);
	void DrawShopHeader(Player3D* player);
	void DrawShopOptions();
	void DrawShopFooter();
	void DrawShopNpc(Player3D* player);
	void SelectClass();
	void BuyClass();
	int GetCost(int upgradeCount);

	Model* model_;
	SphereCollider* mpShopIn;
	SphereCollider* mpSafeZoon;

	VECTOR mTargetPosition;
	VECTOR mvStartPosition;
	ShopState mShopState;

	int mnSelect;
	int mnSelectMax;
	int mnSelectMin;
	bool mbOldMouseDown;

	int mnUpgradeMaxHpCount;
	int mnUpgradeAttackCount;
	int mnUpgradeSpeedCount;
	int mnUpgradeEvasionSpeedCount;

	int mnIconMaxHpHandle;
	int mnIconAttackHandle;
	int mnIconSpeedHandle;
	int mnIconEvasionDistHandle;
	int mnIconEvasionInvHandle;
};

