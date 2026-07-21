#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>

/// @brief 会話ウィンドウ・テキストメッセージ（チャットUI）の描画および制御を行うクラス
class Chat
{
public:
	Chat();
	~Chat();

	/// @brief 指定した文字列メッセージをチャットUI枠内に描画する
	/// @param chatdate 表示するテキスト文字列
	void Draw(std::string chatdate);

	/// @brief チャット描画設定やタイマー状態をリセットする
	void Reset();

private:
	Texture* texture_; ///< 会話ウィンドウ背景枠等のテクスチャポインタ
	int first_font_;    ///< 描画に使用するフォントハンドル
	int fontsize;       ///< フォントサイズ
};