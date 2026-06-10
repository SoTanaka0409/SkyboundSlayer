#pragma once
#include"vector"
#include"List"
#include"Weapon.h"

class WeaponManager
{
public:
	WeaponManager();

	~WeaponManager();

	void Update();

	void AddWeapon(Weapon::WeaponDate* date);
	
	void Draw();

	void ChangeWeapon(Weapon::WeaponDate* date);

	std::string GetFileName() { return filename; }

	float GetDamage();

	
	/*void SetSp(bool sp) { Sp = sp; }
	bool GetSp() { return Sp; }*/

	std::vector<Weapon::Tag_Weapon>mTagList;
	std::list<Weapon::WeaponDate*>mDateList;
	//std::vector<Weapon*>mBulletList;
	
private:
	std::string filename;
	int count;
};