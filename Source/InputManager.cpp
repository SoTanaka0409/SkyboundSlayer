#include"InputManager.h"
#include"DxLib.h"

int InputManager::down_buffer_[256] = { 0 };
int InputManager::up_buffer_[256] = { 0 };
int InputManager::mouse_down_buffer_ = 0;


/// @brief InputManagerのコンストラクタ
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
	if (down_buffer_[KeyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	//現在のキーの状態をバッファに格納
	down_buffer_[KeyCode] = keyState;
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
	if (mouse_down_buffer_ == 0 && mouseState != 0)
	{
		result = 1;
	}
	mouse_down_buffer_ = mouseState;
	return result;
}


/// @brief マウスの座標を取得するため
/// @param int& x, int& y
/// @details 引数への代入
void InputManager::GetMousePos(int& x, int& y)
{
	GetMousePoint(&x, &y);
}
