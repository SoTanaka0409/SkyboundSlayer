#pragma once
class InputManager
{
public:
    // コンストラクタ
// [入力] なし [出力] なし [副作用] なし
	InputManager();

    // コンストラクタ
// [入力] なし [出力] なし [副作用] なし
    // デストラクタ
// [入力] なし [出力] なし [副作用] なし
	~InputManager();

	// 指定されたキーが押された瞬間だけ 1 を返す関数
	static int CheckDownKey(int KeyCode);

	// 指定されたキーが離された瞬間だけ 1 を返す関数
	static int CheckUpKey(int KeyCode);

	// 指定されたキーを押し続けている間１を返す関数
	static int CheckPressKey(int KeyCode);

	// 左マウスクリック（タップ）された瞬間だけ 1 を返す関数
	static int CheckMouseClickLeft();

	// マウスカーソルの現在座標を取得する関数
    // マウス座標を取得する
// [入力] int& x, int& y [出力] なし [副作用] 引数への代入
	static void GetMousePos(int& x, int& y);

private:
	static int mouse_down_buffer_;

	static int down_buffer_[256];
	static int up_buffer_[256];
};