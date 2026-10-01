#include <fstream>
#include <sstream>
#include"GameScene.h"
#include"Config.h"
#include"Enemy3D_AT.h"
#include"Player3D.h"
#include"InputManager.h"
#include"Enemy3D.h"
#include"ObjectManager.h"
#include"Stage.h"
#include"Tree.h"
#include"StatShop.h"
#include"Wall.h"
#include"EnemyManager.h"
#include"SkyBox.h"
#include"StageCollider.h"
#include"Rock.h"
#include"StageObject.h"

/// @details 非同期ロード進行用フラグとタイマーの初期化
GameScene::GameScene()
	: m_LoadTimer(500)     // ロード待機フレーム数
	, m_LoadCount(0)       // ロード完了アセット数
	, m_IsLoadFlag(true)  // ロード中判定フラグ
{
}

GameScene::~GameScene()
{
	delete m_PauseMenu;
}

/// @details 3D環境の設定、アクター生成、ステージ情報のパース処理の実行
void GameScene::Initialize()
{
	SceneGame::Initialize();
	m_PauseMenu = new PauseMenu();
	SetupEnvironment();
	CreateInitialActors();
	CreateStage();
	LoadStageObjectsFromCsv();
	CreateSkyBox();
	Master::m_SoundManager->PlayBGM(SoundManager::BGM_GAME);

	SetMouseDispFlag(false);
}

/// @details DXライブラリのフォグ・環境光設定のグローバル変更
void GameScene::SetupEnvironment()
{
	// 空間の奥行きを表現し、遠方のモデル描画の境界を自然に馴染ませるためフォグを設定
	SetFogEnable(TRUE);
	SetFogColor(20, 30, 50);
	SetFogStartEnd(500.0f, 10000.0f);

	SetGlobalAmbientLight(GetColorF(0.15f, 0.15f, 0.2f, 1.0f));

	SetLightDifColor(GetColorF(0.6f, 0.6f, 0.8f, 1.0f));
	SetLightSpcColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));
	SetLightDirection(VNorm(VGet(-0.5f, -0.8f, 0.3f)));
}

/// @details プレイヤーやショップ等の必須アクターオブジェクトのヒープ確保
void GameScene::CreateInitialActors()
{
	new Player3D("Resource/model/character/01_human.mv1", VGet(-1200, 20.0f, -1000), 30.0f, 12.0f, 150.0f, true);
	new StatShop("Resource/model/character/04_shop_clerk_model.mv1", VGet(-1500, 100, 1500));
	new StageObject(VGet(-1250, 20.0f, -1050), "Resource/model/props/campfire/01_campfire_model.mv1", VGet(10.0f, 10.0f, 10.0f));
}

/// @details 地形モデルおよび当たり判定モデルの生成
void GameScene::CreateStage()
{
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/model/bg/island/01_island.mv1", "Resource/model/bg/island/01_island.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/model/stage/normal/01_normal_stage.mv1", "Resource/model/stage/normal/02_normal_collider.mv1", VGet(3.0f, 0.3f, 3.0f));
}

/// @details CSVファイルからのデータ読み込みおよびステージオブジェクトの大量生成
void GameScene::LoadStageObjectsFromCsv()
{
	std::ifstream file(L"Resource/data/CSV/01_stage_layout.csv");
	if (file.is_open())
	{
		std::string line;
		// データ不整合や型変換エラーでのクラッシュを防ぐためヘッダー行を読み飛ばす
		std::getline(file, line);
		while (std::getline(file, line))
		{
			if (line.empty()) continue;
			std::stringstream ss(line);
			std::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr, texture, colSizeStr, isRelativeStr;

			std::getline(ss, type, ',');
			std::getline(ss, model, ',');
			std::getline(ss, xStr, ',');
			std::getline(ss, yStr, ',');
			std::getline(ss, zStr, ',');
			std::getline(ss, sxStr, ',');
			std::getline(ss, syStr, ',');
			std::getline(ss, szStr, ',');
			std::getline(ss, texture, ',');
			std::getline(ss, colSizeStr, ',');
			std::getline(ss, isRelativeStr, ',');

			float x = std::stof(xStr);
			float y = std::stof(yStr);
			float z = std::stof(zStr);
			float sx = std::stof(sxStr);
			float sy = std::stof(syStr);
			float sz = std::stof(szStr);
			int isRelative = 0;
			if (!isRelativeStr.empty()) isRelative = std::stoi(isRelativeStr);

			VECTOR pos = VGet(x, y, z);

			// ステージ中心からの相対座標指定を許可し、後からのステージ全体移動に追従させる
			if (isRelative == 1)
			{
				pos = VAdd(Config::GetStageCenter(), pos);
			}
			VECTOR scale = VGet(sx, sy, sz);

			if (type == "StageObject")
			{
				float colSize = 0.0f;
				if (!colSizeStr.empty()) colSize = std::stof(colSizeStr);
				new StageObject(pos, model, scale, "", colSize);
			}
			else if (type == "Stage")
			{
				new Stage(pos, model, model, scale, texture);
			}
		}
	}
}

