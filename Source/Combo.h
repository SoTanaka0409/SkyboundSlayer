#pragma once
#include "DxLib.h"
#include "Object2D.h"
#include "Texture.h"
#include <string>
#include <iostream>
#include <vector>

/// @brief コンボ数（ヒット数）の保持および数字UIテクスチャの描画を行う2Dオブジェクトクラス
class Combo : public Object2D
{
public:
	Combo();
	~Combo();

	/// @brief コンボのタイマー減算や演出更新を行う
	void Update() override;

	/// @brief 現在のコンボ数に対応する数字テクスチャを画面に描画する
	void Draw() override;

	/// @brief 攻撃ヒット時にコンボ数を加算し、タイマーや表示演出をリセット・更新する
	void HitAddCombo();

private:
	int m_Combo;          ///< 現在の連続ヒット（コンボ）数
	int handle;          ///< コンボ表示用グラフィックハンドル

	Texture* m_Texture0; 
	Texture* m_Texture1; 
	Texture* m_Texture2; 
	Texture* m_Texture3; 
	Texture* m_Texture4; 
	Texture* m_Texture5; 
	Texture* m_Texture6; 
	Texture* m_Texture7; 
	Texture* m_Texture8; 
	Texture* m_Texture9; 
};