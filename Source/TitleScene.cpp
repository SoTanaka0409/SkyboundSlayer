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

/// @brief TitleSceneのコンストラクタ
/// @details UI点滅アニメーション用パラメータおよびカメラ角度の初期化を行う
TitleScene::TitleScene()
	: color_fade_(1), color_flag_(false), camera_angle_(0.0f)
{
}

/// @brief TitleSceneのデストラクタ
TitleScene::~TitleScene()
{
}

/// @brief タイトルシーンの初期化処理
/// @details カメラ・進行度・コライダーの初期化、BGM再生、およびタイトル専用3D背景・ステージオブジェクトの生成を行う
void TitleScene::Initialize()
{
	SetMouseDispFlag(true);
	Master::camera_->Initialize();
	// 周回プレイ時の不具合を防ぐため、タイトルに戻った時点でクリア回数とコライダー情報をリセットする
	Master::game_clear_count_ = 0;
	ColliderManager::GetInstance()->DeleteAllCollider();

	Master::score_manager_->LoadHighScore();
	Master::sound_manager_->PlayBGM(SoundManager::BGM_TITLE);

	// 一枚絵ではなく、実際のゲームプレイと同じ3Dモデルを配置してカメラを回すことでシームレスな世界観を演出する
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/model/bg/island/01_island.mv1", "Resource/model/bg/island/01_island.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/model/stage/normal/01_normal_stage.mv1", "Resource/model/stage/normal/02_normal_collider.mv1", VGet(3.0f, 0.3f, 3.0f));

	// 配置データのハードコーディングを避け、CSVから読み込むことでプランナーの調整工数を削減する
	std::ifstream file(L"Resource/data/CSV/01_stage_layout.csv");
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line); // ヘッダー行をスキップしてパースエラーを回避
		while (std::getline(file, line))
		{
			if (line.empty()) continue; // 空行によるクラッシュを防止
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

	SkyBox* pSkyBox = new SkyBox("Resource/model/bg/sky/01_sky_dome_model.x", VGet(0, 0, -5000));
	float scale = 13.0f;
	pSkyBox->SetScale(VGet(scale, scale, scale));
	pSkyBox->SetModelTexture("Resource/model/bg/sky/02_sky_texture.jpg");

	new StageObject(VGet(0.0f, 0.0f, 500.0f), "Resource/model/props/portal/01_portal.mv1", VGet(3.0f, 3.0f, 3.0f));
	camera_angle_ = 0.0f;
}

/// @brief タイトルシーンの毎フレーム更新処理
/// @details カメラアングルの更新とマウス入力による画面遷移判定を行う
void TitleScene::Update()
{
	Scene::Update();
	UpdateTitleCamera();
	HandleMenuInput();
}

/// @brief タイトル背景を周回するカメラ座標の計算および適用を行う
void TitleScene::UpdateTitleCamera()
{
	// 浮動小数点精度の低下によるカメラのカクつきを防ぐため、2πラジアンを超えたらリセットする
	camera_angle_ += 0.002f;
	if (camera_angle_ >= DX_PI_F * 2.0f) camera_angle_ -= DX_PI_F * 2.0f;

	VECTOR camPos = VGet(cosf(camera_angle_) * 3000.0f, 2000.0f, sinf(camera_angle_) * 3000.0f);
	VECTOR camTarget = VGet(0.0f, 1000.0f, 0.0f);
	SetCameraPositionAndTarget_UpVecY(camPos, camTarget);
}

