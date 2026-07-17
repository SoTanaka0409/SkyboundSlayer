#pragma once
#include"dxlib.h"
#include"Object3D.h"
#include"Texture.h"


class SphereCollider;
class CapsuleCollider;


class StageCollider :public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	StageCollider();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~StageCollider();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnEnter(Collider* collider, Collider* check);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnTrigger(Collider* collider, Collider* check);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnExit(Collider* collider, Collider* check);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void ColliderUpdate();
private:
	SphereCollider* go_boss_collider_;

	
	
	Texture* texture_;
	Texture* texture2_;
	int Count;
	int Time;
};