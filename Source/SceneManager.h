#pragma once

class Scene;

class SceneManager
{
public:

	enum SCENE_TYPE
	{
		SCENE_NONE = 0,
		SCENE_RESET,
		SCENE_NAME,
		SCENE_TITLE,
		SCENE_OPERATION,
		SCENE_RULE,//タイトル
		SCENE_TUTORIAL,
		SCENE_LEVEL,
		SCENE_GAME,    //ゲーム
		SCENE_3DHARD,
		SCENE_RESULT,   //リザルト
		SCENE_RESULTWIN,
		SCENE_NORMALRESULTSCENE,//ゲームオーバー
		SCENE_3D, SCENE_TEST_COLLISION

	};
public:
	SceneManager();
	
	~SceneManager();


	void Initialize();

	void Draw();

	void Update();

	void Finalize();
	//シーン遷移（切り替え処理）が必要な状態なら遷移処理をする
	void ChangeSceneIfNeeded();
	//次に移動する遷移するシーンの設定
	//note:シーン遷移をしたい場合は、必ずこの処理を経由して遷移させる
	void SetNextScene(SCENE_TYPE next) { mnNextSceneType = next; }

	void SetSceneHard(bool Hard) { SceneHard = Hard; }
	bool GetSceneHard() { return SceneHard; }

	void SetSceneNormal(bool Normal) {SceneNormal=Normal; }
	bool GetSceneNormal() { return SceneNormal; }
	

	//現在のシーンの取得
	Scene* GetCurrentScene() { return mpCurrentScene; }

private:
	SCENE_TYPE mnSceneType;     //現在のシーンのタイプ
	SCENE_TYPE mnNextSceneType;//次のシーンのタイプ
	Scene* mpCurrentScene;    //現在シーンのポインタ
	Scene* mp;

	bool SceneHard;
	bool SceneNormal;

};
