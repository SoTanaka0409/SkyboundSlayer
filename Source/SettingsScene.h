#pragma once
#include "DxLib.h"
#include "Scene.h"

// アーキテクチャ設計：設定画面を担当するシーンクラス。
// 各種サウンド設定やデバッグフラグをUI上で視覚的に操作し、Masterクラスの静的状態へ即時反映させるためのインターフェースを定義する。
class SettingsScene : public Scene
{
public:
	SettingsScene();
	~SettingsScene();

	// 入力：なし
	// 出力：なし
	// 副作用：設定用UI要素の座標・矩形領域の初期化
	void Initialize() override;

	// 入力：なし
	// 出力：なし
	// 副作用：マウス入力による設定値の変更およびメニューのインタラクション制御
	void Update() override;

	// 入力：なし
	// 出力：なし
	// 副作用：ヘッダー、各種設定項目（トグル・ボリュームバー）、および戻るボタンのレンダリング
	void Draw() override;

	// 入力：なし
	// 出力：なし
	// 副作用：設定画面専用リソースの破棄
	void Finalize() override;

private:
	// アーキテクチャ設計：画面内のUI要素を矩形（x, y, w, h）で管理し、当たり判定や描画位置計算を統一する構造体
	struct Rect
	{
		int x, y, w, h;
	};

	// 入力：なし
	// 出力：なし
	// 副作用：マウス操作に基づいたトグル状態の変更や、ボリュームバーのドラッグ処理
	void HandleInput();

	// 各描画レイヤーの分担処理
	void DrawBackground();
	void DrawHeader();
	void DrawSettingRows();
	void DrawFooter();

	// 入力：rect = 描画領域, enabled = ON/OFF状態, label = ラベル名
	// 副作用：設定項目のトグルスイッチを描画
	void DrawToggle(const Rect& rect, bool enabled, const char* label);

	// 入力：y = 縦位置, label = ラベル名, enabled = 有効状態, volume = 現在の音量値
	// 副作用：ボリューム調整用のスライダーバーおよび現在値の描画
	void DrawVolumeRow(int y, const char* label, bool enabled, int volume);

	// 入力：rect = 描画領域, label = ボタン名, hover = ホバー判定フラグ
	// 副作用：ボタンのインタラクティブな描画（マウスホバー時のハイライト等）
	void DrawButton(const Rect& rect, const char* label, bool hover);

	// 入力：rect = 領域, mouseX/Y = 現在のマウス座標
	// 出力：領域内にマウスがあれば true
	bool IsMouseInRect(const Rect& rect, int mouseX, int mouseY) const;

	// 入力：rect = バーの領域, mouseX = マウスX座標
	// 出力：0-255の範囲にスケーリングされた音量値
	int GetVolumeFromMouseX(const Rect& rect, int mouseX) const;

	// 各UI操作領域の定義
	Rect bgm_toggle_rect_;
	Rect bgm_bar_rect_;
	Rect se_toggle_rect_;
	Rect se_bar_rect_;
	Rect debug_toggle_rect_;
	Rect back_rect_;

	// 演出：画面明滅等のブリンク処理用パラメータ
	int color_fade_;
	bool color_flag_;
};
