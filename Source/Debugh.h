#pragma once
#include"Dxlib.h"


class Debug
{
public:

	bool Getdebug() { return debugOn; }
	void SetDebug(bool debug) { debugOn = debug; }

private:
	bool debugOn = true;



};