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
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Enemy3D(std::string filename, VECTOR initPos, float hp,float speed,float HitSize,float Serch1,float Serch2,float Serch3,int money,bool is_separate_anim_);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Enemy3D();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw()override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update()override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Attack()override;

	
	

};
