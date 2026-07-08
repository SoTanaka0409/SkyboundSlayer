#include"InputManager.h"
#include"DxLib.h"

int InputManager::mDownBuffer[256] = { 0 };
int InputManager::mUpBuffer[256] = { 0 };
int InputManager::mMouseDownBuffer = 0;

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

int InputManager::CheckDownKey(int KeyCode)
{
	// 戻り値用の変数を用意
	int result = 0;

	// 指定キーの現在の状態を取得
	int keyState = CheckHitKey(KeyCode);
	
	//前回キーが押されておらず、現在キーが押されていたら
	if (mDownBuffer[KeyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	//現在のキーの状態をバッファに格納
	mDownBuffer[KeyCode] = keyState;
	return result;
}
int InputManager::CheckPressKey(int KeyCode)
{
	return CheckHitKey(KeyCode);
}
int InputManager::CheckMouseClickLeft()
{
	int result = 0;
	int mouseState = GetMouseInput() & MOUSE_INPUT_LEFT;
	if (mMouseDownBuffer == 0 && mouseState != 0)
	{
		result = 1;
	}
	mMouseDownBuffer = mouseState;
	return result;
}

void InputManager::GetMousePos(int& x, int& y)
{
	GetMousePoint(&x, &y);
}
