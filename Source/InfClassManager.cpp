#include"InfClassManager.h"
#include"ItemManager.h"
#include"Master.h"

InfClassManager::InfClassManager()
{
	
	
}

void InfClassManager::Draw()
{
	

}

void InfClassManager::Update()
{
	
	int Lognumber = 0;
	for (auto Log = LogList.begin(); Log != LogList.end(); Log++)
	{
		
		(*Log)->SetPosition(VGet(0.0f, 700.0f - 30.0f * static_cast<float>(Lognumber), 0.0f));
		Lognumber++;
		
	}
	for (auto Log = LogList.begin(); Log != LogList.end(); )
	{
		if ((*Log)->GetElaseFlag() == true)
		{	
			Log = LogList.erase(Log);
		}
		else
		{
			Log++;
		}
		
	}

}


