#pragma once
#include "DxLib.h"

class Config
{
public:
	// Screen Size
	static constexpr int ScreenWidth = 1980;
	static constexpr int ScreenHeight = 1080;

	// Camera Setting
	static constexpr float CameraFar = 80000.0f;

	// Scene3D Stage Sizes
	static constexpr float StageWallWidth = 4000.0f;
	static constexpr float StageWallDistance = 10000.0f;
	static constexpr float StageBossWallWidth = 2000.0f;
	static constexpr float StageBossWallDistance = 4000.0f;

	// Tutorial Stage Sizes
	static constexpr float TutorialWallWidth = 8000.0f;
	static constexpr float TutorialWallDistance = 2000.0f;
	static constexpr float TutorialBossWallWidth = 2500.0f;
	static constexpr float TutorialBossWallDistance = 5000.0f;

	// Stage Center Position
	static VECTOR GetStageCenter() {
		return VGet(-1500.0f, 0.0f, -5000.0f);
	}
};
