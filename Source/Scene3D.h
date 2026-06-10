#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Texture.h"
#include"EnemyManager.h"



class Scene3D :public Scene
{
private:
	Texture* mpTexture;
	Texture* mpTexture2;
	EnemyManager* mpEnemyManager;
public:
	Scene3D();
	~Scene3D();


	enum class Phase {
		PHASE_1,
		PHASE_2,
		BOSS,
		CLEAR
	};

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

	Phase mCurrentPhase;
	void SpawnPhaseEnemies();
	VECTOR pos = { 200,400 };
	VECTOR size = { 600,100 };
};