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

/// @details UI轤ｹ貊・い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ逕ｨ縺ｮ繝代Λ繝｡繝ｼ繧ｿ蛻晄悄蛹・
TitleScene::TitleScene()
	: color_fade_(1), color_flag_(false), camera_angle_(0.0f)
{
}

TitleScene::~TitleScene()
{
}

/// @details 繧ｫ繝｡繝ｩ繝ｻ騾ｲ陦悟ｺｦ繝ｻ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蛻晄悄蛹悶√♀繧医・繧ｿ繧､繝医Ν蟆ら畑縺ｮ3D閭梧勹縺ｮ逕滓・
void TitleScene::Initialize()
{
	Master::camera_->Initialize();
	// 蜻ｨ蝗槭・繝ｬ繧､譎ゅ・荳榊・蜷医ｒ髦ｲ縺舌◆繧√√ち繧､繝医Ν縺ｫ謌ｻ縺｣縺滓凾轤ｹ縺ｧ繧ｯ繝ｪ繧｢蝗樊焚縺ｨ繧ｳ繝ｩ繧､繝繝ｼ諠・ｱ繧偵Μ繧ｻ繝・ヨ縺吶ｋ
	Master::game_clear_count_ = 0;
	ColliderManager::GetInstance()->DeleteAllCollider();

	Master::score_manager_->LoadHighScore();
	Master::sound_manager_->PlayBGM(SoundManager::BGM_TITLE);

	// 荳譫夂ｵｵ縺ｧ縺ｯ縺ｪ縺上∝ｮ滄圀縺ｮ繧ｲ繝ｼ繝繝励Ξ繧､縺ｨ蜷後§3D繝｢繝・Ν繧帝・鄂ｮ縺励※繧ｫ繝｡繝ｩ繧貞屓縺吶％縺ｨ縺ｧ繧ｷ繝ｼ繝繝ｬ繧ｹ縺ｪ荳也阜隕ｳ繧呈ｼ泌・縺吶ｋ
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D繝｢繝・Ν/閭梧勹/豬ｮ驕雁ｳｶ/01_豬ｮ驕雁ｳｶ繝｢繝・Ν.mv1", "Resource/3D繝｢繝・Ν/閭梧勹/豬ｮ驕雁ｳｶ/01_豬ｮ驕雁ｳｶ繝｢繝・Ν.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/3D繝｢繝・Ν/繧ｹ繝・・繧ｸ/騾壼ｸｸ繧ｹ繝・・繧ｸ/01_騾壼ｸｸ繧ｹ繝・・繧ｸ繝｢繝・Ν.mv1", "Resource/3D繝｢繝・Ν/繧ｹ繝・・繧ｸ/騾壼ｸｸ繧ｹ繝・・繧ｸ/02_騾壼ｸｸ繧ｹ繝・・繧ｸ蠖薙◆繧雁愛螳壹Δ繝・Ν.mv1", VGet(3.0f, 0.3f, 3.0f));

	// 驟咲ｽｮ繝・・繧ｿ縺ｮ繝上・繝峨さ繝ｼ繝・ぅ繝ｳ繧ｰ繧帝∩縺代，SV縺九ｉ隱ｭ縺ｿ霎ｼ繧縺薙→縺ｧ繝励Λ繝ｳ繝翫・縺ｮ隱ｿ謨ｴ蟾･謨ｰ繧貞炎貂帙☆繧・
	std::ifstream file(L"Resource/繝・・繧ｿ/CSV/01_繧ｹ繝・・繧ｸ驟咲ｽｮ繝・・繧ｿ.csv");
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line); // 繝倥ャ繝繝ｼ陦後ｒ繧ｹ繧ｭ繝・・縺励※繝代・繧ｹ繧ｨ繝ｩ繝ｼ繧貞屓驕ｿ
		while (std::getline(file, line))
		{
			if (line.empty()) continue; // 遨ｺ陦後↓繧医ｋ繧ｯ繝ｩ繝・す繝･繧帝亟豁｢
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
			}
			else if (type == "Stage") {
				new Stage(pos, model, model, scale, texture);
			}
		}
		file.close();
	}

	SkyBox* pSkyBox = new SkyBox("Resource/3D繝｢繝・Ν/閭梧勹/遨ｺ/01_遨ｺ繝峨・繝繝｢繝・Ν.x", VGet(0, 0, -5000));
	float scale = 13.0f;
	pSkyBox->SetScale(VGet(scale, scale, scale));
	pSkyBox->SetModelTexture("Resource/3D繝｢繝・Ν/閭梧勹/遨ｺ/02_遨ｺ繝・け繧ｹ繝√Ε.jpg");

	new StageObject(VGet(0.0f, 0.0f, 500.0f), "Resource/3D繝｢繝・Ν/蟆冗黄/繝昴・繧ｿ繝ｫ/01_繝昴・繧ｿ繝ｫ繝｢繝・Ν.mv1", VGet(3.0f, 3.0f, 3.0f));
	camera_angle_ = 0.0f;
}

