#pragma once
#include"dxlib.h"
#include"Object2D.h"
#include"Object3D.h"

class Buff
{
public:
	
	struct effectDate
	{
		Object3D::StatusState type;
		int Timer;
		int Count;
		float Effect;
		bool mbUse=false;
		
	}efDate;

public:
	Buff(int timer,float effect,Object3D::StatusState type);
	~Buff();

	void Update();

private:
	
	
	float OldNum;//それぞれのbuffをする前の値、buffが終わったら返す
	
	bool Do_buff;
};