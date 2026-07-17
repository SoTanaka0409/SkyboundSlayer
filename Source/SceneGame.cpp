#include "SceneGame.h"



/*
 * 目的（SceneGameのSceneGame処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
SceneGame::SceneGame(GameManager::Difficulty diff)
	: initial_difficulty_(diff)
	, game_manager_(nullptr)
	, enemy_manager_(nullptr)
{
}

SceneGame::~SceneGame()
{
}



/*
 * 目的（SceneGameのInitialize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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



/*
 * 目的（SceneGameのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SceneGame::Update()
{
	if (!Master::is_cutscene_playing_) {
		Scene::Update();
	}
	if (game_manager_)
	{
		game_manager_->Update();
	}
}



/*
 * 目的（SceneGameのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SceneGame::Draw()
{
	Scene::Draw();
	if (game_manager_)
	{
		game_manager_->Draw();
	}
}



/*
 * 目的（SceneGameのFinalize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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



/*
 * 目的（SceneGameのIsShopPhase処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
bool SceneGame::IsShopPhase() const
{
	if (!game_manager_) return false;
	auto phase = game_manager_->GetCurrentPhase();
	return (phase == GameManager::Phase::kShop1 ||
			phase == GameManager::Phase::kShop2 ||
			phase == GameManager::Phase::kShop3);
}



/*
 * 目的（SceneGameのIsBattlePhase処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
bool SceneGame::IsBattlePhase() const
{
	if (!game_manager_) return false;
	auto phase = game_manager_->GetCurrentPhase();
	return (phase == GameManager::Phase::kPhase1 ||
			phase == GameManager::Phase::kPhase2 ||
			phase == GameManager::Phase::kPhase3 ||
			phase == GameManager::Phase::kBoss);
}

