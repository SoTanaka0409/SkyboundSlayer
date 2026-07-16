#pragma once
#include "Scene.h"
#include "GameManager.h"
#include "EnemyManager.h"

class SceneGame : public Scene
{
public:
	SceneGame(GameManager::Difficulty diff = GameManager::Difficulty::kNormal);
	virtual ~SceneGame();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

	/*
	 * Checks whether the game is in a shop phase.
	 * [Input] none
	 * [Output] true when the current phase is a shop phase
	 * [Side effects] none
	 */
	bool IsShopPhase() const;

	/*
	 * Checks whether the game is in a battle phase.
	 * [Input] none
	 * [Output] true when the current phase is a battle phase
	 * [Side effects] none
	 */
	bool IsBattlePhase() const;

	GameManager* game_manager_;
	EnemyManager* enemy_manager_;

protected:
	GameManager::Difficulty initial_difficulty_;
};
