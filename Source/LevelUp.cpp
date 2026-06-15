#include"LevelUp.h"



LevelUp::LevelUp()
	:mnXp(0)
	,maxhp(0)
	,attack(0)
	,exp(1)
	,level(1)
	,speed(0)
	,mbLevelUp(false)
{
	
}

LevelUp::~LevelUp()
{

}

void LevelUp::Draw()
{
	
	float XpRatio = exp/GetNextXp();
	exp = exp < 0 ? 0 : exp; //HPが0未満になったら0を入れる
	exp = exp > GetNextXp() ? GetNextXp() : exp; //HPが最大値を超えていたら、最大値を入れる

	
	unsigned int Color;
	unsigned int Color2;
	unsigned int Color3;
	Color = GetColor(255, 255, 255);
	Color2 = GetColor(0, 0, 0);
	Color3 = GetColor(0, 0, 255);
	DrawBox(pos.x, pos.y, pos.x + size.x, pos.y + size.y, Color2, true);
	DrawBox(pos.x, pos.y, pos.x + size.x, pos.y + size.y, Color, true);
	DrawBox(pos.x + 1, pos.y + 1,
		pos.x + size.x * XpRatio - 1,
		pos.y + size.y - 1,
		Color3, true);

/////////////////////////////////////////////////////
	
//////////////////////////////////////////////////////
	DrawFormatString(30, 800, Color3, "レベル:%f", level);

}

void LevelUp::Update()
{
	if (mbLevelUp)mbLevelUp = false;//レベルアップの恩恵
	
	//レベルUPフラグ
	
	if (exp > GetNextXp())
	{
		level += 1;
		exp = 1;
		attack += 1;
		maxhp += 20;
		speed += 5;
		mbLevelUp = true;
	}

}

void LevelUp::AddXp(int xp)
{
	exp += xp;

	
}