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



/*
 * 目的（TitleSceneのTitleScene処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
TitleScene::TitleScene()
: color_fade_(1), color_flag_(false), camera_angle_(0.0f)
{
}

TitleScene::~TitleScene()
{
}



/*
 * 目的（TitleSceneのInitialize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::Initialize()
{
	Master::camera_->Initialize();
	Master::game_clear_count_ = 0;
	ColliderManager::GetInstance()->DeleteAllCollider();
	
	Master::score_manager_->LoadHighScore();
	Master::sound_manager_->PlayBGM(SoundManager::BGM_TITLE);
	
	// 譛ｬ邱ｨ縺ｨ蜷後§繧ｹ繝・・繧ｸ縺ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ隱ｭ縺ｿ霎ｼ繧
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1",VGet(3.0f,0.3f,3.0f));
	
	std::ifstream file("Resource/CSV/stage_objects.csv");
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line); // 繝倥ャ繝繝ｼ繧偵せ繧ｭ繝・・
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
	
	// 繧ｿ繧､繝医Ν逕ｨ縺ｫ繝・Ξ繝昴・繧ｿ繝ｼ繧ゆｸ縺､鄂ｮ縺・※縺翫￥
	new StageObject(VGet(0.0f, 0.0f, 500.0f), "Resource/3D/portal/source/portal.mv1", VGet(3.0f, 3.0f, 3.0f));
	
	camera_angle_ = 0.0f;
}



/*
 * 目的（TitleSceneのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::Update()
{
    Scene::Update();
    UpdateTitleCamera();
    HandleMenuInput();
}



/*
 * 目的（TitleSceneのUpdateTitleCamera処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::UpdateTitleCamera()
{
    camera_angle_ += 0.002f;
    if (camera_angle_ >= DX_PI_F * 2.0f)
    {
        camera_angle_ -= DX_PI_F * 2.0f;
    }

    VECTOR camPos = VGet(cosf(camera_angle_) * 3000.0f, 2000.0f, sinf(camera_angle_) * 3000.0f);
    VECTOR camTarget = VGet(0.0f, 1000.0f, 0.0f);
    SetCameraPositionAndTarget_UpVecY(camPos, camTarget);
}



/*
 * 目的（TitleSceneのHandleMenuInput処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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
        Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
        Master::scene_manager_->SetNextScene(SceneManager::kScene3D);
    }
    else if (IsHoverRule(mx, my))
    {
        Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
        Master::scene_manager_->SetNextScene(SceneManager::kSceneRule);
    }
}



/*
 * 目的（TitleSceneのIsHoverStart処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
bool TitleScene::IsHoverStart(int mx, int my) const
{
    return mx >= 96 && mx <= 416 && my >= 732 && my <= 794;
}



/*
 * 目的（TitleSceneのIsHoverRule処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
bool TitleScene::IsHoverRule(int mx, int my) const
{
    return mx >= 96 && mx <= 416 && my >= 812 && my <= 874;
}



/*
 * 目的（TitleSceneのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::Draw()
{
    UpdatePromptBlink();
    DrawSceneBackground();
    DrawTitlePanel();
    DrawMenuPanel();
    DrawPrompt();
}



/*
 * 目的（TitleSceneのUpdatePromptBlink処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::UpdatePromptBlink()
{
    if (color_flag_)
    {
        color_fade_ -= 4;
        if (color_fade_ <= 0)
        {
            color_fade_ = 0;
            color_flag_ = false;
        }
    }
    else
    {
        color_fade_ += 4;
        if (color_fade_ >= 255)
        {
            color_fade_ = 255;
            color_flag_ = true;
        }
    }
}



/*
 * 目的（TitleSceneのDrawSceneBackground処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::DrawSceneBackground()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    Scene::Draw();

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
    DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(15, 18, 25), TRUE); // Base 60%
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



/*
 * 目的（TitleSceneのDrawTitlePanel処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::DrawTitlePanel()
{
	const int accentGold = GetColor(218, 178, 86); // Accent 10%
	const int accentGoldDark = GetColor(98, 73, 32);
	const int mainPanel = GetColor(30, 35, 45); // Main 30%
	const int panelLight = GetColor(47, 52, 65);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(58, 70, 760, 250, mainPanel, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawLine(70, 70, 760, 70, accentGold, 2);
	DrawLine(70, 250, 760, 250, accentGoldDark, 2);
	DrawLine(70, 70, 70, 250, accentGoldDark, 2);
	DrawLine(760, 70, 760, 250, accentGold, 2);
	DrawBox(84, 88, 746, 98, panelLight, TRUE);

    SetFontSize(76);
    DrawFormatString(99, 119, GetColor(10, 8, 4), "Sky Castle Hunter");
    DrawFormatString(94, 114, GetColor(255, 231, 155), "Sky Castle Hunter");
    SetFontSize(24);
}



/*
 * 目的（TitleSceneのDrawMenuPanel処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::DrawMenuPanel()
{
	int mx, my;
	InputManager::GetMousePos(mx, my);

	bool hoverStart = IsHoverStart(mx, my);
	bool hoverRule = IsHoverRule(mx, my);
	const int accentGold = GetColor(218, 178, 86); // Accent 10%
	const int accentGoldDark = GetColor(98, 73, 32);
	const int mainPanel = GetColor(30, 35, 45); // Main 30%
	const int menuBtn = GetColor(20, 24, 32);
	const int panelLight = GetColor(47, 52, 65);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(70, 714, 448, 910, mainPanel, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawLine(70, 714, 448, 714, accentGold, 1);
	DrawLine(70, 910, 448, 910, accentGoldDark, 1);
	DrawLine(70, 714, 70, 910, accentGoldDark, 1);
	DrawLine(448, 714, 448, 910, accentGold, 1);
	DrawBox(84, 728, 434, 742, panelLight, TRUE);

	SetFontSize(34);
	DrawBox(96, 732, 416, 794, hoverStart ? GetColor(48, 39, 18) : menuBtn, TRUE);
	DrawLine(96, 732, 416, 732, hoverStart ? accentGold : accentGoldDark, 1);
	DrawFormatString(126, 748, hoverStart ? GetColor(255, 246, 184) : GetColor(222, 236, 248), "%sGAME START", hoverStart ? "> " : "  ");

	DrawBox(96, 812, 416, 874, hoverRule ? GetColor(48, 39, 18) : menuBtn, TRUE);
	DrawLine(96, 812, 416, 812, hoverRule ? accentGold : accentGoldDark, 1);
	DrawFormatString(126, 828, hoverRule ? GetColor(255, 246, 184) : GetColor(222, 236, 248), "%sRULE", hoverRule ? "> " : "  ");
}



/*
 * 目的（TitleSceneのDrawPrompt処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::DrawPrompt()
{
    	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155 + color_fade_ / 3);
	DrawBox(Config::ScreenWidth / 2 - 210, Config::ScreenHeight - 76, Config::ScreenWidth / 2 + 210, Config::ScreenHeight - 34, GetColor(30, 35, 45), TRUE); // Main 30%
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    SetFontSize(22);
    DrawFormatString(Config::ScreenWidth / 2 - 156, Config::ScreenHeight - 66, GetColor(214, 220, 224), "CLICK A COMMAND TO BEGIN");
    SetFontSize(24);
}


/*
 * 目的（TitleSceneのFinalize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void TitleScene::Finalize()
{
}

