#pragma once
#include"Dxlib.h"
#include"Item.h"
#include"vector"
#include"Object2D.h"

class InfClass:public Object2D
{
public:
	
public:
	InfClass(int Log_timer,std::string Inf,int num);

	void Update();

	void SetPosition(VECTOR pos) { mvPosition = pos; }

	void Draw();

	bool GetElaseFlag() { return mbElaseFlag; }

private:
	int LogTime;
	std::string LogDate;//logを保存する
	int LogCount;//ログ場所の移動の貯めのカウント
	bool mbElaseFlag;//ewaseするためのフラグ

	int LogNumber;//使用するログの種類を選択
	
	
};