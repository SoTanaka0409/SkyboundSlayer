#pragma once
#include "DxLib.h"

class Config
{
public:
	// ゲーム画面の幅
    static constexpr int ScreenWidth = 1980;
    // ゲーム画面の高さ
    static constexpr int ScreenHeight = 1080;

	// カメラ設定
	static constexpr float CameraFar = 80000.0f;
	
	// 3Dステージのサイズ
	static constexpr float StageRadius_x = 4000.0f;
	static constexpr float StageRadius_z = 5000.0f;
	static constexpr float StageWallDistance = 10000.0f;
	static constexpr float BossStageRadius = 3000;
	
	static constexpr float StageBossWallDistance = 4000.0f;

	// ステージ中心座標
	static VECTOR GetStageCenter()
	{
		return VGet(-1500.0f, 0.0f, -5000.0f);
	}
	static VECTOR GetStageBossCenter()
	{
		return VGet(0, 5040, -20000);
	}
};
