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

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	StatShop(std::string filename, VECTOR vec);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~StatShop();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update() override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void StartWalkingIn();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void StartWalkingOut();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void movePosition();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsArrived() const { return shop_state_ == ShopState::ARRIVED; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnEnter(Collider* collider, Collider* check) override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnTrigger(Collider* collider, Collider* check) override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool CanUpdateShop() const;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void CloseShopIfPhaseEnding();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void UpdateShopMenu();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void HandleShopCloseInput();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DrawShopMenu(Player3D* player);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DrawShopHeader(Player3D* player);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DrawShopOptions();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DrawShopFooter();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DrawShopNpc(Player3D* player);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SelectClass();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void BuyClass();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	int GetCost(int upgradeCount);
	void UpdateWalkIn();
	void UpdateWalkOut();
	void UpdateShopColliderVisibility();
	void SyncModelTransform();

	Model* model_;
	SphereCollider* shop_in_;
	SphereCollider* safe_zoon_;

	VECTOR target_position_;
	VECTOR mvStartPosition;
	ShopState shop_state_;

	int select_;
	int select_max_;
	int select_min_;
	bool old_mouse_down_;

	int upgrade_max_hp_count_;
	int upgrade_attack_count_;
	int upgrade_speed_count_;
	int upgrade_evasion_speed_count_;

	int icon_max_hp_handle_;
	int icon_attack_handle_;
	int icon_speed_handle_;
	int icon_evasion_dist_handle_;
	int icon_evasion_inv_handle_;
};

