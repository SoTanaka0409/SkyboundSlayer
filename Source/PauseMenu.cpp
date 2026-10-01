#include "PauseMenu.h"
#include "Config.h"
#include "Master.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

PauseMenu::PauseMenu()
	: m_ColorFade(0)
	, m_ColorFlag(false)
{
	Initialize();
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::Initialize()
{
	m_BgmToggleRect = { 720, 312, 150, 48 };
	m_BgmBarRect = { 920, 320, 380, 24 };

	m_SeToggleRect = { 720, 432, 150, 48 };
	m_SeBarRect = { 920, 440, 380, 24 };

	m_DebugToggleRect = { 820, 552, 150, 48 };

	m_ResumeRect = { 600, 750, 300, 60 };
	m_QuitRect = { 1020, 750, 300, 60 };
}

void PauseMenu::Update()
{
	HandleInput();
}

void PauseMenu::Draw()
{
	DrawBackground();
	DrawHeader();
	DrawSettingRows();
	DrawFooter();
}

void PauseMenu::HandleInput()
{
	int mouseX, mouseY;
	InputManager::GetMousePos(mouseX, mouseY);

	if (!InputManager::CheckMouseClickLeft()) return;

	if (IsMouseInRect(m_BgmToggleRect, mouseX, mouseY))
	{
		Master::m_SoundManager->ToggleBgmEnabled();
		Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(m_BgmBarRect, mouseX, mouseY))
	{
		Master::m_SoundManager->SetBgmVolume(GetVolumeFromMouseX(m_BgmBarRect, mouseX));
		Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(m_SeToggleRect, mouseX, mouseY))
	{
		Master::m_SoundManager->ToggleSeEnabled();
		Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(m_SeBarRect, mouseX, mouseY))
	{
		Master::m_SoundManager->SetSeVolume(GetVolumeFromMouseX(m_SeBarRect, mouseX));
		Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(m_DebugToggleRect, mouseX, mouseY))
	{
		Master::m_Debug->SetDebug(!Master::m_Debug->Getdebug());
		Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
	}
	else if (IsMouseInRect(m_ResumeRect, mouseX, mouseY))
	{
		Master::m_SoundManager->PlaySE(SoundManager::SE_WINDOW);
		Master::m_IsPauseOn = false;
		SetMouseDispFlag(FALSE);
	}
	else if (IsMouseInRect(m_QuitRect, mouseX, mouseY))
	{
		Master::m_SoundManager->PlaySE(SoundManager::SE_WINDOW);
		Master::m_IsPauseOn = false;
		Master::m_SceneManager->SetNextScene(SceneManager::kSceneTitle);
	}
}

void PauseMenu::DrawBackground()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(11, 14, 20), TRUE);

	DrawBox(520, 110, 1450, 850, GetColor(23, 27, 36), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawLine(520, 110, 1450, 110, GetColor(218, 178, 86), 2);
	DrawLine(520, 850, 1450, 850, GetColor(98, 73, 32), 2);
	DrawLine(520, 110, 520, 850, GetColor(98, 73, 32), 2);
	DrawLine(1450, 110, 1450, 850, GetColor(218, 178, 86), 2);
}

void PauseMenu::DrawHeader()
{
	SetFontSize(72);
	DrawFormatString(604, 134, GetColor(0, 0, 0), "PAUSE");
	DrawFormatString(598, 128, GetColor(255, 231, 155), "PAUSE");
}

void PauseMenu::DrawSettingRows()
{
	DrawVolumeRow(280, "BGM", Master::m_SoundManager->IsBgmEnabled(), Master::m_SoundManager->GetBgmVolume());
	DrawVolumeRow(400, "SE", Master::m_SoundManager->IsSeEnabled(), Master::m_SoundManager->GetSeVolume());

	SetFontSize(34);
	DrawFormatString(594, 560, GetColor(222, 236, 248), "DEBUG");
	DrawToggle(m_DebugToggleRect, Master::m_Debug->Getdebug(), "MODE");

	SetFontSize(18);
	DrawFormatString(990, 568, GetColor(150, 164, 180), "ON/OFF: debug display & controls");
	SetFontSize(24);
}

void PauseMenu::DrawFooter()
{
	int mouseX, mouseY;
	InputManager::GetMousePos(mouseX, mouseY);
	
	DrawButton(m_ResumeRect, "RESUME", IsMouseInRect(m_ResumeRect, mouseX, mouseY));
	DrawButton(m_QuitRect, "QUIT", IsMouseInRect(m_QuitRect, mouseX, mouseY));
}

void PauseMenu::DrawToggle(const Rect& rect, bool enabled, const char* label)
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

void PauseMenu::DrawVolumeRow(int y, const char* label, bool enabled, int volume)
{
	Rect toggleRect = label[0] == 'B' ? m_BgmToggleRect : m_SeToggleRect;
	Rect barRect = label[0] == 'B' ? m_BgmBarRect : m_SeBarRect;
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
	DrawFormatString(barRect.x + barRect.w + 14, barRect.y - 2, enabled ? GetColor(245, 246, 248) : GetColor(130, 136, 146), "%3d%%", percent);
}

void PauseMenu::DrawButton(const Rect& rect, const char* label, bool hover)
{
	const int fill = hover ? GetColor(60, 50, 20) : GetColor(20, 24, 32);
	const int text_color = hover ? GetColor(255, 255, 200) : GetColor(222, 236, 248);
	int expand = hover ? 6 : 0; // ホバー時に枠を拡大して強調

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
	DrawBox(rect.x - expand, rect.y - expand, rect.x + rect.w + expand, rect.y + rect.h + expand, fill, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawLine(rect.x - expand, rect.y - expand, rect.x + rect.w + expand, rect.y - expand, hover ? GetColor(255, 215, 100) : GetColor(98, 73, 32), 2);

	SetFontSize(hover ? 32 : 30);
	DrawFormatString(rect.x + 48 - expand, rect.y + 17 - expand/2, text_color, "%s%s", hover ? "> " : "  ", label);
	SetFontSize(24);
}

bool PauseMenu::IsMouseInRect(const Rect& rect, int mouseX, int mouseY) const
{
	return mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h;
}

int PauseMenu::GetVolumeFromMouseX(const Rect& rect, int mouseX) const
{
	int localX = mouseX - rect.x;
	if (localX < 0) localX = 0;
	if (localX > rect.w) localX = rect.w;
	return localX * 255 / rect.w;
}