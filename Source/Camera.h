#pragma once
#include"DxLib.h"
#include"Model.h"
#include"TitleScene.h"
//クラスの前方宣言
class Object3D;

class Camera
{
public:
    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
	Camera();//コンストラクタ

    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
    // デストラクタ
// [入力] なし [出力] なし [副作用] なし
	~Camera();//デストラクタ


    // カメラの初期設定を行う
// [入力] なし [出力] なし [副作用] カメラ座標と回転の初期化
	void Initialize();//初期化

    // カメラの終了処理を行う
// [入力] なし [出力] なし [副作用] なし
	void Finalize();

    // カメラの更新処理を行う
// [入力] なし [出力] なし [副作用] 揺れや追従の計算
	void Update(); // 更新
    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
    // 3人称視点カメラの更新を行う
// [入力] なし [出力] なし [副作用] プレイヤー座標に基づく追従処理
	void UpdateThirdPersonCamera(); // 三人称カメラ更新処理

    // マウス入力によるカメラ回転を更新する
// [入力] なし [出力] なし [副作用] target_rot_ の更新
	void UpdateRotation();//回転処理

    // カメラの現在座標を取得する
// [入力] なし [出力] VECTOR [副作用] なし
	VECTOR GetPosition() { return position_; }//座標取得
	VECTOR GetlookAtPosition() { return mvLookAtPosition; }//注視点取得

    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
	float GetLookCamera() { return vertical_angle_; }
	void AddHorizontalAngle(float angle) { horizontal_angle_ += angle; }
    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
	void SetLookCamera(float hor) { vertical_angle_ = hor; }

	VECTOR GetLookCamera2() { return dir; }//VECTORバージョン
	void SetLookCamera2(VECTOR dir2) { dir = dir2; }	void SetCutsceneMode(bool isCutscene) { is_cutscene_mode_ = isCutscene; }
	void SetCutsceneTarget(VECTOR pos) { mCutsceneTargetPos = pos; }

	// ★New★
	// 画面揺れ
    // カメラの揺れ（シェイク）計算を行う
// [入力] なし [出力] なし [副作用] draw_pos_ と draw_target_ の一時的なオフセット
	void Shake();
    // カメラの揺れパラメータを設定する
// [入力] float time, float width, float angleSpeed, float stepTime [出力] なし [副作用] シェイク変数の初期化
	void SetupShake(float time, float width, float angleSpeed, float stepTime = 1.0f);

private:
	// 注視点の計算処理
	void UpdateLookAtPosition(VECTOR targetPos);
	// カメラ座標の計算・設定処理
	void UpdateCameraPosition();

	// === 定数 ===
	static constexpr float kTargetOffsetY = 240.0f;
	static constexpr float kDefaultOffsetY = 160.0f;
	static constexpr float kCameraDistance = 500.0f;
	static constexpr float kCameraCalcRadius = 400.0f;
	static constexpr float kMouseSensitivity = 0.05f;

	bool is_cutscene_mode_ = false;
	VECTOR mCutsceneTargetPos = VGet(0.0f, 0.0f, 0.0f);
	float horizontal_angle_;  //水平方向アングル
	float vertical_angle_;   // 垂直方向アングル

	VECTOR position_; //カメラ座標
	VECTOR mvLookAtPosition;   //カメラの注移転座標   座標固定 (今回は)
	VECTOR dir;

	Object3D* target_;    //カメラを向ける対象
	Model* model_;


	int centerX = 640;
	int centerY = 360;
	const float ROTATE_SPEED = 0.2f;//回転速度
	float target_angle_;//目標の回転地
	float angle_;//現在の回転地

	float shake_angle_;
	float shake_time_counter_;
	float shake_time_;
	float shake_width_;
	float shake_angle_speed_;
	float step_time_;
	VECTOR mvShakePosition;
};
