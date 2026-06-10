#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Texture.h"
#include"EnemyManager.h"
#include"GameManager.h"



class Scene3D :public Scene
{
private:
	Texture* mpTexture;
	Texture* mpTexture2;
	EnemyManager* mpEnemyManager;
public:
	Scene3D();
	~Scene3D();


	GameManager* mpGameManager;

	void Initialize();
	void Finalize();
	void Draw();
	void Update();

	
private:
	float warningRadius = 0.0f;
	float maxRadius = 520.0f;
	float LoadTimer;
	float LoadCount;
	bool Loadflag;


	VECTOR pos = { 200,400 };
	VECTOR size = { 600,100 };
};