#pragma once
#include"Dxlib.h"
#include"Scene.h"
#include"Texture.h"


class TitleScene :public Scene
{
public:
	TitleScene();

	~TitleScene();

	void Initialize();

	void Draw();

	void Update();

	void Finalize();

private:
	Texture* mpTexture;
	Scene* mpScene;
	int mnC;
	int mnS;
	int mnE;

	bool C;
	bool S;
	bool E;

	bool C_Enter;
	bool S_Enter;
	bool E_Enter;

	int mnPause;
	int size = GetFontSize();
};