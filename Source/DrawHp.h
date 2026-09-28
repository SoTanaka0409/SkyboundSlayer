#pragma once
#include"DxLib.h"
#include"Object3D.h"
#include<vector>
#include"Enemy3D.h"

class Enemy; // 前方宣言

class DrawHp 
{
public:
	/// @brief centerPos,,,中心座標
	/// @brief topLeft... 左上座標
	/// @brief bottomRight...右下座標
	DrawHp();//コンストラクタ
	~DrawHp();//デストラクタ
	void Update();//更新
	void Draw();//描画

private:
	void UpdateHpBars();
	void DrawSingleHpBar(Enemy* pEne);

	static constexpr float kHpBarOffsetY = 250.0f;
	static constexpr int kHpBarWidth = 60;
	static constexpr int kHpBarHeight = 10;
};