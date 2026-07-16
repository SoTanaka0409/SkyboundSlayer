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
	const int panelX = 1564;
	const int panelY = 122;
	const int panelW = 328;
	const int panelH = 58;
	const int panel = GetColor(18, 17, 20);
	const int gold = GetColor(198, 154, 64);
	const int goldDark = GetColor(98, 73, 32);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(panelX - 6, panelY - 4, panelX + panelW + 6, panelY + panelH + 6, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, panel, true);
	DrawLine(panelX, panelY, panelX + panelW, panelY, gold, 1);
	DrawLine(panelX, panelY + panelH, panelX + panelW, panelY + panelH, goldDark, 1);
	DrawLine(panelX, panelY, panelX, panelY + panelH, goldDark, 1);
	DrawLine(panelX + panelW, panelY, panelX + panelW, panelY + panelH, gold, 1);
	DrawFormatString(panelX + 18, panelY + 17, GetColor(245, 226, 174), "MONEY");
	DrawFormatString(panelX + 186, panelY + 17, GetColor(238, 238, 238), "%d", mnMoney);
}
void HaveMoneyClass::PullMoney(int money)
{
	auto mpPlayer = Master::mpPlayer;
	auto player = Master::mpPlayer;
	player->have_money_->AddMoney(-money);
	

}

