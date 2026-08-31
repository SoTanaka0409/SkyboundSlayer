#include"Scene.h"
#include"ObjectManager.h"
#include"Master.h"
#include"ColliderManager.h"


/// @brief Sceneの初期化（コンストラクタ）
Scene::Scene()
{
	//オブジェクトマネージャーの生成
	object_manager_ = new ObjectManager();
	collider_manager_ = new ColliderManager();
}

Scene::~Scene()
{
	if (object_manager_ != nullptr)
	{
		delete object_manager_;
	}
	
}
//描画


/// @brief Sceneの描画処理
void Scene::Draw()
{
	if (object_manager_ != nullptr)
	{
		object_manager_->Draw();
	}
}
//更新


/// @brief Sceneの状態更新処理
void Scene::Update()
{
	if (object_manager_ != nullptr)
	{
		
		object_manager_->Update();
	}
}