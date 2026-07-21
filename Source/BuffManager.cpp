#include"BuffManager.h"


/// @brief BuffManagerの初期化（コンストラクタ）
BuffManager::BuffManager()
{

}

BuffManager::~BuffManager()
{

}


/// @brief BuffManagerの描画処理
void BuffManager::Draw()
{

}


/// @brief BuffManagerの状態更新処理
void BuffManager::Update()
{
	for (auto b : buffList)
	{
		(*b).Update();

	}

}


/// @brief BuffManagerのAddBuff処理
void BuffManager::AddBuff(Buff* date)
{
	
	for (auto b : buffList)
	{
		if (b->efDate.type==date->efDate.type)
		{
			b->efDate.type = date->efDate.type;
			b->efDate.Count = date->efDate.Count;
			
			
			date->efDate.Effect = 0;
			
			return;
		}
	}
	date->efDate.use_ = true;
	buffList.push_back(date);
	
	
}


/// @brief BuffManagerのDeleteList処理
void BuffManager::DeleteList()
{
	for (auto b=buffList.begin();b!=buffList.end();)
	{
		if ((*b)->efDate.Timer<=(*b)->efDate.Count)
		{
			b = buffList.erase(b);
			return;
		}
		else
		{
			b++;
		}
	}

}


/// @brief BuffManagerのGetBuff処理
float BuffManager::GetBuff(Object3D::StatusState state)
{
	for (auto b : buffList)
	{
		if (b->efDate.type == state)
		{
			if ((*b).efDate.use_ == true)
			{
				return b->efDate.Effect;
			}
		}
		/*if (b->efDate.type ==state)
		{
			if ((*b).efDate.use_ == true)
			{
				return b->efDate.Effect;
			}
		}*/
		
		
		
		
	}

	return 0.0f;
}
