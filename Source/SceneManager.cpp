#include "SceneManager.h"
#include "Fade.h"
#include "Scene3D.h"
#include "TitleScene.h"
#include "Scene.h"
#include "Rule.h"
#include "ResultWin.h"
#include "ColliderManager.h"

#include"ResultWin.h"
/*
 * 目的（SceneManagerのコンストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] 各種変数の初期化
 */
SceneManager::SceneManager()
	: scene_type_(SceneType::kSceneNone)
	, next_scene_type_(SceneType::kSceneNone)
	, current_scene_(nullptr)
{
}


/*
 * 目的（SceneManagerのデストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] 現在のシーンのメモリ解放
 */
SceneManager::~SceneManager()
{
}


/*
 * 目的（シーン管理の初期化を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] 初期シーン（タイトル）の設定とメモリ確保
 */
void SceneManager::Initialize()
{
	next_scene_type_ = SceneType::kSceneTitle;
	ChangeSceneIfNeeded();
}


/*
 * 目的（現在のシーンの更新処理を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] current_scene_のUpdate呼び出し
 */
void SceneManager::Update()
{
	current_scene_->Update();
	Fade::GetInstance()->Update();
}


/*
 * 目的（現在のシーンの描画処理を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] current_scene_のDraw呼び出し
 */
void SceneManager::Draw()
{
	current_scene_->Draw();
	Fade::GetInstance()->Draw();
}


/*
 * 目的（現在のシーンの終了処理を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] current_scene_のFinalize呼び出し
 */
void SceneManager::Finalize()
{
}


/*
 * 目的（次のシーンが予約されていればシーン切り替えを行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] 前のシーンの破棄、コライダーの全削除、新しいシーンの生成と初期化
 */
void SceneManager::ChangeSceneIfNeeded()
{
	if (scene_type_ == next_scene_type_)
	{
		return;
	}

	if (scene_type_ != SceneType::kSceneNone)
	{
		if (!Fade::GetInstance()->IsFading())
		{
			Fade::GetInstance()->StartFadeOut();
			return;
		}

		if (Fade::GetInstance()->GetState() == Fade::State::FadeOut && !Fade::GetInstance()->IsFadeOutFinished())
		{
			return;
		}
	}

	if (current_scene_ != nullptr)
	{
		current_scene_->Finalize();
		delete current_scene_;
		current_scene_ = nullptr;
		ColliderManager::GetInstance()->DeleteAllCollider();
	}

	scene_type_ = next_scene_type_;

	switch (scene_type_)
	{
	case SceneType::kSceneTitle:
		current_scene_ = new TitleScene();
		break;
	case SceneType::kSceneRule:
		current_scene_ = new Rule();
		break;
	case SceneType::kScene3D:
		current_scene_ = new Scene3D();
		break;
	case SceneType::kSceneResultWin:
		current_scene_ = new ResultWin();
		break;
	default:
		current_scene_ = new TitleScene();
		scene_type_ = SceneType::kSceneTitle;
		break;
	}

	current_scene_->Initialize();

	if (scene_type_ != SceneType::kSceneNone)
	{
		Fade::GetInstance()->StartFadeIn();
	}
}

SceneGame* 
/*
 * 目的（現在のシーンがゲームシーンであればポインタを取得するため）
 * [入力] なし
 * [出力] SceneGame*: ゲームシーンのポインタ（違う場合はnullptr）
 * [副作用] なし
 */
SceneManager::GetSceneGame()
{
	return dynamic_cast<SceneGame*>(current_scene_);
}
