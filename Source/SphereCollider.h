#pragma once
#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	SphereCollider(Object3D* parent, VECTOR center, float radius);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual ~SphereCollider();

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