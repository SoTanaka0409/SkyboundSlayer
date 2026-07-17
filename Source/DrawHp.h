#pragma once
#include"DxLib.h"
#include"Object3D.h"
#include<vector>
#include"Enemy3D.h"

class Enemy; // 前方宣言

class DrawHp 
{
public:
	//centerPos,,,中心座標
	//topLeft... 左上座標
	//bottomRight...右下座標
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	DrawHp();//コンストラクタ

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~DrawHp();//デストラクタ

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();//更新
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();//描画

private:
	void UpdateHpBars();
	void DrawSingleHpBar(Enemy* pEne);

	static constexpr float kHpBarOffsetY = 250.0f;
	static constexpr int kHpBarWidth = 60;
	static constexpr int kHpBarHeight = 10;
};