#pragma once
#include"Dxlib.h"
#include"Texture.h"
#include"Scene.h"
#include"TitleScene.h"
class ResultWin:public Scene
{
public:
	ResultWin();

	~ResultWin();

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void SetName(char name) { Name = name; }
private:
	Texture* mpTexture;
	TitleScene* mpTitle;

	int AllTimer;
	char Name;
	
};