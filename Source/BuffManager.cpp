#include"BuffManager.h"

BuffManager::BuffManager()
{

}

BuffManager::~BuffManager()
{

}

void BuffManager::Draw()
{

}

void BuffManager::Update()
{
	for (auto b : buffList)
	{
		(*b).Update();

	}

}

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
	date->efDate.mbUse = true;
	buffList.push_back(date);
	
	
}

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

float BuffManager::GetBuff(Object3D::StatusState state)
{
	for (auto b : buffList)
	{
		if (b->efDate.type == state)
		{
			if ((*b).efDate.mbUse == true)
			{
				return b->efDate.Effect;
			}
		}
		/*if (b->efDate.type ==state)
		{
			if ((*b).efDate.mbUse == true)
			{
				return b->efDate.Effect;
			}
		}*/
		
		
		
		
	}

	return 0.0f;
}
