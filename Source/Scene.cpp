#include"Scene.h"
#include"ObjectManager.h"
#include"Master.h"
#include"ColliderManager.h"
Scene::Scene()
{
	//オブジェクトマネージャーの生成
	mpObjectManager = new ObjectManager();
	mpColliderManager = new ColliderManager();
}

Scene::~Scene()
{
	if (mpObjectManager != nullptr)
	{
		delete mpObjectManager;
	}
	
}
//描画
void Scene::Draw()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Draw();
	}
}
//更新
void Scene::Update()
{
	if (mpObjectManager != nullptr)
	{
		
		mpObjectManager->Update();
	}
}