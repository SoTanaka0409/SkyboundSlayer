#pragma once
#include"dxlib.h"
#include"Object2D.h"
#include"Object3D.h"

class HaveMoneyClass
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	HaveMoneyClass(int money);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~HaveMoneyClass();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddMoney(int money);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void PullMoney(int money);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	int HaveMoney() { return money_; }

private:
	int money_;
	Object3D* parent_;

};