#include "SceneManager.h"
#include "Fade.h"
#include "Scene3D.h"
#include "TitleScene.h"
#include "Scene.h"
#include "Rule.h"
#include "SettingsScene.h"
#include "ResultScene.h"
#include "ColliderManager.h"

/// @brief SceneManagerのコンストラクタ
/// @details 各種メンバ変数およびシーンタイプの初期化を行う
SceneManager::SceneManager()
	: scene_type_(SceneType::kSceneNone)
	, next_scene_type_(SceneType::kSceneNone)
	, current_scene_(nullptr)
{
}

/// @brief SceneManagerのデストラクタ
/// @details リソース解放などの終了処理を行う
SceneManager::~SceneManager()
{
}

/// @brief シーンマネージャーの初期化処理
/// @details 初期シーン（タイトル画面）を設定し、最初のシーン生成と初期化を実行する
void SceneManager::Initialize()
{
	next_scene_type_ = SceneType::kSceneTitle;
	ChangeSceneIfNeeded();
}

/// @brief シーンマネージャーの状態更新処理
/// @details 現在アクティブなシーンおよびフェード処理の更新を毎フレーム実行する
void SceneManager::Update()
{
	current_scene_->Update();
	Fade::GetInstance()->Update();
}

/// @brief シーンマネージャーの描画処理
/// @details 現在アクティブなシーンおよび画面上のフェード効果を描画する
void SceneManager::Draw()
{
	current_scene_->Draw();
	Fade::GetInstance()->Draw();
}

/// @brief シーンマネージャーの終了処理
/// @details アプリケーション終了時などに呼ばれる最終的な解放処理を行う
void SceneManager::Finalize()
{
}

/// @brief 必要に応じてシーンの切り替え・生成・破棄を行う処理
/// @details フェードアウト・フェードインを制御し、古いシーンの破棄と新しいシーンの生成・初期化を行う
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

/// @brief 現在のシーンをSceneGame型へキャストして取得する
/// @return SceneGame* ゲームシーンへのポインタ（対象でない場合はnullptr）
/// @details 現在のシーンがゲーム本編であればポインタを返し、それ以外ではnullptrを返す
SceneGame* SceneManager::GetSceneGame()
{
	return dynamic_cast<SceneGame*>(current_scene_);
}