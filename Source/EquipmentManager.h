#pragma once
#include"Equipment.h"
#include"list"

class EquipmentManager
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	EquipmentManager();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~EquipmentManager();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void AddEquipment(Equipment::EquipmentDate* date);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void WearEquipment(Equipment::EquipmentDate* date);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetDamage();
	std::list<Equipment::EquipmentDate*>date_list_;
private:
	
};