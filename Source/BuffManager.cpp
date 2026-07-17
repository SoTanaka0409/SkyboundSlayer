#include"BuffManager.h"


/*
 * 目的（BuffManagerのBuffManager処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
BuffManager::BuffManager()
{

}

BuffManager::~BuffManager()
{

}


/*
 * 目的（BuffManagerのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void BuffManager::Draw()
{

}


/*
 * 目的（BuffManagerのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void BuffManager::Update()
{
	for (auto b : buffList)
	{
		(*b).Update();

	}

}


/*
 * 目的（BuffManagerのAddBuff処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（BuffManagerのDeleteList処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（BuffManagerのGetBuff処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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
