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

// 入力: なし / 出力: なし
// 副作用: 各種アニメーション制御用変数の初期化
TitleScene::TitleScene()
	: color_fade_(1), color_flag_(false), camera_angle_(0.0f)
{
}

TitleScene::~TitleScene()
{
}

// 入力: なし / 出力: なし
// 副作用: カメラやスコアの初期化、およびタイトル画面の背景となる3Dステージ・オブジェクト群のVRAMロード
void TitleScene::Initialize()
{
	Master::camera_->Initialize();
	Master::game_clear_count_ = 0;
	ColliderManager::GetInstance()->DeleteAllCollider();

	Master::score_manager_->LoadHighScore();
	Master::sound_manager_->PlayBGM(SoundManager::BGM_TITLE);

	// 実際のゲームプレイ中と同じ迫力ある背景を演出として表示するため、本編用のステージをロードする
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1", VGet(3.0f, 0.3f, 3.0f));

	std::ifstream file("Resource/CSV/stage_objects.csv");
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line); // 1行目は定義用のCSVヘッダーであるため読み飛ばす
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
			}
			else if (type == "Stage") {
				new Stage(pos, model, model, scale, texture);
			}
		}
		file.close();
	}

	SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x", VGet(0, 0, -5000));
	float scale = 13.0f;
	pSkyBox->SetScale(VGet(scale, scale, scale));
	pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");

	// 景観のアクセントとしてタイトル画面専用のオブジェクトを配置
	new StageObject(VGet(0.0f, 0.0f, 500.0f), "Resource/3D/portal/source/portal.mv1", VGet(3.0f, 3.0f, 3.0f));

	camera_angle_ = 0.0f;
}

// 入力: なし / 出力: なし
// 副作用: カメラの旋回アニメーションと、プレイヤーのマウス入力によるUI選択状態を毎フレーム処理する
void TitleScene::Update()
{
	Scene::Update();
	UpdateTitleCamera();
	HandleMenuInput();
}

// 入力: なし / 出力: なし
// 副作用: 静止画のような退屈な印象を避けるため、原点を中心にカメラを円軌道で自動旋回させる
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

// 入力: なし / 出力: なし
// 副作用: 左クリック検知時にマウス座標とボタン領域を照合し、条件合致時に対応するシーンへの遷移要求を発行する
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

// 入力: mx, my (マウス座標) / 出力: 対象領域内か否か(bool)
// 副作用: なし（ハードコードされたSTARTボタン領域に対する静的な当たり判定）
bool TitleScene::IsHoverStart(int mx, int my) const
{
	return mx >= 96 && mx <= 416 && my >= 732 && my <= 794;
}

// 入力: mx, my (マウス座標) / 出力: 対象領域内か否か(bool)
// 副作用: なし（ハードコードされたRULEボタン領域に対する静的な当たり判定）
bool TitleScene::IsHoverRule(int mx, int my) const
{
	return mx >= 96 && mx <= 416 && my >= 812 && my <= 874;
}

// 入力: なし / 出力: なし
// 副作用: 背景3Dモデルの上に、タイトルロゴやメニューUIなどをZ順を考慮して合成し、描画バッファへ登録する
void TitleScene::Draw()
{
	UpdatePromptBlink();
	DrawSceneBackground();
	DrawTitlePanel();
	DrawMenuPanel();
	DrawPrompt();
}

// 入力: なし / 出力: なし
// 副作用: ユーザーの目を引くため、アルファ値を周期的に増減させてテキストの点滅（ブリンク）演出を進行させる
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

// 入力: なし / 出力: なし
// 副作用: UIパネルの視認性を高めるため、3D背景とUIの中間層に半透明の暗いフィルター（暗幕）を描画する
void TitleScene::DrawSceneBackground()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	Scene::Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(15, 18, 25), TRUE); // Base 60%
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 入力: なし / 出力: なし
// 副作用: メインタイトルの文字列と、それを装飾する背景パネル・枠線を描画する
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
	// 黒いテキストを少しずらして先に描画することでドロップシャドウを表現し、テキストの可読性を向上させる
	DrawFormatString(99, 119, GetColor(10, 8, 4), "Sky Castle Hunter");
	DrawFormatString(94, 114, GetColor(255, 231, 155), "Sky Castle Hunter");
	SetFontSize(24);
}

// 入力: なし / 出力: なし
// 副作用: マウスのホバー状態を動的に反映させながら、各種メニューボタンと装飾を描画する
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

// 入力: なし / 出力: なし
// 副作用: UpdatePromptBlinkで更新されたアルファ値を適用し、点滅する操作プロンプトを描画する
void TitleScene::DrawPrompt()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155 + color_fade_ / 3);
	DrawBox(Config::ScreenWidth / 2 - 210, Config::ScreenHeight - 76, Config::ScreenWidth / 2 + 210, Config::ScreenHeight - 34, GetColor(30, 35, 45), TRUE); // Main 30%
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(22);
	DrawFormatString(Config::ScreenWidth / 2 - 156, Config::ScreenHeight - 66, GetColor(214, 220, 224), "CLICK A COMMAND TO BEGIN");
	SetFontSize(24);
}

// 入力: なし / 出力: なし
// 副作用: なし（このクラス固有の動的リソースがないため空実装とする）
void TitleScene::Finalize()
{
}