#include "SettingsScene.h"
#include "Config.h"
#include "InputManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "SoundManager.h"

/// @details UIレイアウト用の各矩形領域および演出用アニメーション変数の初期化
SettingsScene::SettingsScene()
	: bgm_toggle_rect_{ 720, 312, 150, 48 }
	, bgm_bar_rect_{ 920, 320, 420, 30 }
	, se_toggle_rect_{ 720, 432, 150, 48 }
	, se_bar_rect_{ 920, 440, 420, 30 }
	, debug_toggle_rect_{ 720, 552, 150, 48 }
	, back_rect_{ 80, 860, 260, 62 }
	, color_fade_(0)
	, color_flag_(false)
{
}

SettingsScene::~SettingsScene()
{
}

/// @details アニメーション変数のリセット
void SettingsScene::Initialize()
{
	color_fade_ = 0;
	color_flag_ = false;
}

/// @details ユーザー入力（マウス操作）の検知と設定状態の更新
void SettingsScene::Update()
{
	HandleInput();
}

/// @details 設定メニューUIのレンダリング命令の発行
void SettingsScene::Draw()
{
	DrawBackground();
	DrawHeader();
	DrawSettingRows();
	DrawFooter();
}

void SettingsScene::Finalize()
{
}

/// @details 各UI矩形に対するマウスホバー
void SettingsScene::HandleInput()
{
	int mouseX, mouseY;
	InputManager::GetMousePos(mouseX, mouseY);

	if (!InputManager::CheckMouseClickLeft()) return;

	// UX仕様：直感的な操作感を提供するため、トグルスイッチおよびボリュームバーのクリックを検知し、SEで即時フィードバックを返す
	if (IsMouseInRect(bgm_toggle_rect_, mouseX, mouseY))
	{
		Master::sound_manager_->ToggleBgmEnabled();
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(bgm_bar_rect_, mouseX, mouseY))
	{
		Master::sound_manager_->SetBgmVolume(GetVolumeFromMouseX(bgm_bar_rect_, mouseX));
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(se_toggle_rect_, mouseX, mouseY))
	{
		Master::sound_manager_->ToggleSeEnabled();
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(se_bar_rect_, mouseX, mouseY))
	{
		Master::sound_manager_->SetSeVolume(GetVolumeFromMouseX(se_bar_rect_, mouseX));
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(debug_toggle_rect_, mouseX, mouseY))
	{
		Master::debug_->SetDebug(!Master::debug_->Getdebug());
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(back_rect_, mouseX, mouseY))
	{
		Master::sound_manager_->PlaySE(SoundManager::SE_WINDOW);
		Master::scene_manager_->SetNextScene(SceneManager::kSceneTitle);
	}
}

