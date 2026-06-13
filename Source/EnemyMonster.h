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

	EnemyMonster(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, float xp, int money, bool isSeparateAnim);
	~EnemyMonster();

	void Update() override;
	void Draw() override;
	void Attack() override;
	void OnTrigger(Collider* collider, Collider* check) override;
	void DeathEnemy() override;

private:
	AttackState mAttackState;
	int mChargeTimer;
	int mJumpTimer;

	SphereCollider* mpLandingAttackCollider;
	bool mHasLandedHit; // To prevent multiple hits from the same landing

	// Jump parameters
	float mJumpVelocity;
	float mGravity;
	float mForwardSpeed;
	VECTOR mJumpTargetDir;
};
