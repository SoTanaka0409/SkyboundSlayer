#pragma once
#include "Scene.h"
#include "GameManager.h"
#include "EnemyManager.h"

/// @brief ゲーム本編（3Dバトル・ショップ巡回等）の進行を管理するシーンクラス
class SceneGame : public Scene
{
public:
	/// @brief SceneGameのコンストラクタ
	/// @param diff ゲーム開始時の選択難易度（既定値: kNormal）
	SceneGame(GameManager::Difficulty diff = GameManager::Difficulty::kNormal);

	/// @brief SceneGameのデストラクタ
	virtual ~SceneGame();

	/// @brief ゲームシーンの初期化処理
	/// @details プレイヤー、ステージ、各種マネージャー（GameManager/EnemyManager）の生成と初期化を行う
	virtual void Initialize() override;

	/// @brief ゲームシーンの毎フレーム更新処理
	/// @details ゲーム進行制御（GameManager）や各種オブジェクトの更新を実行する
	virtual void Update() override;

	/// @brief ゲームシーンの描画処理
	/// @details 3Dステージ・キャラクター・HUD等の描画を実行する
	virtual void Draw() override;

	/// @brief ゲームシーンの終了・解放処理
	/// @details シーン破棄時に管理下のマネージャーや動的オブジェクトを安全に解放する
	virtual void Finalize() override;

	/// @brief 現在のゲーム状態がショップフェーズ（強化・準備期間）か判定する
	/// @return bool ショップフェーズであればtrue
	bool IsShopPhase() const;

	/// @brief 現在のゲーム状態がバトルフェーズ（ウェーブ戦闘中）か判定する
	/// @return bool バトルフェーズであればtrue
	bool IsBattlePhase() const;

public:
	GameManager* game_manager_;   ///< フェーズ進行やルール判定を統括するマネージャーポインタ
	EnemyManager* enemy_manager_; ///< 敵オブジェクトの生成・管理を担当するマネージャーポインタ

protected:
	GameManager::Difficulty initial_difficulty_; ///< シーン生成時に指定された選択難易度設定
};