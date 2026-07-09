#pragma once

class Scene;
class SceneGame;


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
		SCENE_RULE,//繧ｿ繧､繝医Ν
		SCENE_TUTORIAL,
		SCENE_LEVEL,
		SCENE_GAME,    //繧ｲ繝ｼ繝
		SCENE_3DHARD,
		SCENE_RESULT,   //繝ｪ繧ｶ繝ｫ繝・
		SCENE_RESULTWIN,
		SCENE_NORMALRESULTSCENE,//繧ｲ繝ｼ繝繧ｪ繝ｼ繝舌・
		SCENE_3D, SCENE_TEST_COLLISION

	};
public:
	SceneManager();
	
	~SceneManager();


	void Initialize();

	void Draw();

	void Update();

	void Finalize();
	//繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ・亥・繧頑崛縺亥・逅・ｼ峨′蠢・ｦ√↑迥ｶ諷九↑繧蛾・遘ｻ蜃ｦ逅・ｒ縺吶ｋ
	void ChangeSceneIfNeeded();
	//谺｡縺ｫ遘ｻ蜍輔☆繧矩・遘ｻ縺吶ｋ繧ｷ繝ｼ繝ｳ縺ｮ險ｭ螳・
	//note:繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ繧偵＠縺溘＞蝣ｴ蜷医・縲∝ｿ・★縺薙・蜃ｦ逅・ｒ邨檎罰縺励※驕ｷ遘ｻ縺輔○繧・
	void SetNextScene(SCENE_TYPE next) { mnNextSceneType = next; }

	void SetSceneHard(bool Hard) { SceneHard = Hard; }
	bool GetSceneHard() { return SceneHard; }

	void SetSceneNormal(bool Normal) {SceneNormal=Normal; }
	bool GetSceneNormal() { return SceneNormal; }
	

	//迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ蜿門ｾ・
	Scene* GetCurrentScene() { return mpCurrentScene; }
	SCENE_TYPE GetCurrentSceneType() { return mnSceneType; }
	SceneGame* GetSceneGame();

private:
	SCENE_TYPE mnSceneType;     //迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ繧ｿ繧､繝・
	SCENE_TYPE mnNextSceneType;//谺｡縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ繧ｿ繧､繝・
	Scene* mpCurrentScene;    //迴ｾ蝨ｨ繧ｷ繝ｼ繝ｳ縺ｮ繝昴う繝ｳ繧ｿ
	Scene* mp;

	bool SceneHard;
	bool SceneNormal;

};
