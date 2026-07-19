#pragma once

class Scene;
class SceneGame;

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
		kSceneResultScene
	};

	SceneManager();
	~SceneManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void ChangeSceneIfNeeded();

	void SetNextScene(SceneType next) { next_scene_type_ = next; }
	Scene* GetCurrentScene() { return current_scene_; }
	SceneType GetCurrentSceneType() { return scene_type_; }
	SceneGame* GetSceneGame();

private:
	SceneType scene_type_;
	SceneType next_scene_type_;
	Scene* current_scene_;
};