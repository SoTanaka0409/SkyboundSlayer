#pragma once

class Scene;
class SceneGame;

// アーキテクチャ設計：ゲーム内の各フェーズ（タイトル、本編、リザルト等）を一元管理し、シーン間の安全な移行、メモリ管理、および更新・描画の委譲を担うステート管理ハブ
class SceneManager
{
public:
	enum SceneType
	{
		kSceneNone = 0,
		kSceneTitle,
		kSceneRule,
		kSceneSettings,
		kScene3D,
		kSceneResultWin
	};

	SceneManager();
	~SceneManager();

	// 入力：なし
	// 出力：なし
	// 副作用：初期シーンへの遷移予約およびシーン構築処理の実行
	void Initialize();

	// 入力：なし
	// 出力：なし
	// 副作用：現在アクティブなシーンのUpdateおよびフェード演出の進行
	void Update();

	// 入力：なし
	// 出力：なし
	// 副作用：現在アクティブなシーンのDrawおよびフェード演出のレンダリング
	void Draw();

	// 入力：なし
	// 出力：なし
	// 副作用：全シーン管理リソースの破棄
	void Finalize();

	// 入力：なし
	// 出力：なし
	// 副作用：フェードアウト完了検知後、古いシーンの破棄と新しいシーンの構築、およびフェードインの開始
	void ChangeSceneIfNeeded();

	// 入力：next = 遷移先シーンID
	// 出力：なし
	// 副作用：次フレーム移行用の遷移先ステート更新
	void SetNextScene(SceneType next) { next_scene_type_ = next; }

	// 入力：なし
	// 出力：現在アクティブなシーンへのポインタ
	// 副作用：なし
	Scene* GetCurrentScene() { return current_scene_; }

	// 入力：なし
	// 出力：現在アクティブなシーンの列挙型ID
	// 副作用：なし
	SceneType GetCurrentSceneType() { return scene_type_; }

	// 入力：なし
	// 出力：SceneGameへのダウンキャストポインタ（ゲーム本編でない場合はnullptr）
	// 副作用：なし
	SceneGame* GetSceneGame();

private:
	SceneType scene_type_;
	SceneType next_scene_type_;
	Scene* current_scene_;
};