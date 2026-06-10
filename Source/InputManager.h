#pragma once
class InputManager
{
public:
	InputManager();

	~InputManager();

	// 指定されたキーが押された瞬間だけ 1 を返す関数
	static int CheckDownKey(int KeyCode);

	// 指定されたキーが離された瞬間だけ 1 を返す関数
	static int CheckUpKey(int KeyCode);

	// 指定されたキーを押し続けている間１を返す関数
	static int CheckPressKey(int KeyCode);

private:
	static int mDownBuffer[256];
	static int mUpBuffer[256];
};