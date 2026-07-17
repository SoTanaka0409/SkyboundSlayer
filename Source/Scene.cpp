#include"Scene.h"
#include"ObjectManager.h"
#include"Master.h"
#include"ColliderManager.h"


/*
 * 目的（SceneのScene処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（SceneのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Scene::Draw()
{
	if (object_manager_ != nullptr)
	{
		object_manager_->Draw();
	}
}
//更新


/*
 * 目的（SceneのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Scene::Update()
{
	if (object_manager_ != nullptr)
	{
		
		object_manager_->Update();
	}
}