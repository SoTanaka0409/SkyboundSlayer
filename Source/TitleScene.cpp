#include "TitleScene.h"
#include "InputManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "ColliderManager.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include "Stage.h"
#include "StageObject.h"
#include "SkyBox.h"
#include "Config.h"

TitleScene::TitleScene()
: mnColorFade(1), mbColorFlag(false), mCameraAngle(0.0f)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Master::mpCamera->Initialize();
	Master::GameClearCount = 0;
	ColliderManager::GetInstance()->DeleteAllCollider();
	
	Master::mpScoreManager->LoadHighScore();
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
	
	// 本編と同じスチE�Eジとオブジェクトを読み込む
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1",VGet(3.0f,0.3f,3.0f));
	
	std::ifstream file("Resource/CSV/stage_objects.csv");
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line); // ヘッダーをスキチE�E
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
			if (isRelative == 1) { pos = VAdd(Config::GetStageCenter(), pos); }
			VECTOR scale = VGet(sx, sy, sz);
			
			if (type == "StageObject") {
				float colSize = 0.0f;
				if (!colSizeStr.empty()) colSize = std::stof(colSizeStr);
				new StageObject(pos, model, scale, "", colSize);
			} else if (type == "Stage") {
				new Stage(pos, model, model, scale, texture);
			}
		}
		file.close();
	}
	
	SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x",VGet(0,0,-5000));
	float scale = 13.0f;
	pSkyBox->SetScale(VGet(scale, scale, scale));
	pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");
	
	// タイトル用にチE��ポ�Eターも一つ置ぁE��おく
	new StageObject(VGet(0.0f, 0.0f, 500.0f), "Resource/3D/portal/source/portal.mv1", VGet(3.0f, 3.0f, 3.0f));
	
	mCameraAngle = 0.0f;
}

void TitleScene::Update()
{
    Scene::Update();
    UpdateTitleCamera();
    HandleMenuInput();
}

void TitleScene::UpdateTitleCamera()
{
    mCameraAngle += 0.002f;
    if (mCameraAngle >= DX_PI_F * 2.0f)
    {
        mCameraAngle -= DX_PI_F * 2.0f;
    }

    VECTOR camPos = VGet(cosf(mCameraAngle) * 3000.0f, 2000.0f, sinf(mCameraAngle) * 3000.0f);
    VECTOR camTarget = VGet(0.0f, 1000.0f, 0.0f);
    SetCameraPositionAndTarget_UpVecY(camPos, camTarget);
}

void TitleScene::HandleMenuInput()
{
    int mx, my;
    InputManager::GetMousePos(mx, my);

    if (!InputManager::CheckMouseClickLeft())
    {
        return;
    }

    if (IsHoverStart(mx, my))
    {
        Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
        Master::mpSceneManager->SetNextScene(SceneManager::kScene3D);
    }
    else if (IsHoverRule(mx, my))
    {
        Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
        Master::mpSceneManager->SetNextScene(SceneManager::kSceneRule);
    }
}

bool TitleScene::IsHoverStart(int mx, int my) const
{
    return mx >= 96 && mx <= 416 && my >= 732 && my <= 794;
}

bool TitleScene::IsHoverRule(int mx, int my) const
{
    return mx >= 96 && mx <= 416 && my >= 812 && my <= 874;
}

void TitleScene::Draw()
{
    UpdatePromptBlink();
    DrawSceneBackground();
    DrawTitlePanel();
    DrawMenuPanel();
    DrawPrompt();
}

void TitleScene::UpdatePromptBlink()
{
    if (mbColorFlag)
    {
        mnColorFade -= 4;
        if (mnColorFade <= 0)
        {
            mnColorFade = 0;
            mbColorFlag = false;
        }
    }
    else
    {
        mnColorFade += 4;
        if (mnColorFade >= 255)
        {
            mnColorFade = 255;
            mbColorFlag = true;
        }
    }
}

void TitleScene::DrawSceneBackground()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    Scene::Draw();

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
    DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::DrawTitlePanel()
{
    const int gold = GetColor(218, 178, 86);
    const int goldDark = GetColor(98, 73, 32);
    const int panelLight = GetColor(42, 43, 48);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 185);
    DrawBox(58, 70, 760, 250, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawLine(70, 70, 760, 70, gold, 2);
    DrawLine(70, 250, 760, 250, goldDark, 2);
    DrawLine(70, 70, 70, 250, goldDark, 2);
    DrawLine(760, 70, 760, 250, gold, 2);
    DrawBox(84, 88, 746, 98, panelLight, TRUE);

    SetFontSize(76);
    DrawFormatString(99, 119, GetColor(10, 8, 4), "Sky Castle Hunter");
    DrawFormatString(94, 114, GetColor(255, 231, 155), "Sky Castle Hunter");
    SetFontSize(24);
    DrawFormatString(102, 205, GetColor(193, 214, 234), "FLOATING STAGE RAID");
}

void TitleScene::DrawMenuPanel()
{
    int mx, my;
    InputManager::GetMousePos(mx, my);

    bool hoverStart = IsHoverStart(mx, my);
    bool hoverRule = IsHoverRule(mx, my);
    const int gold = GetColor(218, 178, 86);
    const int goldDark = GetColor(98, 73, 32);
    const int panel = GetColor(12, 14, 18);
    const int panelLight = GetColor(42, 43, 48);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 185);
    DrawBox(70, 714, 448, 910, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawLine(70, 714, 448, 714, gold, 1);
    DrawLine(70, 910, 448, 910, goldDark, 1);
    DrawLine(70, 714, 70, 910, goldDark, 1);
    DrawLine(448, 714, 448, 910, gold, 1);
    DrawBox(84, 728, 434, 742, panelLight, TRUE);

    SetFontSize(34);
    DrawBox(96, 732, 416, 794, hoverStart ? GetColor(48, 39, 18) : panel, TRUE);
    DrawLine(96, 732, 416, 732, hoverStart ? gold : goldDark, 1);
    DrawFormatString(126, 748, hoverStart ? GetColor(255, 246, 184) : GetColor(222, 236, 248), "%sGAME START", hoverStart ? "> " : "  ");

    DrawBox(96, 812, 416, 874, hoverRule ? GetColor(48, 39, 18) : panel, TRUE);
    DrawLine(96, 812, 416, 812, hoverRule ? gold : goldDark, 1);
    DrawFormatString(126, 828, hoverRule ? GetColor(255, 246, 184) : GetColor(222, 236, 248), "%sRULE", hoverRule ? "> " : "  ");
}

void TitleScene::DrawPrompt()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155 + mnColorFade / 3);
    DrawBox(Config::ScreenWidth / 2 - 210, Config::ScreenHeight - 76, Config::ScreenWidth / 2 + 210, Config::ScreenHeight - 34, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    SetFontSize(22);
    DrawFormatString(Config::ScreenWidth / 2 - 156, Config::ScreenHeight - 66, GetColor(214, 220, 224), "CLICK A COMMAND TO BEGIN");
    SetFontSize(24);
}
void TitleScene::Finalize()
{
}

