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

// 入力：なし
// 出力：なし
// 副作用：点滅演出用タイマーおよびカメラ角度の初期化
TitleScene::TitleScene()
	: color_fade_(1), color_flag_(false), camera_angle_(0.0f)
{
}

TitleScene::~TitleScene()
{
}

// 入力：なし
// 出力：なし
// 副作用：カメラリセット、ハイスコア読み込み、BGM開始、およびステージ背景・天球モデルのデータ駆動型ロード
void TitleScene::Initialize()
{
	Master::camera_->Initialize();
	Master::game_clear_count_ = 0;
	ColliderManager::GetInstance()->DeleteAllCollider();

	Master::score_manager_->LoadHighScore();
	Master::sound_manager_->PlayBGM(SoundManager::BGM_TITLE);

	// レベルデザイン：静止画ではなく、実際のゲームプレイと同じ3D環境を背景に動かすことで、起動直後の没入感を向上させる
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1", VGet(3.0f, 0.3f, 3.0f));

	// データ駆動設計：ステージ配置をCSV管理することで、開発者がモデルの座標を容易に調整できるようにする
	std::ifstream file("Resource/CSV/stage_objects.csv");
	if (file.is_open())
	{
		std::string line;
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

	new StageObject(VGet(0.0f, 0.0f, 500.0f), "Resource/3D/portal/source/portal.mv1", VGet(3.0f, 3.0f, 3.0f));
	camera_angle_ = 0.0f;
}

// 入力：なし
// 出力：なし
// 副作用：カメラ旋回アニメーションの進行と、UIへのマウス入力判定の毎フレーム実行
void TitleScene::Update()
{
	Scene::Update();
	UpdateTitleCamera();
	HandleMenuInput();
}

// 入力：なし
// 出力：なし
// 副作用：カメラ位置の三角関数計算および強制注視点設定による旋回演出
void TitleScene::UpdateTitleCamera()
{
	// 演出仕様：常にカメラを円軌道で自動旋回させることで、タイトル画面に動的な広がりを持たせる
	camera_angle_ += 0.002f;
	if (camera_angle_ >= DX_PI_F * 2.0f) camera_angle_ -= DX_PI_F * 2.0f;

	VECTOR camPos = VGet(cosf(camera_angle_) * 3000.0f, 2000.0f, sinf(camera_angle_) * 3000.0f);
	VECTOR camTarget = VGet(0.0f, 1000.0f, 0.0f);
	SetCameraPositionAndTarget_UpVecY(camPos, camTarget);
}

// 入力：なし
// 出力：なし
// 副作用：マウス座標とUIボタン領域の衝突判定に基づく、シーン遷移要求の発行
void TitleScene::HandleMenuInput()
{
	int mx, my;
	InputManager::GetMousePos(mx, my);

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

// 入力：mx, my = マウス座標
// 出力：指定領域内であれば true
// 副作用：なし（静的なヒットボックス判定）
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

// 入力：なし
// 出力：なし
// 副作用：UI要素（タイトルロゴ、パネル、プロンプト）の描画命令発行
void TitleScene::Draw()
{
	UpdatePromptBlink();
	DrawSceneBackground();
	DrawTitlePanel();
	DrawMenuPanel();
	DrawPrompt();
}

// 入力：なし
// 出力：なし
// 副作用：color_fade_値の周期的な変動
void TitleScene::UpdatePromptBlink()
{
	// 演出仕様：ユーザーの操作を促すテキストを明滅させ、インタラクティブ性を強調する
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

// 入力：なし
// 出力：なし
// 副作用：シーン背景の描画およびUI可読性を高める暗幕の重ね描き
void TitleScene::DrawSceneBackground()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	Scene::Draw();

	// 視認性仕様：3D空間が明るい場合でもUIが読みやすいよう、背景の上に半透明の暗幕を敷くことでコントラストを調整する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(15, 18, 25), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 入力：なし
// 出力：なし
// 副作用：タイトルロゴの描画（影による可読性向上処理を含む）
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
	// 視認性仕様：テキストにオフセットした暗色を先に描画し、ドロップシャドウを表現することで文字の可読性を確保する
	DrawFormatString(99, 119, GetColor(10, 8, 4), "Sky Castle Hunter");
	DrawFormatString(94, 114, GetColor(255, 231, 155), "Sky Castle Hunter");
	SetFontSize(24);
}

// 入力：なし
// 出力：なし
// 副作用：マウスホバー判定に合わせた各メニューボタンのカラー変更と描画
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

// 入力：なし
// 出力：なし
// 副作用：点滅プロンプトの描画
void TitleScene::DrawPrompt()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 155 + color_fade_ / 3);
	DrawBox(Config::ScreenWidth / 2 - 210, Config::ScreenHeight - 76, Config::ScreenWidth / 2 + 210, Config::ScreenHeight - 34, GetColor(30, 35, 45), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(22);
	DrawFormatString(Config::ScreenWidth / 2 - 156, Config::ScreenHeight - 66, GetColor(214, 220, 224), "CLICK A COMMAND TO BEGIN");
	SetFontSize(24);
}

// 入力：なし
// 出力：なし
// 副作用：なし
void TitleScene::Finalize()
{
}