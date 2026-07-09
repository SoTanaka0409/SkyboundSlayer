#pragma once
#include"Dxlib.h"


class ResultChange
{
public:
	ResultChange();

	~ResultChange();

	void Draw();

	void Update();
	bool GetWin() { return mnWin; }
	void SetWin(bool win) { mnWin = win; }
	bool GetLose() { return mnLose; }
	void SetLose(bool lose) { mnLose = lose; }
private:
	bool mnWin;
	bool mnLose;


};