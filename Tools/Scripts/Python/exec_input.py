# -*- coding: utf-8 -*-
import os

h_path = r'Source\InputManager.h'
cpp_path = r'Source\InputManager.cpp'

with open(h_path, 'r', encoding='shift_jis', errors='ignore') as f:
    h_content = f.read()
    
if 'CheckMouseClickLeft' not in h_content:
    h_content = h_content.replace('static int CheckPressKey(int KeyCode);', 
'''static int CheckPressKey(int KeyCode);

	// 左マウスクリック（タップ）された瞬間だけ 1 を返す関数
	static int CheckMouseClickLeft();

	// マウスカーソルの現在座標を取得する関数
	static void GetMousePos(int& x, int& y);''')
    
    h_content = h_content.replace('private:',
'''private:
	static int mMouseDownBuffer;
''')
    
    with open(h_path, 'w', encoding='shift_jis', errors='replace') as f:
        f.write(h_content)
    print("InputManager.h updated.")

with open(cpp_path, 'r', encoding='shift_jis', errors='ignore') as f:
    cpp_content = f.read()

if 'CheckMouseClickLeft' not in cpp_content:
    cpp_content = cpp_content.replace('int InputManager::mUpBuffer[256] = { 0 };',
'''int InputManager::mUpBuffer[256] = { 0 };
int InputManager::mMouseDownBuffer = 0;''')

    new_funcs = '''
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
'''
    cpp_content += new_funcs
    with open(cpp_path, 'w', encoding='shift_jis', errors='replace') as f:
        f.write(cpp_content)
    print("InputManager.cpp updated.")

