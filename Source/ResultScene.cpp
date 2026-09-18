#include "ResultScene.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Stage.h"
#include "StageObject.h"
#include "SkyBox.h"
#include "Config.h"
#include "Player3D.h"
#include <fstream>
#include <sstream>

namespace
{
	const int PanelX = 72;  // UIレイアウトの基準X座標
	const int PanelY = 82;  // UIレイアウトの基準Y座標
	const int PanelW = 560; // リザルトパネルの幅（デザイン仕様固定）
	const int PanelH = 850; // リザルトパネルの高さ（デザイン仕様固定）

/// @param アニメーション進行度t (0.0~1.0想定)
/// @return イージング補間後の進行度
/// @details 進行度が1.0を超えた場合、意図しない描画座標の計算を防ぐため1.0にクランプする
	float EaseOutCubic(float t)
	{
		if (t > 1.0f)
		{
			t = 1.0f;
		}
		return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
	}

/// @details 背景用3Dモデルおよび配置データ(CSV)の動的メモリ確保と読み込み
	void LoadResultStage()
	{
		new Stage(VGet(0.0f, 5000.0f, -20000.0f),
			"Resource/model/bg/island/01_island.mv1",
			"Resource/model/bg/island/01_island.mv1",
			VGet(200.0f, 100.0f, 200.0f));
		new Stage(Config::GetStageCenter(),
			"Resource/model/stage/normal/01_normal_stage.mv1",
			"Resource/model/stage/normal/02_normal_collider.mv1",
			VGet(3.0f, 0.3f, 3.0f));

		std::ifstream file(L"Resource/data/CSV/01_stage_layout.csv");
		if (file.is_open())
		{
			std::string line;
			std::getline(file, line); // ヘッダー行をスキップしてデータ不整合を回避
			while (std::getline(file, line))
			{
				if (line.empty())
				{
					continue; // 空行パース時のクラッシュを防止
				}

				std::stringstream ss(line);
				std::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr;
				std::string texture, colSizeStr, isRelativeStr;
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

				VECTOR pos = VGet(std::stof(xStr), std::stof(yStr), std::stof(zStr));
				if (!isRelativeStr.empty() && std::stoi(isRelativeStr) == 1)
				{
					pos = VAdd(Config::GetStageCenter(), pos);
				}
				VECTOR scale = VGet(std::stof(sxStr), std::stof(syStr), std::stof(szStr));

				if (type == "StageObject")
				{
					float colSize = colSizeStr.empty() ? 0.0f : std::stof(colSizeStr);
					new StageObject(pos, model, scale, "", colSize);
				}
				else if (type == "Stage")
				{
					new Stage(pos, model, model, scale, texture);
				}
			}
		}

		SkyBox* skyBox = new SkyBox("Resource/model/bg/sky/01_sky_dome_model.x", VGet(0.0f, 0.0f, -5000.0f));
		skyBox->SetScale(VGet(13.0f, 13.0f, 13.0f));
		skyBox->SetModelTexture("Resource/model/bg/sky/02_sky_texture.jpg");
	}

/// @details カメラ座標と注視点をリザルト演出専用の固定座標へ強制上書き
	void SetResultCamera()
	{
		const VECTOR stageCenter = Config::GetStageCenter();
		const VECTOR cameraPos = VAdd(stageCenter, VGet(6500.0f, 8500.0f, 7000.0f));
		const VECTOR cameraTarget = VAdd(stageCenter, VGet(0.0f, 200.0f, 0.0f));
		SetCameraPositionAndTarget_UpVecY(cameraPos, cameraTarget);
	}
}

/// @details 演出進行用タイマーの初期化
ResultScene::ResultScene()
	: all_timer_(0)
{
}

ResultScene::~ResultScene()
{
}

/// @details スコア保存処理の実行と、リザルト専用BGM・環境光・カメラの適用
void ResultScene::Initialize()
{
	Master::sound_manager_->PlayBGM(SoundManager::BGM_RESULT);
	Master::score_manager_->SaveHighScore(); // アプリ強制終了時にスコアが失われるのを防ぐため早期保存

	LoadResultStage();
	SetLightEnable(TRUE);
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));

	SetResultCamera();
	SetCameraNearFar(100.0f, Config::CameraFar);
}

/// @details アニメーションタイマーの加算と画面遷移入力の監視
void ResultScene::Update()
{
	all_timer_++;
	SetResultCamera();
	Scene::Update();
	HandleReturnInput();
}

/// @details プレイヤー入力を検知し、タイトル画面へのシーン遷移を要求する
void ResultScene::HandleReturnInput()
{
	// 演出のスキップによるUX低下や、ロード直後の誤操作を防ぐため入力を一定時間ブロック
	if (all_timer_ < 100)
	{
		return;
	}

	if (InputManager::CheckMouseClickLeft() || InputManager::CheckDownKey(KEY_INPUT_BACK))
	{
		Master::scene_manager_->SetNextScene(SceneManager::kSceneTitle);
	}
}

/// @details 3Dシーンと、Zバッファ書き込み制御によるUIレイヤーの合成描画
void ResultScene::Draw()
{
	SetWriteZBufferFlag(FALSE); // 3DモデルにUIが遮蔽されないようにZバッファ更新を一時停止
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(15, 18, 25), TRUE);
	SetWriteZBufferFlag(TRUE);

	Scene::Draw();

	int size = GetFontSize(); // 描画後にフォントサイズを復元するため退避
	DrawResultPanel();
	DrawResultHeader();
	DrawResultStats();
	DrawResultFooter();
	SetFontSize(size);
}

