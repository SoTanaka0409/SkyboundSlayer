#include "SceneManager.h"
#include "Fade.h"
#include "Scene3D.h"
#include "TitleScene.h"
#include "Scene.h"
#include "Rule.h"
#include "SettingsScene.h"
#include "ResultScene.h"
#include "ColliderManager.h"

SceneManager::SceneManager()
	: scene_type_(SceneType::kSceneNone)
	, next_scene_type_(SceneType::kSceneNone)
	, current_scene_(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	next_scene_type_ = SceneType::kSceneTitle;
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	current_scene_->Update();
	Fade::GetInstance()->Update();
}

void SceneManager::Draw()
{
	current_scene_->Draw();
	Fade::GetInstance()->Draw();
}

void SceneManager::Finalize()
{
}

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
	case SceneType::kSceneSettings:
		current_scene_ = new SettingsScene();
		break;
	case SceneType::kScene3D:
		current_scene_ = new Scene3D();
		break;
	case SceneType::kSceneResultScene:
		current_scene_ = new ResultScene();
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

SceneGame* SceneManager::GetSceneGame()
{
	return dynamic_cast<SceneGame*>(current_scene_);
}