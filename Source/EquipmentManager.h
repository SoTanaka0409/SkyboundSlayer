#pragma once
#include"Equipment.h"
#include"list"

class EquipmentManager
{
public:
	EquipmentManager();

	~EquipmentManager();

	void Draw();

	void Update();

	void AddEquipment(Equipment::EquipmentDate* date);

	void WearEquipment(Equipment::EquipmentDate* date);
	float GetDamage();
	std::list<Equipment::EquipmentDate*>mDateList;
private:
	
};