/// @details 透過ブレンド設定を変更し、UIのベースパネルを描画
void ResultScene::DrawResultPanel()
{
	const int accentGold = GetColor(218, 178, 86);     // アクセントカラー（明）
	const int accentGoldDark = GetColor(98, 73, 32);   // アクセントカラー（暗）
	const int mainPanel = GetColor(30, 35, 45);        // パネル背景色
	const int panelLight = GetColor(47, 52, 65);       // パネルハイライト色

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(PanelX - 18, PanelY - 18, PanelX + PanelW + 18, PanelY + PanelH + 18, GetColor(0, 0, 0), TRUE);
	DrawBox(PanelX, PanelY, PanelX + PanelW, PanelY + PanelH, mainPanel, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawBox(PanelX + 16, PanelY + 16, PanelX + PanelW - 16, PanelY + 28, panelLight, TRUE);
	DrawLine(PanelX, PanelY, PanelX + PanelW, PanelY, accentGold, 2);
	DrawLine(PanelX, PanelY + PanelH, PanelX + PanelW, PanelY + PanelH, accentGoldDark, 2);
	DrawLine(PanelX, PanelY, PanelX, PanelY + PanelH, accentGoldDark, 2);
	DrawLine(PanelX + PanelW, PanelY, PanelX + PanelW, PanelY + PanelH, accentGold, 2);
}

/// @details タイマーに依存したイージング計算によるヘッダーテキストの描画
void ResultScene::DrawResultHeader()
{
	float ease1 = EaseOutCubic(all_timer_ / 30.0f); // 30フレームかけて完了するスライドイン補間
	int titleX = (int)(PanelX - 620 + (620 * ease1));

	const bool isWin = Master::score_manager_ == nullptr || Master::score_manager_->IsResultVictory();
	const char* title = isWin ? "QUEST CLEARED" : "QUEST FAILED";
	const char* subtitle = isWin ? "FLOATING STAGE SECURED" : "QUEST FAILED";

	SetFontSize(62);
	DrawFormatString(titleX + 5, PanelY + 62 + 5, GetColor(42, 30, 0), title);
	DrawFormatString(titleX, PanelY + 62, GetColor(255, 239, 178), title);
	SetFontSize(22);
	DrawFormatString(PanelX + 34, PanelY + 142, GetColor(190, 210, 230), subtitle);
}

/// @details スコアマネージャーから実績値を取得し、スライドイン演出付きで描画
void ResultScene::DrawResultStats()
{
	// 段階的な演出仕様を満たすため、ヘッダー描画完了(30フレーム)まで処理を待機
	if (all_timer_ < 30)
	{
		return;
	}

	float ease2 = EaseOutCubic((all_timer_ - 30) / 28.0f);
	int contentX = (int)(PanelX - 620 + (620 * ease2));

	SetFontSize(28);
	DrawFormatString(contentX + 34, PanelY + 218, GetColor(255, 225, 160), "BATTLE RESULT");
	SetFontSize(30);
	DrawFormatString(contentX + 54, PanelY + 270, GetColor(236, 239, 242), "Defeated Enemies");
	DrawFormatString(contentX + 390, PanelY + 270, GetColor(255, 255, 255), "%3d", Master::score_manager_->GetDefeatedEnemies());
	DrawFormatString(contentX + 54, PanelY + 326, GetColor(236, 239, 242), "Potions Used");
	DrawFormatString(contentX + 390, PanelY + 326, GetColor(255, 255, 255), "%3d", Master::score_manager_->GetUsedPotions());

	DrawLine(PanelX + 34, PanelY + 410, PanelX + PanelW - 34, PanelY + 410, GetColor(86, 72, 45), 1);
	SetFontSize(28);
	DrawFormatString(contentX + 34, PanelY + 458, GetColor(255, 190, 190), "FINAL STATUS");
	SetFontSize(30);
	DrawFormatString(contentX + 54, PanelY + 512, GetColor(180, 255, 178), "Max HP");
	DrawFormatString(contentX + 390, PanelY + 512, GetColor(255, 255, 255), "%3.0f", Master::score_manager_->GetFinalHp());
	DrawFormatString(contentX + 54, PanelY + 568, GetColor(255, 150, 150), "Attack");
	DrawFormatString(contentX + 390, PanelY + 568, GetColor(255, 255, 255), "%3.0f", Master::score_manager_->GetFinalAttack());
	DrawFormatString(contentX + 54, PanelY + 624, GetColor(158, 214, 255), "Speed");
	DrawFormatString(contentX + 390, PanelY + 624, GetColor(255, 255, 255), "%3.0f", Master::score_manager_->GetFinalSpeed());
}

/// @details プレイヤーに画面遷移の操作が可能であることを示す、透過度計算を用いた点滅UIの描画
void ResultScene::DrawResultFooter()
{
/// @param ブロック期間中はナビゲーションUIを非表示にする
	if (all_timer_ < 100)
	{
		return;
	}

	const int gold = GetColor(218, 178, 86);
	int alpha = (all_timer_ % 60 < 30) ? 255 : 105; // 60フレーム周期の明滅アニメーション
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	SetFontSize(30);
	DrawBox(PanelX + 74, PanelY + 744, PanelX + PanelW - 74, PanelY + 806, GetColor(38, 34, 26), TRUE);
	DrawLine(PanelX + 74, PanelY + 744, PanelX + PanelW - 74, PanelY + 744, gold, 1);
	DrawFormatString(PanelX + 148, PanelY + 762, GetColor(255, 255, 255), "Tap / Click to Return");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ResultScene::Finalize()
{
	Master::sound_manager_->StopBGM();
}
