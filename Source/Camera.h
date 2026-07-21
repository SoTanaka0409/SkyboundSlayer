#pragma once
#include "DxLib.h"
#include "Model.h"
#include "TitleScene.h"

class Object3D;

/// @brief 三人称追従・カットシーン・画面揺れ（シェイク）等を管理する3Dカメラクラス
class Camera
{
public:
	Camera();
	~Camera();

	/// @brief カメラのパラメータおよび各種変数を初期化する
	void Initialize();

	/// @brief カメラの終了処理を行う
	void Finalize();

	/// @brief 毎フレームのカメラ座標・注視点の更新を行う
	void Update();

	/// @brief 三人称視点（TPS）用カメラの追従・位置計算を行う
	void UpdateThirdPersonCamera();

	/// @brief マウス入力等に基づくカメラの回転角計算を行う
	void UpdateRotation();

	/// @brief 現在のカメラのワールド座標を取得する
	/// @return VECTOR カメラの座標
	VECTOR GetPosition() { return position_; }

	/// @brief 現在のカメラの注視点座標を取得する
	/// @return VECTOR 注視点の座標
	VECTOR GetlookAtPosition() { return mvLookAtPosition; }

	/// @brief 垂直方向（仰俯角）の回転角度を取得する
	/// @return float 垂直角度
	float GetLookCamera() { return vertical_angle_; }

	/// @brief 水平方向の回転角度を加算する
	/// @param angle 加算する角度
	void AddHorizontalAngle(float angle) { horizontal_angle_ += angle; }

	/// @brief 垂直方向の回転角度を設定する
	/// @param hor 設定する垂直角度
	void SetLookCamera(float hor) { vertical_angle_ = hor; }

	/// @brief カメラの視線方向ベクトルを取得する
	/// @return VECTOR 視線方向ベクトル
	VECTOR GetLookCamera2() { return dir; }

	/// @brief カメラの視線方向ベクトルを設定する
	/// @param dir2 設定する視線方向ベクトル
	void SetLookCamera2(VECTOR dir2) { dir = dir2; }

	/// @brief カットシーンモードの有効/無効を切り替える
	/// @param isCutscene 有効にする場合はtrue
	void SetCutsceneMode(bool isCutscene) { is_cutscene_mode_ = isCutscene; }

	/// @brief カットシーン時に注視するターゲット座標を設定する
	/// @param pos 注視するワールド座標
	void SetCutsceneTarget(VECTOR pos) { mCutsceneTargetPos = pos; }

	/// @brief 画面揺れ（カメラシェイク）のオフセット座標を計算する
	void Shake();

	/// @brief 画面揺れ（カメラシェイク）のパラメータを設定・初期化する
	/// @param time 揺れの継続時間
	/// @param width 揺れの振幅（幅）
	/// @param angleSpeed 周期回転速度
	/// @param stepTime 時間経過係数（デフォルト: 1.0f）
	void SetupShake(float time, float width, float angleSpeed, float stepTime = 1.0f);

private:
	/// @brief ターゲット座標に基づき注視点（LookAt）座標を更新する
	/// @param targetPos 基準となるターゲット座標
	void UpdateLookAtPosition(VECTOR targetPos);

	/// @brief 計算された角度と距離に基づきカメラの位置座標を更新する
	void UpdateCameraPosition();

	// === 定数（感度や距離以外の不変なもの） ===
	static constexpr float kTargetOffsetY = 240.0f;   ///< ターゲット注視点のY軸オフセット量
	static constexpr float kDefaultOffsetY = 160.0f;  ///< 通常時のカメラY軸オフセット量
	static constexpr float kCameraCalcRadius = 400.0f; ///< カメラ位置計算用の球体半径（距離）

	// ※ kMouseSensitivity と kCameraDistance は設定から即時取得するため定数から外します

	bool is_cutscene_mode_ = false;       ///< カットシーンモード中かどうかのフラグ
	VECTOR mCutsceneTargetPos;            ///< カットシーン時に注視するターゲット座標
	float horizontal_angle_;              ///< 水平方向の回転角度（方位角）
	float vertical_angle_;                ///< 垂直方向の回転角度（仰俯角）

	VECTOR position_;                     ///< カメラのワールド位置座標
	VECTOR mvLookAtPosition;              ///< カメラの注視点ワールド座標
	VECTOR dir;                           ///< カメラの視線方向ベクトル

	Object3D* target_;                    ///< 追従対象の3Dオブジェクトへのポインタ
	Model* model_;                        ///< 関連モデルへのポインタ

	int centerX = 640;                    ///< マウスカーソル固定・差分計算基準の画面中央X座標
	int centerY = 360;                    ///< マウスカーソル固定・差分計算基準の画面中央Y座標
	const float ROTATE_SPEED = 0.2f;      ///< カメラの回転補間速度定数
	float target_angle_;                  ///< 補間目標とする回転角度
	float angle_;                         ///< 現在の補間計算用回転角度

	float shake_angle_;                   ///< シェイク計算用サイン波の現在角度
	float shake_time_counter_;            ///< シェイク処理の経過時間カウンタ
	float shake_time_;                    ///< シェイクの総継続時間
	float shake_width_;                   ///< シェイクの振幅（揺れ幅）
	float shake_angle_speed_;             ///< シェイクの周期速度
	float step_time_;                     ///< シェイクのステップ進行時間
	VECTOR mvShakePosition;               ///< カメラ座標に加算されるシェイク用のオフセットベクトル
};