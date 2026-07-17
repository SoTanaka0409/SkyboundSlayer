#pragma once
#include"DxLib.h"
#include<string>
#include"Object3D.h"
#include"Model.h"

#include"camera.h"
#include"Debug.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ItemManager.h"
#include"ShortInventory.h"
#include"InfClassManager.h"
#include"BuffManager.h"
#include"HaveMoneyClass.h"
#include"EquipmentManager.h"

class SphereCollider;
class CapsuleCollider;

class Player3D :public Object3D
{
public:
	enum AttackStateInfo {
		kAttackNormal,
		kAttackJump,
		kAttackSlide,
	} attack_state_;

public:
	// コンストラクタ
		// [出力] なし [副作用] メンバ初期化、コライダー生成
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Player3D(std::string filename, VECTOR initPos, float junppower, float speed, float hp, bool is_separate_anim_ = false);

	// デストラクタ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		~Player3D();

	// 描画処理
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void Draw()override;

	// 更新処理
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void Update()override;

	// 管理クラス用更新
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void ManagerUpdate();

	// ステージ外判定
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void CheckStageOut();

	// 移動処理
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void MoveEx();

	// 攻撃選択
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void SelectAttack();

	// 通常攻撃
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void Attack();

	// スライド攻撃
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void AttackSlide();

	// ジャンプ攻撃
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void AttackJump();

	// ダメージ処理
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void Damage(float damage);

	// 状態リセット
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void ResetNUETRAL();

	// 無敵フラグ設定
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void SetInvisible(bool flag) { is_invisible_ = flag; }

	// 無敵フラグ取得
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		bool GetInvisible() { return is_invisible_; }

	// ジャンプ処理
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void Jump();

	// ビューモード更新
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void UpdateViewMode();

	// 敵探索
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void SearchEnemy();

	// コライダー位置更新
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void UpdateColliderPosition();

	// 攻撃クールダウン更新
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void UpdateAttackCooldowns();

	// 回避行動
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void Evasion();

	// コライダー削除
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void CollDelete();

	// ロックオン対象更新
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void UpdateTargetLock();

	// 角度取得
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		float GetAngle() { return angle_; }

	// 移動による回転処理
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void RotationByMove();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetHp() { return hp_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetHp(float hp) { hp_ = hp; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetAttack() { return attack_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetAttack(float attack) { attack_ = attack; }

	// 全ステータス状態取得
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		float GetAllStatusState(Object3D::StatusState state);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetSpeed() { return speed_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetSpeed(float speed) { speed_ = speed_ + speed; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetSize() { return size_; }

	// --- Upgrade Setters ---
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddUpgradeMaxHp(float add) { upgrade_max_hp_ += add; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddUpgradeAttack(float add) { upgrade_attack_ += add; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddUpgradeSpeed(float add) { upgrade_speed_ += add; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddUpgradeEvasionSpeed(float add) { upgrade_evasion_speed_ += add; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddUpgradeEvasionInvincibility(int add) { upgrade_evasion_invincibility_ += add; }
	// -----------------------

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VECTOR GetFirstPos() { return first_position_; };
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool GetStageOutFlag() { return is_stage_out_; }

	// ステータスバー描画
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void DrawStatusBars();

	// アニメーション追加
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		void AddAnimation(AnimationState state, std::string filename);

	// 当たり判定開始
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		virtual void OnEnter(Collider* collider, Collider* check) override;
	// 当たり判定継続
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		virtual void OnTrigger(Collider* collider, Collider* check) override;
	// 当たり判定終了
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
		virtual void OnExit(Collider* collider, Collider* check) override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	CapsuleCollider* GetCollisionCollider() { return capsule_collider_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	SphereCollider* Get500Collider() { return search_enemy_collider_; }

	Model* model_;
	ShortInventory* short_inventory_;
	BuffManager* buff_manager_;
	ItemManager* item_manager_;
	HaveMoneyClass* have_money_;
	EquipmentManager* equipment_manager_;

private:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void UpdateInvincibilityTimer();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void UpdatePlayerSystems();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool ShouldSkipGameplayUpdate() const;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsBossFadeActive() const;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void ValidateTarget();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void UpdateGameplayActions();
	bool CanDrawPlayer() const;
	void DrawPlayerModel();
	void DrawDebugInfo();
	void DrawAttachmentDebug();

	Object3D* target_;
	float target_angle_;
	float angle_;
	VECTOR first_position_;

	const float kRotateSpeed = 0.2f;
	const float kJumpPower = 30.0f;

	bool is_jumping_;
	bool is_jump_falling_;
	bool has_reached_jump_peak_;
	bool is_jump_collider_active_;
	bool is_attack_slide_target_found_;

	float jump_power_;
	float speed_;
	float evasion_speed_;
	float normal_speed_;
	float attack_slide_speed_;
	float hp_;
	float max_hp_;
	float ride_old_hp_;
	float size_;

	// --- Upgrade Stats from StatShop ---
	float upgrade_max_hp_ = 0.0f;
	float upgrade_attack_ = 0.0f;
	float upgrade_speed_ = 0.0f;
	float upgrade_evasion_speed_ = 0.0f;
	int upgrade_evasion_invincibility_ = 0;
	int invincible_timer_ = 0;
	// -----------------------------------

	int attack_slide_count_;
	int attack_jump_count_;
	int attack_count_;

	int attack_slide_cooldown_ = 200;
	int attack_cooldown_ = 30;
	int attack_jump_cooldown_ = 200;

	float attack_;
	float normal_attack_;
	float jump_attack_;
	float slide_attack_;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VECTOR move_vec_ = VGet(0.0f, 0.0f, 0.0f);
	VECTOR previous_move_vec_;
	VECTOR attack_slide_direction_;
	VECTOR attack_slide_step_;

	bool is_invisible_;

	int target_search_count_;
	float nearest_target_distance_;

	SphereCollider* attach_collider_;
	SphereCollider* search_enemy_collider_;
	SphereCollider* attack_slide_collider_;
	SphereCollider* attack_jump_collider_;
	CapsuleCollider* capsule_collider_;

	int attack_selection_index_;
	bool is_stage_out_;
};
