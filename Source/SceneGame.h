#pragma once
#include "Scene.h"
#include "GameManager.h"
#include "EnemyManager.h"

class SceneGame : public Scene
{
public:
	SceneGame(GameManager::Difficulty diff = GameManager::Difficulty::NORMAL);
	virtual ~SceneGame();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

	bool IsShopPhase() const;
	bool IsBattlePhase() const;

	GameManager* mpGameManager;
	EnemyManager* mpEnemyManager;

protected:
	GameManager::Difficulty mInitialDifficulty;
};
