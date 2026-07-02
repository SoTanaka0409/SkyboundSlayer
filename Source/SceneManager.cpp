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
	,SceneHard(false)//ハードシーンかどうか
	,SceneNormal(false)//ノーマルシーンかどうか
{

}

SceneManager::~SceneManager()
{

}
void SceneManager::Initialize()
{
	////初期シーンの設定

	mnNextSceneType = SCENE_TYPE::SCENE_3D;

	//シーン遷移させる
	ChangeSceneIfNeeded();

}



void SceneManager::Update()
{
	//シーンの更新
	mpCurrentScene->Update();
}

void SceneManager::Draw()
{
	//シーンの描画
	mpCurrentScene->Draw();
}

void SceneManager::Finalize()
{

}

void SceneManager::ChangeSceneIfNeeded()
{
	//現在のシーンと次のシーンが一緒であれば何もしない
	if (mnSceneType == mnNextSceneType)
	{
		return;
	}
	if (mpCurrentScene != nullptr)
	{
		//現在のシーンの終了処理をする
		mpCurrentScene->Finalize();

		//一旦シーン自体も破棄しておく
		delete mpCurrentScene;
		mpCurrentScene = nullptr;

		// シーンが切り替わるときは、以前のシーンに所属していたコライダーを一掃する
		ColliderManager::GetInstance()->DeleteAllCollider();
	}

	//次のシーンにするためシーンタイプを更新
	mnSceneType = mnNextSceneType;

	//mnSceneTypeに応じてシーンを生成する
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
	//シーンの生成がされているはずなので、初期化処理を読んでおく
	mpCurrentScene->Initialize();

}

SceneGame* SceneManager::GetSceneGame()
{
	return dynamic_cast<SceneGame*>(mpCurrentScene);
}
