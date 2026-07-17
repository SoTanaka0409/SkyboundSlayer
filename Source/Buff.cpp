#include"Buff.h"
#include"Master.h"
#include"ObjectManager.h"


/*
 * 目的（BuffのBuff処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Buff::Buff(int timer, float effect,Object3D::StatusState type)
	:Do_buff(true)
{
	auto player_ = Master::player_;
	Player3D* player = Master::player_;
	efDate.Timer = timer;
	efDate.Effect = effect;
	efDate.Count = 0;
	efDate.type=type;

}
Buff::~Buff()
{

}


/*
 * 目的（BuffのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Buff::Update()
{
	efDate.Count++;

	if(efDate.Count>= efDate.Timer)
	{
		Do_buff = false;
		efDate.use_ = false;
		efDate.Effect = 0;
		
	}
	else
	{
		DrawFormatString(200, 800, GetColor(255, 0, 0), "buff使用中");
	}


}

