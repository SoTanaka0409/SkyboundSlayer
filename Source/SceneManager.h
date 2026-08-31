#pragma once

class Scene;
class SceneGame;

/// @brief シーンの遷移管理や、現在アクティブなシーンの実行（更新・描画）を統括するマネージャー類
class SceneManager
{
public:
	/// @brief シーンの種類を識別する列挙型
	enum SceneType
	{
		kSceneNone = 0,     ///< シーン未設定状態
		kSceneTitle,        ///< タイトル画面シーン
		kSceneRule,         ///< ルール説明画面シーン
		kSceneSettings,     ///< 設定画面シーン
		kGameScene,           ///< 3Dゲーム本編（メインゲーム）シーン
		kSceneResultScene   ///< リザルト画面シーン
	};

	SceneManager();
	~SceneManager();

	/// @brief 初期シーンの構築および管理用パラメータの初期化を行う
	void Initialize();

	/// @brief 現在アクティブなシーンの毎フレームの更新処理およびシーン遷移チェックを行う
	void Update();

	/// @brief 現在アクティブなシーンの描画処理を呼び出す
	void Draw();

	/// @brief シーンマネージャーの終了処理および現在シーンのメモリ解放を行う
	void Finalize();

	/// @brief 次のシーンへの遷移要求（next_scene_type_）がある場合、シーンの切り替えを実行する
	void ChangeSceneIfNeeded();

	/// @brief 次に遷移するシーンの種類を設定する
	/// @param next 遷移先のSceneType
	void SetNextScene(SceneType next) { next_scene_type_ = next; }

	/// @brief 現在アクティブなシーンのポインタを取得する
	/// @return Scene* 現在のシーンポインタ
	Scene* GetCurrentScene() { return current_scene_; }

	/// @brief 現在のシーンの種類（SceneType）を取得する
	/// @return SceneType 現在のシーン識別タイプ
	SceneType GetCurrentSceneType() { return scene_type_; }

	/// @brief ゲーム本編シーン（SceneGame）のポインタを取得する
	/// @return SceneGame* ゲーム本編シーンポインタ（他シーンの場合はnullptr）
	SceneGame* GetSceneGame();

private:
	SceneType scene_type_;        ///< 現在実行中のシーン識別タイプ
	SceneType next_scene_type_;   ///< 次フレームで遷移予約されているシーン識別タイプ
	Scene* current_scene_;        ///< 現在アクティブなシーンオブジェクトへのポインタ
};