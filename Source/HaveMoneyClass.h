#pragma once
#include"dxlib.h"
#include"Object2D.h"
#include"Object3D.h"

class HaveMoneyClass
{
public:
	HaveMoneyClass(int money);

	~HaveMoneyClass();

	void Update();

	void Draw();

	void AddMoney(int money);

	void PullMoney(int money);

	int HaveMoney() { return mnMoney; }

private:
	int mnMoney;
	Object3D* mpParent;

};