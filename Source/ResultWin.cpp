#include "ResultWin.h"
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
	const int PanelX = 72;
	const int PanelY = 82;
	const int PanelW = 560;
	const int PanelH = 850;

	float EaseOutCubic(float t)
	{
		if (t > 1.0f)
		{
			t = 1.0f;
		}
		return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
	}

	void LoadResultStage()
	{
		new Stage(VGet(0.0f, 5000.0f, -20000.0f),
			"Resource/3D/stage_sky/source/Flooting_Stage.mv1",
			"Resource/3D/stage_sky/source/Flooting_Stage.mv1",
			VGet(200.0f, 100.0f, 200.0f));
		new Stage(Config::GetStageCenter(),
			"Resource/3D/Stage/Stage00.mv1",
			"Resource/3D/Stage/Stage00_c.mv1",
			VGet(3.0f, 0.3f, 3.0f));

		std::ifstream file("Resource/CSV/stage_objects.csv");
		if (file.is_open())
		{
			std::string line;
			std::getline(file, line);
			while (std::getline(file, line))
			{
				if (line.empty())
				{
					continue;
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

		SkyBox* skyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x", VGet(0.0f, 0.0f, -5000.0f));
		skyBox->SetScale(VGet(13.0f, 13.0f, 13.0f));
		skyBox->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");
	}

	void SetResultCamera()
	{
		const VECTOR stageCenter = Config::GetStageCenter();
		const VECTOR cameraPos = VAdd(stageCenter, VGet(6500.0f, 8500.0f, 7000.0f));
		const VECTOR cameraTarget = VAdd(stageCenter, VGet(0.0f, 200.0f, 0.0f));
		SetCameraPositionAndTarget_UpVecY(cameraPos, cameraTarget);
	}
}



/*
 * 目的（ResultWinのResultWin処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
ResultWin::ResultWin()
	: all_timer_(0)
{
}

ResultWin::~ResultWin()
{
}



/*
 * 目的（ResultWinのInitialize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::Initialize()
{
	Master::sound_manager_->PlayBGM(SoundManager::BGM_RESULT);
	Master::score_manager_->SaveHighScore();

	LoadResultStage();
	SetLightEnable(TRUE);
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));

	SetResultCamera();
	SetCameraNearFar(100.0f, Config::CameraFar);
}



/*
 * 目的（ResultWinのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::Update()
{
	all_timer_++;
	SetResultCamera();
	Scene::Update();
	HandleReturnInput();
}



/*
 * 目的（ResultWinのHandleReturnInput処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::HandleReturnInput()
{
	if (all_timer_ < 100)
	{
		return;
	}

	if (InputManager::CheckMouseClickLeft() || InputManager::CheckDownKey(KEY_INPUT_BACK))
	{
		Master::scene_manager_->SetNextScene(SceneManager::kSceneTitle);
	}
}



/*
 * 目的（ResultWinのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::Draw()
{
	SetWriteZBufferFlag(FALSE);
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(15, 18, 25), TRUE); // Base 60%
	SetWriteZBufferFlag(TRUE);

	Scene::Draw();

	int size = GetFontSize();
	DrawResultPanel();
	DrawResultHeader();
	DrawResultStats();
	DrawResultFooter();
	SetFontSize(size);
}



/*
 * 目的（ResultWinのDrawResultPanel処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::DrawResultPanel()
{
	const int accentGold = GetColor(218, 178, 86); // Accent 10%
	const int accentGoldDark = GetColor(98, 73, 32);
	const int mainPanel = GetColor(30, 35, 45); // Main 30%
	const int panelLight = GetColor(47, 52, 65);

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



/*
 * 目的（ResultWinのDrawResultHeader処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::DrawResultHeader()
{
	float ease1 = EaseOutCubic(all_timer_ / 30.0f);
	int titleX = (int)(PanelX - 620 + (620 * ease1));

	SetFontSize(62);
	DrawFormatString(titleX + 5, PanelY + 62 + 5, GetColor(42, 30, 0), "QUEST CLEARED");
	DrawFormatString(titleX, PanelY + 62, GetColor(255, 239, 178), "QUEST CLEARED");
	SetFontSize(22);
	DrawFormatString(PanelX + 34, PanelY + 142, GetColor(190, 210, 230), "FLOATING STAGE SECURED");
}



/*
 * 目的（ResultWinのDrawResultStats処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::DrawResultStats()
{
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



/*
 * 目的（ResultWinのDrawResultFooter処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::DrawResultFooter()
{
	if (all_timer_ < 100)
	{
		return;
	}

	const int gold = GetColor(218, 178, 86);
	int alpha = (all_timer_ % 60 < 30) ? 255 : 105;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	SetFontSize(30);
	DrawBox(PanelX + 74, PanelY + 744, PanelX + PanelW - 74, PanelY + 806, GetColor(38, 34, 26), TRUE);
	DrawLine(PanelX + 74, PanelY + 744, PanelX + PanelW - 74, PanelY + 744, gold, 1);
	DrawFormatString(PanelX + 148, PanelY + 762, GetColor(255, 255, 255), "Tap / Click to Return");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



/*
 * 目的（ResultWinのFinalize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void ResultWin::Finalize()
{
}
