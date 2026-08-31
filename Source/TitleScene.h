#pragma once
#include "dxlib.h"
#include "Scene.h"

// アーキテクチャ設計：タイトル画面を担当するシーンクラス。
// ゲームの第一印象を決定づける場所として、3D背景の旋回演出や、操作を促す点滅UIなどを統合的に制御する。
class TitleScene : public Scene
{
public:
/// @details 点滅演出用タイマーおよびカメラ角度の初期化
	TitleScene();

	~TitleScene();

/// @details カメラリセット、BGMの開始、ステージ背景および天球モデルの動的ロードと生成
	void Initialize() override;

/// @details カメラ旋回アニメーションの進行およびメニューUIへのマウス入力判定の毎フレーム実行
	void Update() override;

/// @details 背景、タイトルロゴ、メニューパネル、および点滅テキストの描画命令発行
	void Draw() override;

/// @details タイトルシーン専用に確保した動的リソースの解放
	void Finalize() override;

private:
/// @details カメラ位置の三角関数計算および強制注視点設定による旋回演出
	void UpdateTitleCamera();

/// @details マウス座標と各ボタン領域の判定に基づいた、シーン遷移（Game
	void HandleMenuInput();

/// @details 点滅演出用アルファ値の周期的な変動（ブリンク演出）
	void UpdatePromptBlink();

/// @details 3D背景の描画およびUI可読性を高めるための半透明な暗幕レイヤーの重ね描き
	void DrawSceneBackground();

/// @details タイトルロゴの描画（影による可読性向上処理を含む）
	void DrawTitlePanel();

/// @details マウスホバー判定に合わせた、各メニューボタンのカラー変更と描画
	void DrawMenuPanel();

/// @details 点滅用のアルファ値を適用した操作プロンプト（Press Start等）の描画
	void DrawPrompt();

/// @param mx, my = マウス座標
/// @return 指定ボタン領域内であれば true
/// @details なし（ボタン領域に対する静的ヒットボックス判定）
	bool IsHoverStart(int mx, int my) const;
	bool IsHoverRule(int mx, int my) const;
	bool IsHoverSettings(int mx, int my) const;

	// カメラ演出用：自動旋回アニメーションの現在の回転角
	float camera_angle_;

	// UI演出用：点滅処理のためのアルファ値（fade）と、明滅方向を制御するトグルフラグ
	int color_fade_;
	bool color_flag_;
};