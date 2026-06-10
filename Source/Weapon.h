#pragma once
#include"DxLib.h"
#include"String"
#include"Model.h"


class Weapon
{
public:
	enum Tag_Weapon
	{
		Tag_Weapon1,
		Tag_Weapon2,
		Tag_Weapon3,
		Tag_Weapon4,
		Tag_Weapon5,
	};
	struct WeaponDate
	{
		Tag_Weapon id;
		int damage;
		std::string name;
		bool mbGet=false;
		std::string filename;
		int price;
		bool isLog = true;
	}inf;

	

	
	
public:
	Weapon();

	~Weapon();

	virtual void Update();

	virtual void Draw();

	void SetTag_W(Tag_Weapon tag) { mnTag_W = tag; }
	Tag_Weapon GetTag_W() { return mnTag_W; }

	std::string GetName() { return inf.name; }
	

	int WeaponNumber;
	

private:

	Tag_Weapon  mnTag_W;
	Model* mpModel;
	
	int mnHandle;//ƒnƒ“ƒhƒ‹
	
	
};