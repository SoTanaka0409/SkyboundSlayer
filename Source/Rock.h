#pragma once
#include"dxlib.h"
#include"Object3D.h"
#include"string"
#include"Model.h"

class SphereCollider;
class CapsuleCollider;


class Rock:public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Rock(std::string filename,VECTOR pos,float r,float High,float size);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Rock();

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
private:
	Model* model_;
	CapsuleCollider* capsule_collider_;
	float high_;
};