/// @details 天球ドームの生成とテクスチャの適用
void GameScene::CreateSkyBox()
{
	SkyBox* pSkyBox = new SkyBox("Resource/model/bg/sky/01_sky_dome_model.x", VGet(0, 0, -5000));
	float scale = 13.0f;
	pSkyBox->SetScale(VGet(scale, scale, scale));
	pSkyBox->SetModelTexture("Resource/model/bg/sky/02_sky_texture.jpg");
}

/// @details ゲーム状態の更新およびリザルト画面への遷移
void GameScene::Update()
{
	if (InputManager::CheckDownKey(KEY_INPUT_P) != 0)
	{
		Master::m_IsPauseOn = !Master::m_IsPauseOn;
		Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
		SetMouseDispFlag(Master::m_IsPauseOn ? TRUE : FALSE);
	}

	if (Master::m_IsPauseOn)
	{
		if (m_PauseMenu) m_PauseMenu->Update();
		return;
	}

	SceneGame::Update();
	Master::m_Save->Update();
	Player3D* player = Master::m_Player;

	if (player != nullptr && player->GetHp() <= 0.0f)
	{
		player->StartDeath();
		if (player->IsDeathAnimationFinished())
		{
			if (Master::m_ScoreManager != nullptr)
			{
				Master::m_ScoreManager->SetResultVictory(false);
				Master::m_ScoreManager->SetFinalStats(0.0f, player->GetAllStatusState(Object3D::Status_Attack), player->GetAllStatusState(Object3D::Status_Speed));
			}
			Master::m_SceneManager->SetNextScene(SceneManager::kSceneResultScene);
		}
		return;
	}

	if (m_GameManager->GetCurrentPhase() == GameManager::Phase::kClear)
	{
		Master::m_GameClearCount = 2;
	}

	if (Master::m_GameClearCount == 2)
	{
		// クリア時の最終ステータスをリザルト画面へ引き継ぐためスコアマネージャーに保存
		if (player != nullptr && Master::m_ScoreManager != nullptr)
		{
			Master::m_ScoreManager->SetResultVictory(true);
			Master::m_ScoreManager->SetFinalStats(player->GetAllStatusState(Object3D::Status_Hp), player->GetAllStatusState(Object3D::Status_Attack), player->GetAllStatusState(Object3D::Status_Speed));
		}
		Master::m_SceneManager->SetNextScene(SceneManager::kSceneResultScene);
	}
}

/// @details 3Dモデル、UI、デバッグ用グリッドの描画
void GameScene::Draw()
{
	SceneGame::Draw();

	// ピンチ演出（HP 30%以下で赤い画面点滅）
	if (Master::m_Player && Master::m_Player->GetHp() > 0 && Master::m_Player->GetHp() <= Master::m_Player->GetMaxHp() * 0.3f)
	{
		int alpha = (int)(60 + sin(GetNowCount() * 0.01f) * 40); // 脈打つ赤いエフェクト
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(200, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	Master::m_Save->Draw();

	if (Master::m_IsPauseOn && m_PauseMenu)
	{
		m_PauseMenu->Draw();
	}

}

/// @details 地面へのグリッド線の描画
void GameScene::DrawDebugGrid()
{
	// マップ作成時やデバッグ時の座標スケール感を目視確認しやすくするための補助線
	const int count = 51;
	const float distance = 500.0f;
	for (int i = 0; i < count; i++)
	{
		float base = (count / 2 - i) * -distance;

		DrawLine3D(
			VGet(-distance * (count / 2), 0.0f, base),
			VGet(distance * (count / 2), 0.0f, base),
			GetColor(255, 255, 255)
		);

		DrawLine3D(
			VGet(base, 0.0f, -distance * (count / 2)),
			VGet(base, 0.0f, distance * (count / 2)),
			GetColor(255, 255, 255)
		);
	}
}

/// @details BGM停止と描画設定の初期化
void GameScene::Finalize()
{
	Master::m_SoundManager->StopBGM();
	SceneGame::Finalize();

	// 次のシーン（UIや2D画面）にフォグ効果が残留して画面が白濁するのを防ぐため明示的にOFFにする
	SetFogEnable(FALSE);
}
