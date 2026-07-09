#pragma once
#include"Dxlib.h"
#include"Object3D.h"

#include"DrawHp.h"
#include"Debug.h"
#include"EnemyManager.h"
#include"Enemy.h"
#include"Collision.h"

class Model;

class Enemy3D :public Enemy
{
public:
	Enemy3D(std::string filename, VECTOR initPos, float hp,float speed,float HitSize,float Serch1,float Serch2,float Serch3,float xp,int money,bool isSeparateAnim);

	~Enemy3D();

	void Draw()override;

	void Update()override;

	void Attack()override;

	
	

private:
	bool mbMagic;



};
