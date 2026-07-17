#pragma once
#include"vector"
#include"Buff.h"
#include"map"
class BuffManager
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	BuffManager();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~BuffManager();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddBuff(Buff* date);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetBuff(Object3D::StatusState state);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DeleteList();

	std::vector<Buff*>buffList;
private:
	
};