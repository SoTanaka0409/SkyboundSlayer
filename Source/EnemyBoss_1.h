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
	EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, float xp,int money, bool isSeparateAnim);

	~EnemyBoss_1();

	void Draw()override;

	void Update()override;

	

	void Attack()override;

	void OnTrigger(Collider* collider, Collider* check)override;
	
	void DeathEnemy()override;
	void Delete()override;

private:
	bool mbMagic;
	SphereCollider* mpJumpAttackCoiider;

	int mAttackType;
	int mAttack1ComboCount;
	
	float mfjumpPower;
	bool HighPositionFlag;
	bool mbjumpDown;
	bool mbjump;
	bool OnJumpCollider;


};