/// @details 繧ｫ繝｡繝ｩ繧｢繝ｳ繧ｰ繝ｫ縺ｮ譖ｴ譁ｰ縺ｨ繝槭え繧ｹ繧ｯ繝ｪ繝・け縺ｫ繧医ｋ逕ｻ髱｢驕ｷ遘ｻ蛻､螳・
void TitleScene::Update()
{
	Scene::Update();
	UpdateTitleCamera();
	HandleMenuInput();
}

/// @details 繧ｿ繧､繝医Ν閭梧勹繧呈雷蝗槭☆繧九き繝｡繝ｩ蠎ｧ讓吶・險育ｮ励→驕ｩ逕ｨ
void TitleScene::UpdateTitleCamera()
{
	// 豬ｮ蜍募ｰ乗焚轤ｹ邊ｾ蠎ｦ縺ｮ菴惹ｸ九↓繧医ｋ繧ｫ繝｡繝ｩ縺ｮ繧ｫ繧ｯ縺､縺阪ｒ髦ｲ縺舌◆繧√・ﾏ繝ｩ繧ｸ繧｢繝ｳ繧定ｶ・∴縺溘ｉ繝ｪ繧ｻ繝・ヨ縺吶ｋ
	camera_angle_ += 0.002f;
	if (camera_angle_ >= DX_PI_F * 2.0f) camera_angle_ -= DX_PI_F * 2.0f;

	VECTOR camPos = VGet(cosf(camera_angle_) * 3000.0f, 2000.0f, sinf(camera_angle_) * 3000.0f);
	VECTOR camTarget = VGet(0.0f, 1000.0f, 0.0f);
	SetCameraPositionAndTarget_UpVecY(camPos, camTarget);
}

/// @details 繧ｯ繝ｪ繝・け縺励◆繝懊ち繝ｳ縺ｫ蠢懊§縺溘す繝ｼ繝ｳ縺ｮ蛻・ｊ譖ｿ縺郁ｦ∵ｱ・
void TitleScene::HandleMenuInput()
{
	int mx, my;
	InputManager::GetMousePos(mx, my);

	// 辟｡鬧・↑蠖薙◆繧雁愛螳夊ｨ育ｮ励ｒ逵√￥縺溘ａ縲∝ｷｦ繧ｯ繝ｪ繝・け縺輔ｌ縺ｦ縺・↑縺・ヵ繝ｬ繝ｼ繝縺ｯ譌ｩ譛溘Μ繧ｿ繝ｼ繝ｳ
	if (!InputManager::CheckMouseClickLeft()) return;

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
	else if (IsHoverSettings(mx, my))
	{
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
		Master::scene_manager_->SetNextScene(SceneManager::kSceneSettings);
	}
}

/// @param 繝槭え繧ｹ蠎ｧ讓・(mx, my)
/// @return 蛻､螳夐伜沺蜀・〒縺ゅｌ縺ｰ true
bool TitleScene::IsHoverStart(int mx, int my) const
{
	return mx >= 96 && mx <= 416 && my >= 732 && my <= 794;
}

bool TitleScene::IsHoverRule(int mx, int my) const
{
	return mx >= 96 && mx <= 416 && my >= 792 && my <= 854;
}

bool TitleScene::IsHoverSettings(int mx, int my) const
{
	return mx >= 96 && mx <= 416 && my >= 872 && my <= 934;
}

/// @details 繧ｿ繧､繝医Ν縺ｮ3D閭梧勹縲√Ο繧ｴ縲∝推遞ｮUI繝代ロ繝ｫ縺ｮ謠冗判
void TitleScene::Draw()
{
	UpdatePromptBlink();
	DrawSceneBackground();
	DrawTitlePanel();
	DrawMenuPanel();
	DrawPrompt();
}

/// @details 繝ｦ繝ｼ繧ｶ繝ｼ縺ｫ繧ｯ繝ｪ繝・け繧剃ｿ・☆繝・く繧ｹ繝育畑繧｢繝ｫ繝輔ぃ蛟､縺ｮ險育ｮ・
void TitleScene::UpdatePromptBlink()
{
	// 騾城℃蠎ｦ繧貞ｾ舌・↓蠅玲ｸ帙＆縺帙√Θ繝ｼ繧ｶ繝ｼ縺ｮ隕也ｷ壹ｒ隱伜ｰ弱☆繧九◆繧√・貊代ｉ縺九↑譏取ｻ・い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧剃ｽ懊ｋ
	if (color_flag_)
	{
		color_fade_ -= 4;
		if (color_fade_ <= 0) { color_fade_ = 0; color_flag_ = false; }
	}
	else
	{
		color_fade_ += 4;
		if (color_fade_ >= 255) { color_fade_ = 255; color_flag_ = true; }
	}
}

