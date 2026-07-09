#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Texture.h"
#include"EnemyManager.h"
class ResetScene :public Scene
{
private:
	Texture* mpTexture;
	Texture* mpTexture2;
	EnemyManager* mpEnemyManager;
public:
	
	void SetResetScene();

	void Initialize();
	void Finalize(){}
	void Draw();
	void Update();
private:
	float warningRadius = 0.0f;
	float maxRadius = 520.0f;
};