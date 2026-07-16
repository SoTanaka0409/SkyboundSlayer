#pragma once
#include"Dxlib.h"
#include"Scene.h"
#include"InputManager.h"

class Rule :public Scene
{
public:
	Rule();

	~Rule();

	void Initialize();

	void Draw();

	void Update();

	void Finalize();

private:
	void HandlePageInput();
	void DrawRuleBackground();
	void DrawRulePage1();
	void DrawRulePage2();
	void DrawRuleFooter();
	int mPage;
	int mBgHandle1;
	int mBgHandle2;
};
