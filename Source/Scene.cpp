#include"Scene.h"
#include"ObjectManager.h"
#include"Master.h"
#include"ColliderManager.h"


/// @brief Sceneの初期化（コンストラクタ）
Scene::Scene()
{
	// オブジェクトマネージャーの生成
	m_ObjectManager = new ObjectManager();
	m_ColliderManager = new ColliderManager();
}

Scene::~Scene()
{
	if (m_ObjectManager != nullptr)
	{
		delete m_ObjectManager;
	}
	
}
/// @brief 描画


/// @brief Sceneの描画処理
void Scene::Draw()
{
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Draw();
	}
}
/// @brief 更新


/// @brief Sceneの状態更新処理
void Scene::Update()
{
	if (m_ObjectManager != nullptr)
	{
		
		m_ObjectManager->Update();
	}
}