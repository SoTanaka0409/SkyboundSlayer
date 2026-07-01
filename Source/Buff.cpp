#include"Buff.h"
#include"Master.h"
#include"ObjectManager.h"

Buff::Buff(int timer, float effect,Object3D::StatusState type)
	:Do_buff(true)
{
	auto mpPlayer = Master::mpPlayer;
	Player3D* player = Master::mpPlayer;
	efDate.Timer = timer;
	efDate.Effect = effect;
	efDate.Count = 0;
	efDate.type=type;

}
Buff::~Buff()
{

}

void Buff::Update()
{
	efDate.Count++;

	if(efDate.Count>= efDate.Timer)
	{
		Do_buff = false;
		efDate.mbUse = false;
		efDate.Effect = 0;
		
	}
	else
	{
		DrawFormatString(200, 800, GetColor(255, 0, 0), "buffŽg—p’†");
	}


}

