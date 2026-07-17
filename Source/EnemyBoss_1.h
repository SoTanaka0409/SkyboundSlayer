#pragma once

#include"Dxlib.h"
#include"Object3D.h"

#include"DrawHp.h"
#include"Debug.h"
#include"EnemyManager.h"
#include"Enemy.h"
#include"Collision.h"

class Model;

class EnemyBoss_1 :public Enemy
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3,int money, bool is_separate_anim_);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~EnemyBoss_1();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw()override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update()override;

	

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Attack()override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void OnTrigger(Collider* collider, Collider* check)override;
	
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DeathEnemy()override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Delete()override;

private:
	// ジャンプ中の物理計算処理
	void UpdateJumpPhysics();

	// === 定数 ===
	static constexpr float kJumpAscendSpeed = 15.0f;
	static constexpr float kJumpDescendSpeed = -25.0f;
	SphereCollider* jump_attack_coiider_;

	int attack_type_;
	int attack1_combo_count_;
	
	float mfjumpPower;
	bool HighPositionFlag;


};
