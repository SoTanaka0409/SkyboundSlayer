#pragma once
#include"Dxlib.h"



class LevelUp
{
private:
	
	
	float level;
	float exp;
	float maxhp;
	float attack;
	float speed;

	float mnXp;
	
	bool mbLevelUp;
	


public:
	LevelUp();

	~LevelUp();

	void Update();

	void Draw();

	float GetNextXp() { return 100 + level * 100; }
	bool GetLevelUp() { return mbLevelUp; }
	
	void AddXp(int xp);

	float GetHp() { return maxhp; }
	float GetAttack() { return attack; }
	float GetSpeed() { return speed; }
	int GetXp() { return exp; }
	
	
	VECTOR pos = { 30,820 };
	VECTOR size = { 300,40 };

};