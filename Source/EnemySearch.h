#pragma once
#include"Dxlib.h"


class EnemySerch
{
public:

	void EneSerch(bool serch) { SerchOn = serch; }
	bool getSerch() { return SerchOn; }


private:
	bool SerchOn=false;//敵が近くにいるか

};