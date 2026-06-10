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
	DrawFormatString(x, y, GetColor(255, 255, 255), "ŠŽƒ}ƒl[:%d",mnMoney);
	
}
void HaveMoneyClass::PullMoney(int money)
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	auto player = dynamic_cast<Player3D*>(mpPlayer);
	player->mpHaveMoney->AddMoney(-money);
	

}

