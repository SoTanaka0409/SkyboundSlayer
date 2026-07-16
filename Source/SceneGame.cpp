#include "SceneGame.h"

SceneGame::SceneGame(GameManager::Difficulty diff)
	: initial_difficulty_(diff)
	, game_manager_(nullptr)
	, enemy_manager_(nullptr)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Initialize()
{
	if (enemy_manager_ == nullptr)
	{
		enemy_manager_ = new EnemyManager();
	}
	if (game_manager_ == nullptr)
	{
		game_manager_ = new GameManager(enemy_manager_, initial_difficulty_);
	}
}

void SceneGame::Update()
{
	if (!Master::CutscenePlaying) {
		Scene::Update();
	}
	if (game_manager_)
	{
		game_manager_->Update();
	}
}

void SceneGame::Draw()
{
	Scene::Draw();
	if (game_manager_)
	{
		game_manager_->Draw();
	}
}

void SceneGame::Finalize()
{
	if (game_manager_)
	{
		delete game_manager_;
		game_manager_ = nullptr;
	}
	if (enemy_manager_)
	{
		delete enemy_manager_;
		enemy_manager_ = nullptr;
	}
}

bool SceneGame::IsShopPhase() const
{
	if (!game_manager_) return false;
	auto phase = game_manager_->GetCurrentPhase();
	return (phase == GameManager::Phase::kShop1 ||
			phase == GameManager::Phase::kShop2 ||
			phase == GameManager::Phase::kShop3);
}

bool SceneGame::IsBattlePhase() const
{
	if (!game_manager_) return false;
	auto phase = game_manager_->GetCurrentPhase();
	return (phase == GameManager::Phase::kPhase1 ||
			phase == GameManager::Phase::kPhase2 ||
			phase == GameManager::Phase::kPhase3 ||
			phase == GameManager::Phase::kBoss);
}

