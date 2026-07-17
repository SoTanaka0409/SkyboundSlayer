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
	Enemy3D_AT(std::string filename, VECTOR initPos, float hp, float speed,float HitSize, float Serch1, float Serch2,float Serch3,float Attack,int money,bool is_separate_anim_);

	~Enemy3D_AT();

// [入力]  [出力]  [副作用]
	void Draw()override;

// [入力]  [出力]  [副作用]
	void Update()override;









	//SphereCollider* attach_collider_;//攻撃の当たり判定の円
	//CapsuleCollider* capsule_collider_;//自身の当たり判定の円
	//SphereCollider* serch_collider_;//敵を見つけるための円
	//SphereCollider* attack_collider_;//攻撃し始める円
	//SphereCollider* stop_collider_;//ある程度の距離で立ち止まる円





	/// <summary>
	/// ///////////チュートリアル
	/// </summary>




};
