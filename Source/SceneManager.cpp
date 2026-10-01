#include "SceneManager.h"
#include "Fade.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Scene.h"
#include "Rule.h"
#include "SettingsScene.h"
#include "ResultScene.h"
#include "ColliderManager.h"

/// @brief SceneManagerのコンストラクタ
/// @details 各種メンバ変数およびシーンタイプの初期化を行う
SceneManager::SceneManager()
	: m_SceneType(SceneType::kSceneNone)
	, m_NextSceneType(SceneType::kSceneNone)
	, m_CurrentScene(nullptr)
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
	m_NextSceneType = SceneType::kSceneTitle;
	ChangeSceneIfNeeded();
}

/// @brief シーンマネージャーの状態更新処理
/// @details 現在アクティブなシーンおよびフェード処理の更新を毎フレーム実行する
void SceneManager::Update()
{
	m_CurrentScene->Update();
	Fade::GetInstance()->Update();
}

/// @brief シーンマネージャーの描画処理
/// @details 現在アクティブなシーンおよび画面上のフェード効果を描画する
void SceneManager::Draw()
{
	m_CurrentScene->Draw();
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
	if (m_SceneType == m_NextSceneType)
	{
		return;
	}

	if (m_SceneType != SceneType::kSceneNone)
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

	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Finalize();
		delete m_CurrentScene;
		m_CurrentScene = nullptr;

		ColliderManager::GetInstance()->DeleteAllCollider();
	}

	m_SceneType = m_NextSceneType;

	switch (m_SceneType)
	{
	case SceneType::kSceneTitle:
		m_CurrentScene = new TitleScene();
		break;
	case SceneType::kSceneRule:
		m_CurrentScene = new Rule();
		break;
	case SceneType::kSceneSettings:
		m_CurrentScene = new SettingsScene();
		break;
	case SceneType::kGameScene:
		m_CurrentScene = new GameScene();
		break;
	case SceneType::kSceneResultScene:
		m_CurrentScene = new ResultScene();
		break;
	default:
		m_CurrentScene = new TitleScene();
		m_SceneType = SceneType::kSceneTitle;
		break;
	}

	m_CurrentScene->Initialize();

	if (m_SceneType != SceneType::kSceneNone)
	{
		Fade::GetInstance()->StartFadeIn();
	}
}

/// @brief 現在のシーンをSceneGame型へキャストして取得する
/// @return SceneGame* ゲームシーンへのポインタ（対象でない場合はnullptr）
/// @details 現在のシーンがゲーム本編であればポインタを返し、それ以外ではnullptrを返す
SceneGame* SceneManager::GetSceneGame()
{
	return dynamic_cast<SceneGame*>(m_CurrentScene);
}