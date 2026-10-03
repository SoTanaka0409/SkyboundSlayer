#include"Buff.h"
#include"Master.h"
#include"ObjectManager.h"


/// @brief Buffの初期化（コンストラクタ）
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


/// @brief Buffの状態更新処理
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
		if (Master::debug_ != nullptr && Master::debug_->Getdebug()) DrawFormatString(200, 800, GetColor(255, 0, 0), "buff使用中");
	}


}

