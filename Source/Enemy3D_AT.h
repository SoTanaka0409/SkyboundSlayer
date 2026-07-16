#pragma once
#include"Dxlib.h"
#include"Object3D.h"

#include"DrawHp.h"
#include"Debug.h"
#include"EnemyManager.h"
#include"Enemy.h"
#include"Collision.h"

class Model;

class Enemy3D_AT :public Enemy
{
public:
	Enemy3D_AT(std::string filename, VECTOR initPos, float hp, float speed,float HitSize, float Serch1, float Serch2,float Serch3,float Attack,int money,bool isSeparateAnim);

	~Enemy3D_AT();

	void Draw()override;

	void Update()override;

	


	




	
private:
	Debug* mpDebug;

	//SphereCollider* mpAttachCollider;//攻撃の当たり判定の円
	//CapsuleCollider* mpCapsuleCollider;//自身の当たり判定の円
	//SphereCollider* mpSerchCollider;//敵を見つけるための円
	//SphereCollider* mpAttackCollider;//攻撃し始める円
	//SphereCollider* mpStopCollider;//ある程度の距離で立ち止まる円
	

	
	

	/// <summary>
	/// ///////////チュートリアル
	/// </summary>




};