/// @details 画面背景および設定メニューのパネル枠を描画
void SettingsScene::DrawBackground()
{
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(11, 14, 20), TRUE);

	// 背景の装飾：単調さを消すため、画面を横切るラインを複数配置して奥行き感を演出
	for (int i = 0; i < 18; ++i)
	{
		int y = 74 + i * 52;
		DrawLine(0, y, Config::ScreenWidth, y + 150, GetColor(25, 31, 43), 1);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawBox(520, 210, 1450, 710, GetColor(23, 27, 36), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawLine(520, 210, 1450, 210, GetColor(218, 178, 86), 2);
	DrawLine(520, 710, 1450, 710, GetColor(98, 73, 32), 2);
	DrawLine(520, 210, 520, 710, GetColor(98, 73, 32), 2);
	DrawLine(1450, 210, 1450, 710, GetColor(218, 178, 86), 2);
}

/// @details ヘッダーラベル「SETTINGS」と操作説明テキストの描画
void SettingsScene::DrawHeader()
{
	SetFontSize(72);
	DrawFormatString(604, 94, GetColor(0, 0, 0), "SETTINGS");
	DrawFormatString(598, 88, GetColor(255, 231, 155), "SETTINGS");

	SetFontSize(24);
	DrawFormatString(664, 176, GetColor(180, 190, 205), "LEFT CLICK TO ADJUST");
	SetFontSize(24);
}

/// @details BGM・SEの音量設定行およびDebugモード切替ボタンの描画
void SettingsScene::DrawSettingRows()
{
	DrawVolumeRow(280, "BGM", Master::sound_manager_->IsBgmEnabled(), Master::sound_manager_->GetBgmVolume());
	DrawVolumeRow(400, "SE", Master::sound_manager_->IsSeEnabled(), Master::sound_manager_->GetSeVolume());

	SetFontSize(34);
	DrawFormatString(594, 560, GetColor(222, 236, 248), "DEBUG");
	DrawToggle(debug_toggle_rect_, Master::debug_->Getdebug(), "MODE");

	SetFontSize(22);
	DrawFormatString(920, 564, GetColor(150, 164, 180), "Shows collider and debug controls in game");
	SetFontSize(24);
}

/// @details フッター領域（戻るボタン）と変更即時適用に関する注意書きの描画
void SettingsScene::DrawFooter()
{
	int mouseX, mouseY;
	InputManager::GetMousePos(mouseX, mouseY);
	DrawButton(back_rect_, "BACK", IsMouseInRect(back_rect_, mouseX, mouseY));

	// UX仕様：設定は即時適用されることをユーザーに示し、保存ボタンを探す手間を省く
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130 + color_fade_ / 2);
	DrawBox(1330, 870, 1810, 918, GetColor(20, 24, 32), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(20);
	DrawFormatString(1370, 884, GetColor(200, 210, 220), "Settings are applied immediately");
	SetFontSize(24);

	// 演出：通知エリアを周期的に明滅させ、ユーザーの注意を引く
	if (color_flag_)
	{
		color_fade_ -= 3;
		if (color_fade_ <= 0) { color_fade_ = 0; color_flag_ = false; }
	}
	else
	{
		color_fade_ += 3;
		if (color_fade_ >= 255) { color_fade_ = 255; color_flag_ = true; }
	}
}

/// @param rect = 領域, enabled = ON
/// @details 状態（色と文字）を反映したトグルスイッチの描画
void SettingsScene::DrawToggle(const Rect& rect, bool enabled, const char* label)
{
	const int fill = enabled ? GetColor(42, 92, 64) : GetColor(58, 42, 46);
	const int accent = enabled ? GetColor(106, 238, 150) : GetColor(238, 112, 116);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
	DrawBox(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, fill, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawLine(rect.x, rect.y, rect.x + rect.w, rect.y, accent, 2);
	DrawLine(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, GetColor(20, 20, 24), 1);

	SetFontSize(24);
	DrawFormatString(rect.x + 18, rect.y + 13, GetColor(245, 246, 248), "%s", enabled ? "ON" : "OFF");
	DrawFormatString(rect.x + 82, rect.y + 13, GetColor(190, 200, 212), "%s", label);
}

/// @param y = 縦位置, label = ラベル, enabled = 状態, volume = 音量
/// @details ボリューム調整用バー（スライダー）の描画
void SettingsScene::DrawVolumeRow(int y, const char* label, bool enabled, int volume)
{
	Rect toggleRect = label[0] == 'B' ? bgm_toggle_rect_ : se_toggle_rect_;
	Rect barRect = label[0] == 'B' ? bgm_bar_rect_ : se_bar_rect_;
	int percent = volume * 100 / 255;
	int fillWidth = barRect.w * volume / 255;

	SetFontSize(34);
	DrawFormatString(594, y + 42, GetColor(222, 236, 248), "%s", label);
	DrawToggle(toggleRect, enabled, "AUDIO");

	DrawBox(barRect.x, barRect.y, barRect.x + barRect.w, barRect.y + barRect.h, GetColor(8, 10, 14), TRUE);
	DrawBox(barRect.x, barRect.y, barRect.x + fillWidth, barRect.y + barRect.h, enabled ? GetColor(71, 174, 210) : GetColor(72, 78, 88), TRUE);
	DrawLine(barRect.x, barRect.y, barRect.x + barRect.w, barRect.y, GetColor(218, 178, 86), 1);
	DrawLine(barRect.x, barRect.y + barRect.h, barRect.x + barRect.w, barRect.y + barRect.h, GetColor(98, 73, 32), 1);

	SetFontSize(24);
	DrawFormatString(barRect.x + barRect.w + 30, barRect.y - 2, enabled ? GetColor(245, 246, 248) : GetColor(130, 136, 146), "%3d%%", percent);
}

/// @param rect = 領域, label = ボタン名, hover = ホバー中か
/// @details 状態に応じた配色でのボタン描画
void SettingsScene::DrawButton(const Rect& rect, const char* label, bool hover)
{
	const int fill = hover ? GetColor(48, 39, 18) : GetColor(20, 24, 32);
	const int text = hover ? GetColor(255, 246, 184) : GetColor(222, 236, 248);

	DrawBox(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, fill, TRUE);
	DrawLine(rect.x, rect.y, rect.x + rect.w, rect.y, hover ? GetColor(218, 178, 86) : GetColor(98, 73, 32), 2);

	SetFontSize(30);
	DrawFormatString(rect.x + 48, rect.y + 17, text, "%s%s", hover ? "> " : "  ", label);
	SetFontSize(24);
}

/// @param rect = 領域, mouseX
/// @return 領域内であれば true
bool SettingsScene::IsMouseInRect(const Rect& rect, int mouseX, int mouseY) const
{
	return mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h;
}

/// @param rect = バー領域, mouseX = マウスX座標
/// @return 音量値 (0-255)
int SettingsScene::GetVolumeFromMouseX(const Rect& rect, int mouseX) const
{
	int localX = mouseX - rect.x;
	if (localX < 0) localX = 0;
	if (localX > rect.w) localX = rect.w;
	return localX * 255 / rect.w;
}