/// @details 閭梧勹繝｢繝・Ν縺ｮ謠冗判縺ｨ縲ゞI縺ｮ隕冶ｪ肴ｧ繧帝ｫ倥ａ繧九◆繧√・蜊企乗・證怜ｹ輔・謠冗判
void TitleScene::DrawSceneBackground()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	Scene::Draw();

	// 譏ｼ閭梧勹縺ｪ縺ｩ譏主ｺｦ縺ｮ鬮倥＞3D繝｢繝・Ν縺梧擂縺溷ｴ蜷医〒繧ゅ∵焔蜑阪・逋ｽ譁・ｭ誘I縺瑚ｪｭ繧√↑縺上↑繧九・繧帝亟縺・
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(15, 18, 25), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// @details 繧ｲ繝ｼ繝繧ｿ繧､繝医Ν縺ｮ繝代ロ繝ｫ縺ｨ繝峨Ο繝・・繧ｷ繝｣繝峨え莉倥″繝・く繧ｹ繝医・謠冗判
void TitleScene::DrawTitlePanel()
{
	const int accentGold = GetColor(218, 178, 86);
	const int accentGoldDark = GetColor(98, 73, 32);
	const int mainPanel = GetColor(30, 35, 45);
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
	// 蜿ｳ荳九↓縺壹ｉ縺励※鮟呈枚蟄励ｒ謠冗判縺吶ｋ縺薙→縺ｧ縲√い繧ｦ繝医Λ繧､繝ｳ/蠖ｱ莉倥″繝輔か繝ｳ繝医ｒ謫ｬ莨ｼ逧・↓陦ｨ迴ｾ縺吶ｋ
	DrawFormatString(99, 119, GetColor(10, 8, 4), "Skybound Slayer");
	DrawFormatString(94, 114, GetColor(255, 231, 155), "Skybound Slayer");
	SetFontSize(24);
}

/// @details 繝槭え繧ｹ繝帙ヰ繝ｼ迥ｶ諷九↓蠢懊§縺溘・繧ｿ繝ｳ縺ｮ濶ｲ螟画峩縺ｨ謠冗判
void TitleScene::DrawMenuPanel()
{
	int mx, my;
	InputManager::GetMousePos(mx, my);

	bool hoverStart = IsHoverStart(mx, my);
	bool hoverRule = IsHoverRule(mx, my);
	bool hoverSettings = IsHoverSettings(mx, my);
	const int accentGold = GetColor(218, 178, 86);
	const int accentGoldDark = GetColor(98, 73, 32);
	const int mainPanel = GetColor(30, 35, 45);
	const int menuBtn = GetColor(20, 24, 32);
	const int panelLight = GetColor(47, 52, 65);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(70, 694, 448, 976, mainPanel, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawLine(70, 694, 448, 694, accentGold, 1);
	DrawLine(70, 976, 448, 976, accentGoldDark, 1);
	DrawLine(70, 694, 70, 976, accentGoldDark, 1);
	DrawLine(448, 694, 448, 976, accentGold, 1);
	DrawBox(84, 708, 434, 722, panelLight, TRUE);

	SetFontSize(34);
	DrawBox(96, 712, 416, 774, hoverStart ? GetColor(48, 39, 18) : menuBtn, TRUE);
	DrawLine(96, 712, 416, 712, hoverStart ? accentGold : accentGoldDark, 1);
	DrawFormatString(126, 728, hoverStart ? GetColor(255, 246, 184) : GetColor(222, 236, 248), "%sGAME START", hoverStart ? "> " : "  ");

	DrawBox(96, 792, 416, 854, hoverRule ? GetColor(48, 39, 18) : menuBtn, TRUE);
	DrawLine(96, 792, 416, 792, hoverRule ? accentGold : accentGoldDark, 1);
	DrawFormatString(126, 808, hoverRule ? GetColor(255, 246, 184) : GetColor(222, 236, 248), "%sRULE", hoverRule ? "> " : "  ");

	DrawBox(96, 872, 416, 934, hoverSettings ? GetColor(48, 39, 18) : menuBtn, TRUE);
	DrawLine(96, 872, 416, 872, hoverSettings ? accentGold : accentGoldDark, 1);
	DrawFormatString(126, 888, hoverSettings ? GetColor(255, 246, 184) : GetColor(222, 236, 248), "%sSETTINGS", hoverSettings ? "> " : "  ");
}

/// @details 逕ｻ髱｢荳矩Κ縺ｫ轤ｹ貊・☆繧区｡亥・繝・く繧ｹ繝医・謠冗判
void TitleScene::DrawPrompt()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155 + color_fade_ / 3);
	DrawBox(Config::ScreenWidth / 2 - 210, Config::ScreenHeight - 76, Config::ScreenWidth / 2 + 210, Config::ScreenHeight - 34, GetColor(30, 35, 45), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(22);
	DrawFormatString(Config::ScreenWidth / 2 - 156, Config::ScreenHeight - 66, GetColor(214, 220, 224), "CLICK A COMMAND TO BEGIN");
	SetFontSize(24);
}

void TitleScene::Finalize()
{
	Master::sound_manager_->StopBGM();
}
