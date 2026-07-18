#pragma once
#include"Dxlib.h"


class Debug
{
public:
    // [入力] なし [出力] bool: デバッグモードフラグ [副作用] なし
	bool Getdebug() { return debugOn; }
    // [入力] bool debug [出力] なし [副作用] デバッグモードの切り替え
	void SetDebug(bool debug) { debugOn = debug; }

private:
	bool debugOn = false;



};