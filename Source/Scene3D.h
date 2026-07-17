#pragma once
#include "SceneGame.h"
#include"Dxlib.h"
#include"Master.h"
#include"Texture.h"
#include"EnemyManager.h"
#include"GameManager.h"



class Scene3D : public SceneGame
{
private:
	Texture* texture_;
	Texture* texture2_;
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Scene3D();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Scene3D();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Initialize();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Finalize();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

private:
	void SetupEnvironment();
	void CreateInitialActors();
	void CreateStage();
	void LoadStageObjectsFromCsv();
	void CreateSkyBox();
	void DrawDebugGrid();

	float warning_radius_ = 0.0f;
	float max_radius_ = 520.0f;
	float load_timer_;
	float load_count_;
	bool is_load_flag_;


	VECTOR pos_ = { 200,400 };
	VECTOR size_ = { 600,100 };
};