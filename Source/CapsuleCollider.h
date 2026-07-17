#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual ~CapsuleCollider();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update(Collider* check) override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw() override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void OnEnter() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void OnTrigger() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void OnExit() override;

};