/// @brief メニュー選択のマウス入力およびシーン切り替え処理を行う
void TitleScene::HandleMenuInput()
{
	int mx, my;
	InputManager::GetMousePos(mx, my);

	// 無駄な当たり判定計算を省くため、左クリックされていないフレームは早期リターン
	if (!InputManager::CheckMouseClickLeft()) return;

	if (IsHoverStart(mx, my))
	{
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
		Master::scene_manager_->SetNextScene(SceneManager::kGameScene);
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

/// @brief マウスカーソルが「GAME START」ボタン上にあるか判定する
/// @param mx マウスのX座標
/// @param my マウスのY座標
/// @return bool ボタンの判定領域内であればtrue
bool TitleScene::IsHoverStart(int mx, int my) const
{
	return mx >= 96 && mx <= 516 && my >= 732 && my <= 794;
}

/// @brief マウスカーソルが「RULE」ボタン上にあるか判定する
/// @param mx マウスのX座標
/// @param my マウスのY座標
/// @return bool ボタンの判定領域内であればtrue
bool TitleScene::IsHoverRule(int mx, int my) const
{
	return mx >= 96 && mx <= 516 && my >= 792 && my <= 854;
}

/// @brief マウスカーソルが「SETTINGS」ボタン上にあるか判定する
/// @param mx マウスのX座標
/// @param my マウスのY座標
/// @return bool ボタンの判定領域内であればtrue
bool TitleScene::IsHoverSettings(int mx, int my) const
{
	return mx >= 96 && mx <= 516 && my >= 872 && my <= 934;
}

/// @brief タイトルシーンの全画面描画処理を行う
/// @details 背景・ロゴ・各ボタンUI・案内プロンプトの描画を行う
void TitleScene::Draw()
{
	UpdatePromptBlink();
	DrawSceneBackground();
	DrawTitlePanel();
	DrawMenuPanel();
	DrawPrompt();
}

/// @brief プロンプトテキスト（案内表示）の点滅用アルファ値を計算更新する
void TitleScene::UpdatePromptBlink()
{
	// 透明度を徐々に増減させ、ユーザーの視線を誘導するための滑らかな明滅アニメーションを作る
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

/// @brief 3D背景モデルの描画およびUI視認性を高める暗転幕の描画を行う
void TitleScene::DrawSceneBackground()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	Scene::Draw();

	// 昼背景など明度の高い3Dモデルが来たい場合でも、手前の白文字UIが読めなくなるのを防ぐ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(15, 18, 25), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// @brief ゲームタイトルロゴパネルおよびドロップシャドウ付きタイトルの描画を行う
void TitleScene::DrawTitlePanel()
{
	const int accentGold = GetColor(218, 178, 86);
	const int accentGoldDark = GetColor(98, 73, 32);
	const int mainPanel = GetColor(30, 35, 45);
	const int panelLight = GetColor(47, 52, 65);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(58, 70, 960, 250, mainPanel, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawLine(70, 70, 960, 70, accentGold, 2);
	DrawLine(70, 250, 960, 250, accentGoldDark, 2);
	DrawLine(70, 70, 70, 250, accentGoldDark, 2);
	DrawLine(960, 70, 960, 250, accentGold, 2);
	DrawBox(84, 88, 946, 98, panelLight, TRUE);

	SetFontSize(76);
	// 右下にずらして黒文字を描画することで、アウトライン/影付きフォントを疑似的に表現する
	DrawFormatString(99, 119, GetColor(10, 8, 4), "Skybound Slayer");
	DrawFormatString(94, 114, GetColor(255, 231, 155), "Skybound Slayer");
	SetFontSize(24);
}

/// @brief メニューパネルおよびホバー状態に応じたボタンの描画を行う
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
	DrawBox(70, 694, 548, 976, mainPanel, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawLine(70, 694, 548, 694, accentGold, 1);
	DrawLine(70, 976, 548, 976, accentGoldDark, 1);
	DrawLine(70, 694, 70, 976, accentGoldDark, 1);
	DrawLine(548, 694, 548, 976, accentGold, 1);
	DrawBox(84, 708, 534, 722, panelLight, TRUE);

	auto drawAnimBtn = [&](int bx, int by, int bw, int bh, bool hover, const char* label) {
		int expand = hover ? 8 : 0;
		int fill = hover ? GetColor(60, 50, 20) : menuBtn;
		int textC = hover ? GetColor(255, 255, 200) : GetColor(222, 236, 248);
		int lineC = hover ? GetColor(255, 215, 100) : accentGoldDark;
		
		DrawBox(bx - expand, by - expand, bx + bw + expand, by + bh + expand, fill, TRUE);
		DrawLine(bx - expand, by - expand, bx + bw + expand, by - expand, lineC, hover ? 2 : 1);
		
		SetFontSize(hover ? 38 : 34);
		DrawFormatString(bx + 30 - expand, by + 16 - expand/2, textC, "%s%s", hover ? "> " : "  ", label);
	};

	drawAnimBtn(96, 712, 420, 62, hoverStart, "GAME START");
	drawAnimBtn(96, 792, 420, 62, hoverRule, "RULE");
	drawAnimBtn(96, 872, 420, 62, hoverSettings, "SETTINGS");
}

/// @brief 画面下部の点滅案内プロンプトを描画する
void TitleScene::DrawPrompt()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155 + color_fade_ / 3);
	DrawBox(Config::ScreenWidth / 2 - 320, Config::ScreenHeight - 76, Config::ScreenWidth / 2 + 320, Config::ScreenHeight - 34, GetColor(30, 35, 45), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(22);
	DrawFormatString(Config::ScreenWidth / 2 - 220, Config::ScreenHeight - 66, GetColor(214, 220, 224), "CLICK A COMMAND TO BEGIN");
	SetFontSize(24);
}

/// @brief タイトルシーンの終了処理を行う
/// @details BGMの停止処理を実行する
void TitleScene::Finalize()
{
	Master::sound_manager_->StopBGM();
}