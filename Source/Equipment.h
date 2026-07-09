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
		bool mbGet = false;//着けている状態true
		std::string filename;
		int price;
		bool isLog = true;
	}Inf;
	
public:

	Equipment();

	~Equipment();

	void Draw();

	void Update();

	

private:
	

};