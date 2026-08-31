#pragma once
#include "DxLib.h"

/// @brief キーボードおよびマウスの入力状態（押下・離脱・ホールド判定）を一括管理する静的クラス
class InputManager
{
public:
	/// @brief InputManagerのコンストラクタ
	InputManager();

	/// @brief InputManagerのデストラクタ
	~InputManager();

	/// @brief 指定されたキーが押された瞬間だけ1（真）を返す
	/// @param KeyCode DxLibのキーコード（例: KEY_INPUT_SPACE）
	/// @return int 押された瞬間であれば1、それ以外は0
	static int CheckDownKey(int KeyCode);

	/// @brief 指定されたキーが離された瞬間だけ1（真）を返す
	/// @param KeyCode DxLibのキーコード
	/// @return int 離された瞬間であれば1、それ以外は0
	static int CheckUpKey(int KeyCode);

	/// @brief 指定されたキーが押され続けている間1（真）を返す
	/// @param KeyCode DxLibのキーコード
	/// @return int 押下中であれば1、それ以外は0
	static int CheckPressKey(int KeyCode);

	/// @brief マウスの左クリック（タップ）が発生した瞬間だけ1（真）を返す
	/// @return int クリックされた瞬間であれば1、それ以外は0
	static int CheckMouseClickLeft();

	/// @brief マウスカーソルの現在の画面座標を取得する
	/// @param x X座標の格納先参照
	/// @param y Y座標の格納先参照
	static void GetMousePos(int& x, int& y);

private:
	static int mouse_down_buffer_;    ///< 前回フレームのマウス左ボタン押下状態バッファ
	static int down_buffer_[256];     ///< 各キーの押下瞬間判定用状態バッファ
	static int up_buffer_[256];       ///< 各キーの離脱瞬間判定用状態バッファ
};