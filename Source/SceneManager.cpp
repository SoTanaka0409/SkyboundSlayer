#include "SceneManager.h"
#include "Fade.h"
#include "Scene3D.h"
#include "TitleScene.h"
#include "Scene.h"
#include "Rule.h"
#include "SettingsScene.h"
#include "ResultWin.h"
#include "ColliderManager.h"

// 入力：なし
// 出力：なし
// 副作用：シーン状態（現在のシーン、次フレームで移行予定のシーン）を示す列挙型とポインタの初期化
SceneManager::SceneManager()
	: scene_type_(SceneType::kSceneNone)
	, next_scene_type_(SceneType::kSceneNone)
	, current_scene_(nullptr)
{
}

// 入力：なし
// 出力：なし
// 副作用：なし
SceneManager::~SceneManager()
{
}

// 入力：なし
// 出力：なし
// 副作用：起動時の初期シーンをタイトル画面（kSceneTitle）に設定し、最初のシーン生成トリガーを発火させる
void SceneManager::Initialize()
{
	next_scene_type_ = SceneType::kSceneTitle;
	ChangeSceneIfNeeded();
}

// 入力：なし
// 出力：なし
// 副作用：現在アクティブなシーンのロジック更新、および画面遷移用フェードアウト/イン演出の進行
void SceneManager::Update()
{
	current_scene_->Update();
	Fade::GetInstance()->Update();
}

// 入力：なし
// 出力：なし
// 副作用：現在アクティブなシーンの描画コマンド発行、および最前面へのフェードエフェクトのオーバーレイ描画
void SceneManager::Draw()
{
	current_scene_->Draw();
	Fade::GetInstance()->Draw();
}

// 入力：なし
// 出力：なし
// 副作用：なし
void SceneManager::Finalize()
{
}

// 入力：なし
// 出力：なし
// 副作用：フェードアウト完了の待機、古いシーンの破棄、全コライダーの強制リセット、および次シーンの動的確保・初期化処理（フェードイン開始）
void SceneManager::ChangeSceneIfNeeded()
{
	if (scene_type_ == next_scene_type_)
	{
		return;
	}

	// アーキテクチャ設計：シーン切り替え要求が来た際、即座にポインタをすげ替えるのではなく、まずフェードアウト演出を挟む。
	// 完全に画面が暗転（FadeOutFinished）してから初めて古いシーンを破棄し、新しいシーンを構築することで、ロード中の描画のチラつきや不整合を完全に隠蔽するステートマシン構造。
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

		// メモリ管理：シーンに紐づく物理コライダーの残骸が次シーンに干渉・メモリリークするのを防ぐための強制クリーンアップ
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

// 入力：なし
// 出力：SceneGameクラスへのポインタ（現在のシーンがゲーム本編系でない場合はnullptrを返す）
// 副作用：なし
// アーキテクチャ設計：現在の汎用的な `Scene` 基底ポインタから、ゲーム本編（SceneGame / Scene3D）特有のメソッドやマネージャーに安全にアクセスするための動的ダウンキャスト（dynamic_cast）ブリッジ。
SceneGame* SceneManager::GetSceneGame()
{
	return dynamic_cast<SceneGame*>(current_scene_);
}