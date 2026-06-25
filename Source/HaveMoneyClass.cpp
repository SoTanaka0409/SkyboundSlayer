#include"HaveMoneyClass.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"

HaveMoneyClass::HaveMoneyClass(int money)
	:mnMoney(money)
	
{
	
	
}

HaveMoneyClass::~HaveMoneyClass()
{

}

void HaveMoneyClass::AddMoney(int money)
{
	mnMoney += money;
}

void HaveMoneyClass::Update()
{

}

void HaveMoneyClass::Draw()
{
	int y=300;
	int x=600;
	DrawBox(x, y, x + 200, y + 50, GetColor(0, 0, 0), true);
	DrawFormatString(x, y, GetColor(255, 255, 255), "Šƒ}ƒl[:%d",mnMoney);
	
}
void HaveMoneyClass::PullMoney(int money)
{
	auto mpPlayer = Master::mpPlayer;
	auto player = dynamic_cast<Player3D*>(mpPlayer);
	player->mpHaveMoney->AddMoney(-money);
	

}

