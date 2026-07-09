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
		
		(*Log)->SetPosition(VGet(0, 700 - 30 * Lognumber, 0));
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

