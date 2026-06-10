#include"InfClass.h"
#include"ItemManager.h"
#include"WeaponManager.h"
#include"Master.h"

InfClass::InfClass(int Log_timer,std::string Inf,int num)
	:Object2D(VGet(0,0,0))
	,LogNumber(num)
	
{
	LogCount = 0;
	mbElaseFlag = false;
	LogTime = Log_timer;
	LogDate = Inf;
}

void InfClass::Draw()
{
	if (LogNumber == 1)
	{
		DrawFormatString(mvPosition.x, mvPosition.y, GetColor(255, 255, 255), "%sを獲得した", LogDate.c_str());
	}
	else if (LogNumber == 2)
	{
		DrawFormatString(mvPosition.x, mvPosition.y, GetColor(255, 255, 255), "%sを使用した", LogDate.c_str());
	}
	else if (LogNumber == 3)
	{
		DrawFormatString(mvPosition.x, mvPosition.y, GetColor(255, 255, 255), "%sが現れた", LogDate.c_str());
	}
	else if (LogNumber == 4)
	{
		DrawFormatString(mvPosition.x, mvPosition.y, GetColor(255, 255, 255), "%sは使用できません", LogDate.c_str());
	}
	else if(LogNumber==5)
	{
		DrawFormatString(mvPosition.x, mvPosition.y, GetColor(255, 255, 255), "%s", LogDate.c_str());
	}
	else
	{
		DrawFormatString(mvPosition.x, mvPosition.y, GetColor(255, 255, 255), "未確認なログです");
	}

	

/*	if (IsDrawFlag == false)
	{
		DrawFormatString(mvPosition.x, mvPosition.y, GetColor(255, 255, 255), "%sを獲得した", LogDate.c_str());
	}
	*/
}

void InfClass::Update()
{
	LogCount++;
	if (LogTime <LogCount)
	{
		mbElaseFlag = true;
		SetDeleteFlag(true);
		SetDrawFlag(false);
	}
	
}