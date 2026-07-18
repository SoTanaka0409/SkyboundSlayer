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

	
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	bool IsShopPhase() const;


    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	bool IsBattlePhase() const;

	GameManager* game_manager_;
	EnemyManager* enemy_manager_;

protected:
	GameManager::Difficulty initial_difficulty_;
};
