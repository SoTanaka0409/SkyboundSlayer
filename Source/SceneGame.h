#pragma once
#include "Scene.h"
#include "GameManager.h"
#include "EnemyManager.h"

class SceneGame : public Scene
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	SceneGame(GameManager::Difficulty diff = GameManager::Difficulty::kNormal);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual ~SceneGame();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual void Initialize() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual void Update() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual void Draw() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual void Finalize() override;

	/*
	 * Checks whether the game is in a shop phase.
	 * [Input] none
	 * [Output] true when the current phase is a shop phase
	 * [Side effects] none
	 */
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	bool IsShopPhase() const;

	/*
	 * Checks whether the game is in a battle phase.
	 * [Input] none
	 * [Output] true when the current phase is a battle phase
	 * [Side effects] none
	 */
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	bool IsBattlePhase() const;

	GameManager* game_manager_;
	EnemyManager* enemy_manager_;

protected:
	GameManager::Difficulty initial_difficulty_;
};
