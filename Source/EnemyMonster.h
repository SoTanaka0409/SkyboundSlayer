#pragma once

#include "Dxlib.h"
#include "Enemy.h"
#include "SphereCollider.h"

class EnemyMonster : public Enemy
{
public:
	enum class AttackState {
		None,
		Charging,
		Jumping,
		Landing
	};

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	EnemyMonster(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~EnemyMonster();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Attack() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void OnTrigger(Collider* collider, Collider* check) override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DeathEnemy() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Delete() override;

private:
	void UpdateAttackIdle();
	void UpdateAttackCharging();
	void UpdateAttackJumping();
	void UpdateAttackLanding();
	bool IsPlayerInJumpRange() const;
	void SetJumpDirectionToPlayer();
	void StartJumpAttack();

	AttackState attack_state_;
	int charge_timer_;
	int jump_timer_;

	SphereCollider* landing_attack_collider_;
	bool has_landed_hit_; // To prevent multiple hits from the same landing

	// ジャンプ用パラメータ
	float jump_velocity_;
	float gravity_;
	float forward_speed_;
	VECTOR jump_target_dir_;
	float jump_start_y_; // Added to record the ground height when jumping
};
