#pragma once

class Scene;
class SceneGame;

class SceneManager
{
public:
	// Scene categories used by the game flow.
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

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	/*
	 * Switches to the reserved next scene while respecting fade state.
	 * [Input] none
	 * [Output] none
	 * [Side effects] Recreates current_scene_, clears colliders, starts Fade.
	 */
	void ChangeSceneIfNeeded();

	/*
	 * Reserves the next scene type.
	 * [Input] next: scene type to switch to
	 * [Output] none
	 * [Side effects] Updates next_scene_type_.
	 */
	void SetNextScene(SceneType next) { next_scene_type_ = next; }

	/*
	 * Returns the current scene instance.
	 * [Input] none
	 * [Output] Current Scene pointer
	 * [Side effects] none
	 */
	Scene* GetCurrentScene() { return current_scene_; }

	/*
	 * Returns the current scene type.
	 * [Input] none
	 * [Output] Current SceneType
	 * [Side effects] none
	 */
	SceneType GetCurrentSceneType() { return scene_type_; }
	SceneGame* GetSceneGame();

private:
	SceneType scene_type_;
	SceneType next_scene_type_;
	Scene* current_scene_;
};
