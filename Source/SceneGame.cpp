#include "SceneGame.h"



/// @brief SceneGameの初期化（コンストラクタ）
SceneGame::SceneGame(GameManager::Difficulty diff)
	: initial_difficulty_(diff)
	, game_manager_(nullptr)
	, enemy_manager_(nullptr)
{
}

SceneGame::~SceneGame()
{
}



/// @brief SceneGameの初期化処理
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



/// @brief SceneGameの状態更新処理
void SceneGame::Update()
{
	if (Master::hit_stop_timer_ > 0)
	{
		Master::hit_stop_timer_--;
		return;
	}

	if (!Master::is_cutscene_playing_) {
		Scene::Update();
	}
	if (game_manager_)
	{
		game_manager_->Update();
	}
}



/// @brief SceneGameの描画処理
void SceneGame::Draw()
{
	Scene::Draw();
	if (game_manager_)
	{
		game_manager_->Draw();
	}
}



/// @brief SceneGameのFinalize処理
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



/// @brief SceneGameのIsShopPhase処理
bool SceneGame::IsShopPhase() const
{
	if (!game_manager_) return false;
	auto phase = game_manager_->GetCurrentPhase();
	return (phase == GameManager::Phase::kShop1 ||
			phase == GameManager::Phase::kShop2 ||
			phase == GameManager::Phase::kShop3);
}



/// @brief SceneGameのIsBattlePhase処理
bool SceneGame::IsBattlePhase() const
{
	if (!game_manager_) return false;
	auto phase = game_manager_->GetCurrentPhase();
	return (phase == GameManager::Phase::kPhase1 ||
			phase == GameManager::Phase::kPhase2 ||
			phase == GameManager::Phase::kPhase3 ||
			phase == GameManager::Phase::kBoss);
}

