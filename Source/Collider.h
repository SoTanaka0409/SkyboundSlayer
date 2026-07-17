#pragma once
#pragma once
#include "DxLib.h"
#include <vector>

class Object3D;

class Collider
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Collider(Object3D* parent);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual ~Collider();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Update(Collider* check);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnEnter();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnTrigger();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnExit();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void HitCheck(Collider* check, bool isHit);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsDeleteFlag() { return delete_flag_; }

public:
	Object3D* parent_object_;

	VECTOR position_;
	VECTOR position2_;
	float radius_;

	bool delete_flag_;

protected:
	std::vector<Collider*> collision_list_;	// 衝突しているColliderのリスト
	
};