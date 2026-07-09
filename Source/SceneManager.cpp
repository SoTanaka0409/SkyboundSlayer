#include"SceneManager.h"
#include"Scene3D.h"
#include "TestCollisionScene.h"
#include"TitleScene.h"
#include"Scene.h"
//#include"TitleScene.h"
//#include"GameScene.h"
#include"ResultScene.h"
#include"Rule.h"
#include"OperationScene.h"
#include"ResultWin.h"
#include"NameScene.h"
#include"TutorialScene.h"
#include"ResetScene.h"
#include"ColliderManager.h"


SceneManager::SceneManager()
	:mnSceneType(SCENE_TYPE::SCENE_NONE)
	, mnNextSceneType(SCENE_TYPE::SCENE_NONE)
	, mpCurrentScene(nullptr)
	,SceneHard(false)//繝上・繝峨す繝ｼ繝ｳ縺九←縺・°
	,SceneNormal(false)//繝弱・繝槭Ν繧ｷ繝ｼ繝ｳ縺九←縺・°
{

}

SceneManager::~SceneManager()
{

}
void SceneManager::Initialize()
{
	////蛻晄悄繧ｷ繝ｼ繝ｳ縺ｮ險ｭ螳・

	mnNextSceneType = SCENE_TYPE::SCENE_3D;

	//繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ縺輔○繧・
	ChangeSceneIfNeeded();

}



void SceneManager::Update()
{
	//繧ｷ繝ｼ繝ｳ縺ｮ譖ｴ譁ｰ
	mpCurrentScene->Update();
}

void SceneManager::Draw()
{
	//繧ｷ繝ｼ繝ｳ縺ｮ謠冗判
	mpCurrentScene->Draw();
}

void SceneManager::Finalize()
{

}

void SceneManager::ChangeSceneIfNeeded()
{
	//迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｨ谺｡縺ｮ繧ｷ繝ｼ繝ｳ縺御ｸ邱偵〒縺ゅｌ縺ｰ菴輔ｂ縺励↑縺・
	if (mnSceneType == mnNextSceneType)
	{
		return;
	}
	if (mpCurrentScene != nullptr)
	{
		//迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ邨ゆｺ・・逅・ｒ縺吶ｋ
		mpCurrentScene->Finalize();

		//荳譌ｦ繧ｷ繝ｼ繝ｳ閾ｪ菴薙ｂ遐ｴ譽・＠縺ｦ縺翫￥
		delete mpCurrentScene;
		mpCurrentScene = nullptr;

		// 繧ｷ繝ｼ繝ｳ縺悟・繧頑崛繧上ｋ縺ｨ縺阪・縲∽ｻ･蜑阪・繧ｷ繝ｼ繝ｳ縺ｫ謇螻槭＠縺ｦ縺・◆繧ｳ繝ｩ繧､繝繝ｼ繧剃ｸ謗・☆繧・
		ColliderManager::GetInstance()->DeleteAllCollider();
	}

	//谺｡縺ｮ繧ｷ繝ｼ繝ｳ縺ｫ縺吶ｋ縺溘ａ繧ｷ繝ｼ繝ｳ繧ｿ繧､繝励ｒ譖ｴ譁ｰ
	mnSceneType = mnNextSceneType;

	//mnSceneType縺ｫ蠢懊§縺ｦ繧ｷ繝ｼ繝ｳ繧堤函謌舌☆繧・
	switch (mnSceneType)
	{
	case SCENE_TYPE::SCENE_TEST_COLLISION:
		mpCurrentScene = new TestCollisionScene();
		break;
	case SCENE_TYPE::SCENE_TITLE:
		mpCurrentScene = new TitleScene();
		break;
	/*case SCENE_TYPE::SCENE_GAME:
		mpCurrentScene = new GameScene(0);
		break;*/
	case SCENE_TYPE::SCENE_RULE:
		mpCurrentScene = new Rule();
		break;
	case SCENE_TYPE::SCENE_RESULT:
		mpCurrentScene = new ResultScene();
		break;
	case SCENE_TYPE::SCENE_3D:
		mpCurrentScene = new Scene3D();
		break;
	case SCENE_TYPE::SCENE_TUTORIAL:
		mpCurrentScene = new TutorialScene();
		break;
	case SCENE_TYPE::SCENE_OPERATION:
		mpCurrentScene = new  OperationScene();
		break;
	case SCENE_TYPE::SCENE_RESULTWIN:
		mpCurrentScene = new ResultWin();
		break;
	case SCENE_TYPE::SCENE_NAME:
		mpCurrentScene = new NameScene();	
		break;
	case SCENE_TYPE::SCENE_RESET:
		mpCurrentScene = new ResetScene();
		break;
	//default:
		
	}
	//繧ｷ繝ｼ繝ｳ縺ｮ逕滓・縺後＆繧後※縺・ｋ縺ｯ縺壹↑縺ｮ縺ｧ縲∝・譛溷喧蜃ｦ逅・ｒ隱ｭ繧薙〒縺翫￥
	mpCurrentScene->Initialize();

}

SceneGame* SceneManager::GetSceneGame()
{
	return dynamic_cast<SceneGame*>(mpCurrentScene);
}
