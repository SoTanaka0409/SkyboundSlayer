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
		bool use_=false;
		
	}efDate;

public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Buff(int timer,float effect,Object3D::StatusState type);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Buff();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

private:
	
	
	float OldNum;//それぞれのbuffをする前の値、buffが終わったら返す
	
	bool Do_buff;
};