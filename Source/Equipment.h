#pragma once
#include"vector"
#include"dxlib.h"
#include<string>

class Equipment
{
public:
	
	enum Tag_Equipment
	{
		Tag_Equipment1,
		Tag_Equipment2,
		Tag_Equipment3,
		Tag_Equipment4,
		Tag_Equipment5,
	};
	struct EquipmentDate
	{
		Tag_Equipment id;
		int damage;
		std::string name;
		bool get_ = false;//着けている状態true
		std::string filename;
		int price;
		bool is_log_ = true;
	}Inf;
	
public:

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Equipment();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Equipment();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

	

private:
	

};