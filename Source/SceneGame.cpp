#include "SceneGame.h"



/// @brief SceneGameの初期化（コンストラクタ）
SceneGame::SceneGame(GameManager::Difficulty diff)
	: m_InitialDifficulty(diff)
	, m_GameManager(nullptr)
	, m_EnemyManager(nullptr)
{
}

SceneGame::~SceneGame()
{
}



/// @brief SceneGameの初期化処理
void SceneGame::Initialize()
{
	if (m_EnemyManager == nullptr)
	{
		m_EnemyManager = new EnemyManager();
	}
	if (m_GameManager == nullptr)
	{
		m_GameManager = new GameManager(m_EnemyManager, m_InitialDifficulty);
	}
}



/// @brief SceneGameの状態更新処理
void SceneGame::Update()
{
	if (Master::m_HitStopTimer > 0)
	{
		Master::m_HitStopTimer--;
		return;
	}

	if (!Master::m_IsCutscenePlaying) {
		Scene::Update();
	}
	if (m_GameManager)
	{
		m_GameManager->Update();
	}
}



/// @brief SceneGameの描画処理
void SceneGame::Draw()
{
	Scene::Draw();
	if (m_GameManager)
	{
		m_GameManager->Draw();
	}
}



/// @brief SceneGameのFinalize処理
void SceneGame::Finalize()
{
	if (m_GameManager)
	{
		delete m_GameManager;
		m_GameManager = nullptr;
	}
	if (m_EnemyManager)
	{
		delete m_EnemyManager;
		m_EnemyManager = nullptr;
	}
}



/// @brief SceneGameのIsShopPhase処理
bool SceneGame::IsShopPhase() const
{
	if (!m_GameManager) return false;
	auto phase = m_GameManager->GetCurrentPhase();
	return (phase == GameManager::Phase::kShop1 ||
			phase == GameManager::Phase::kShop2 ||
			phase == GameManager::Phase::kShop3);
}



/// @brief SceneGameのIsBattlePhase処理
bool SceneGame::IsBattlePhase() const
{
	if (!m_GameManager) return false;
	auto phase = m_GameManager->GetCurrentPhase();
	return (phase == GameManager::Phase::kPhase1 ||
			phase == GameManager::Phase::kPhase2 ||
			phase == GameManager::Phase::kPhase3 ||
			phase == GameManager::Phase::kBoss);
}

