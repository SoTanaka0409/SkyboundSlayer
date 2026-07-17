#pragma once

class Scene;
class SceneGame;

class SceneManager
{
public:
	// ゲームの流れで使用されるシーンのカテゴリ
	enum SceneType
	{
		kSceneNone = 0,
		kSceneTitle,
		kSceneRule,
		kScene3D,
		kSceneResultWin
	};

	SceneManager();
	~SceneManager();
// シーンマネージャーの初期化
    // [入力] なし [出力] なし [副作用] 初期シーンの生成

	void Initialize();
// シーンの更新処理
    // [入力] なし [出力] なし [副作用] 現在のシーンのUpdate呼び出し
	void Update();
// シーンの描画処理
    // [入力] なし [出力] なし [副作用] 現在のシーンのDraw呼び出し
	void Draw();
// シーンの終了処理
    // [入力] なし [出力] なし [副作用] 現在のシーンの破棄
	void Finalize();
// 予約されている次のシーンへの切り替え処理
    // [入力] なし [出力] なし [副作用] 現在のシーンの破棄と新しいシーンの生成

		void ChangeSceneIfNeeded();
// 次に遷移するシーンを予約する
    // [入力] SceneType next [出力] なし [副作用] next_scene_type_の変更

		void SetNextScene(SceneType next) { next_scene_type_ = next; }
// 現在のシーンインスタンスを取得する
    // [入力] なし [出力] Scene*: 現在のシーン [副作用] なし

		Scene* GetCurrentScene() { return current_scene_; }
// 現在のシーンタイプを取得する
    // [入力] なし [出力] SceneType: 現在のシーンタイプ [副作用] なし

		SceneType GetCurrentSceneType() { return scene_type_; }
// 現在のシーンがゲームシーンであれば取得する
    // [入力] なし [出力] SceneGame*: ゲームシーンのポインタ [副作用] なし
	SceneGame* GetSceneGame();

private:
	SceneType scene_type_;
	SceneType next_scene_type_;
	Scene* current_scene_;
};
