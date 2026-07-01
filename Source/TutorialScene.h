#pragma once
#include"DxLib.h"
#include"Player3D.h"
#include"Model.h"
#include"Texture.h"
#include"Scene.h"
#include"Camera.h"
#include"Enemy3D.h"
#include"EnemyManager.h"

class TutorialScene :public Scene
{
public:
	TutorialScene();
	~TutorialScene();

	void Initialize();

	void Draw();

	void Update();

	void Finalize();

	

private:
	Texture* mpTexture;
	int mnTutorialCount;
	EnemyManager* mpEnemyManager;

};