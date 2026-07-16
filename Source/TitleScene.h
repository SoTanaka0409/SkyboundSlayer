#pragma once
#include "Dxlib.h"
#include "Scene.h"

class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	void UpdateTitleCamera();
	void HandleMenuInput();
	void UpdatePromptBlink();
	void DrawSceneBackground();
	void DrawTitlePanel();
	void DrawMenuPanel();
	void DrawPrompt();
	bool IsHoverStart(int mx, int my) const;
	bool IsHoverRule(int mx, int my) const;

	float mCameraAngle;
	int mnColorFade;
	bool mbColorFlag;
};
