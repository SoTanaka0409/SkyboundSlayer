#pragma once
#include "DxLib.h"

// アーキテクチャ設計：ゲーム中のポーズメニューを管理するクラス
// Pキーで展開され、音量設定やゲーム終了（タイトルへ戻る）機能を提供する
class PauseMenu
{
public:
	PauseMenu();
	~PauseMenu();

	void Initialize();
	void Update();
	void Draw();

private:
	struct Rect
	{
		int x, y, w, h;
	};

	void HandleInput();
	void DrawBackground();
	void DrawHeader();
	void DrawSettingRows();
	void DrawFooter();

	void DrawToggle(const Rect& rect, bool enabled, const char* label);
	void DrawVolumeRow(int y, const char* label, bool enabled, int volume);
	void DrawButton(const Rect& rect, const char* label, bool hover);
	bool IsMouseInRect(const Rect& rect, int mouseX, int mouseY) const;
	int GetVolumeFromMouseX(const Rect& rect, int mouseX) const;

	Rect bgm_toggle_rect_;
	Rect bgm_bar_rect_;
	Rect se_toggle_rect_;
	Rect se_bar_rect_;
	Rect debug_toggle_rect_;
	Rect resume_rect_;
	Rect quit_rect_;

	int color_fade_;
	bool color_flag_;
};