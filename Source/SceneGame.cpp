#include "SceneGame.h"

SceneGame::SceneGame(GameManager::Difficulty diff)
	: mInitialDifficulty(diff)
	, mpGameManager(nullptr)
	, mpEnemyManager(nullptr)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Initialize()
{
	if (mpEnemyManager == nullptr)
	{
		mpEnemyManager = new EnemyManager();
	}
	if (mpGameManager == nullptr)
	{
		mpGameManager = new GameManager(mpEnemyManager, mInitialDifficulty);
	}
}

void SceneGame::Update()
{
	Scene::Update();
	if (mpGameManager)
	{
		mpGameManager->Update();
	}
}

void SceneGame::Draw()
{
	Scene::Draw();
	if (mpGameManager)
	{
		mpGameManager->Draw();
	}
}

void SceneGame::Finalize()
{
	if (mpGameManager)
	{
		delete mpGameManager;
		mpGameManager = nullptr;
	}
	if (mpEnemyManager)
	{
		delete mpEnemyManager;
		mpEnemyManager = nullptr;
	}
}

bool SceneGame::IsShopPhase() const
{
	if (!mpGameManager) return false;
	auto phase = mpGameManager->GetCurrentPhase();
	return (phase == GameManager::Phase::SHOP_1 ||
			phase == GameManager::Phase::SHOP_2 ||
			phase == GameManager::Phase::SHOP_3);
}

bool SceneGame::IsBattlePhase() const
{
	if (!mpGameManager) return false;
	auto phase = mpGameManager->GetCurrentPhase();
	return (phase == GameManager::Phase::PHASE_1 ||
			phase == GameManager::Phase::PHASE_2 ||
			phase == GameManager::Phase::PHASE_3 ||
			phase == GameManager::Phase::BOSS);
}
