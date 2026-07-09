#include"Combo.h"

Combo::Combo()
	:Object2D(VGet(0,0,0))
	,mnCombo(0)
{
	 handle = LoadGraph("%d", true);

}

Combo::~Combo()
{

}

void Combo::Draw()
{
	/*for (int i = 0; i < 10; i++)
	{
		DrawGraph(100,100,handle,)
		
	}*/

}


void Combo::Update()
{
	
}

void Combo::HitAddCombo()
{
	mnCombo++;
}
