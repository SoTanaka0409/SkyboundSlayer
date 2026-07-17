#pragma once
#include "DxLib.h"
#include"Player3D.h"
#include"EnemyManager.h"
#include"Object3D.h"
#include"vector"
#include"list"

class Collision
{
public:
	enum Tag_Coll
	{
		/*Coll_Player3D,
		Coll_Enemy3D,
		Coll_Stage3D,
		Coll_Wall3D,
		Coll_Tree3D,
		COll__Max,*/
		Coll_Charactor=1000,
	};
	//キャラクターの当たり判定を書いていく
public:

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Collision();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Collision();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	/*void SphireColl();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void CapsuleColl();*/

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Tag_Coll GetTag() { return CollTag; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetTag(Tag_Coll tag) { CollTag = tag; }
	
	std::list<float>size_list_;
private:
	Tag_Coll CollTag;
  /*  Player3D* Coll_P;
	Enemy* Coll_E;*/
	

	
};

class SphererCollider
{
public:

private:
	VECTOR mvPOsition;
	float radius_;
	Object3D* object_;
};