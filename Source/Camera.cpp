#include "Camera.h"
#include "Config.h"
#include <cmath>
#include "Master.h"
#include "ObjectManager.h"
#include "Scene3D.h"
#include "Object3D.h"
#include "Player3D.h"
#include "Scene.h"

namespace {
	constexpr float kCameraDistance = 300.0f;     ///< カメラと注視点の基本距離
	constexpr float kMouseSensitivity = 0.05f;    ///< マウス移動によるカメラ回転感度
	constexpr float kCameraCalcRadius = 400.0f;    ///< カメラ位置計算用の球体半径
	constexpr float kTargetOffsetY = 240.0f;      ///< ターゲット注視点のY軸オフセット量
	constexpr float kDefaultOffsetY = 160.0f;     ///< 通常時のカメラ注視点Y軸オフセット量
}

/// @brief Cameraのコンストラクタ
/// @details メンバ変数の初期化を行う
Camera::Camera()
	: horizontal_angle_(0.0f)
	, vertical_angle_(0.0f)
	, position_(VGet(0.0f, 0.0f, 0.0f))
	, mvLookAtPosition(VGet(0.0f, 0.0f, 0.0f))
	, target_(nullptr)
	, shake_angle_(0.0f)
	, shake_time_counter_(0.0f)
	, shake_time_(0.0f)
	, shake_width_(0.0f)
	, shake_angle_speed_(0.0f)
	, step_time_(0.0f)
	, mvShakePosition(VGet(0.0f, 0.0f, 0.0f))
{
}

/// @brief Cameraのデストラクタ
Camera::~Camera()
{
}

/// @brief カメラの初期設定を行う
/// @details 初期座標および回転のリセット、描画クリッピング距離の設定を行う
void Camera::Initialize()
{
	target_ = nullptr;
	SetCameraNearFar(100.0f, Config::CameraFar);

	SetBackgroundColor(0, 0, 0);

	SetCameraPositionAndTarget_UpVecY(position_, mvLookAtPosition);

	Update();
}

/// @brief 毎フレームのカメラ状態を更新する
/// @details 三人称追従処理を呼び出す
void Camera::Update()
{
	UpdateThirdPersonCamera();
}

/// @brief 三人称視点カメラの更新処理を行う
/// @details 追従対象の座標に基づき回転・注視点・シェイク・最終位置を再計算する
void Camera::UpdateThirdPersonCamera()
{
	VECTOR targetPos = VGet(0, 0, 0);
	if (is_cutscene_mode_)
	{
		targetPos = mCutsceneTargetPos;
	}
	else if (target_ != nullptr)
	{
		targetPos = target_->GetPosition();
	}

	if (target_ == nullptr)
	{
		target_ = Master::player_;
	}

	UpdateRotation();
	UpdateLookAtPosition(targetPos);
	Shake();
	UpdateCameraPosition();
}

/// @brief 注視点（LookAt）座標の更新処理を行う
/// @param targetPos 基準となるターゲット座標
void Camera::UpdateLookAtPosition(VECTOR targetPos)
{
	if (is_cutscene_mode_ || target_ != nullptr)
	{
		mvLookAtPosition = targetPos;
		mvLookAtPosition.y += kTargetOffsetY;
	}
	else
	{
		// ターゲットがない場合は一定の高さに固定
		mvLookAtPosition.y = kDefaultOffsetY;
	}
}

/// @brief カメラの最終ワールド座標を計算してDxLibへ設定する
void Camera::UpdateCameraPosition()
{
	VECTOR temp; // 計算用一時変数
	temp.x = kCameraCalcRadius * cosf(vertical_angle_ / 180.0f * (3.1415926535897932384626433832795f)) * sinf(horizontal_angle_ / 180.0f * DX_PI_F);
	temp.y = kCameraCalcRadius * sinf(-vertical_angle_ / 180.0f * (3.1415926535897932384626433832795f));
	temp.z = -(kCameraDistance * cosf(vertical_angle_ / 180.0f * DX_PI_F) * cosf(horizontal_angle_ / 180.0f * DX_PI_F));
	position_ = VAdd(temp, mvLookAtPosition);

	SetCameraPositionAndTarget_UpVecY(VAdd(position_, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
}

/// @brief マウス入力によるカメラの角度更新処理を行う
/// @details 水平・垂直角度のクランプ処理およびマウス差分による回転加算を行う
void Camera::UpdateRotation()
{
	if (horizontal_angle_ >= 180.0f)
	{
		horizontal_angle_ -= 360.0f;
	}
	if (horizontal_angle_ <= -180.0f)
	{
		horizontal_angle_ += 360.0f;
	}

	if (vertical_angle_ >= 80.0f)
	{
		vertical_angle_ = 80.0f;
	}

	if (vertical_angle_ <= -80.0f)
	{
		vertical_angle_ = -80.0f;
	}

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	int centerX = 640;
	int centerY = 360;

	auto sceneType = Master::scene_manager_->GetCurrentSceneType();
	if (sceneType == SceneManager::kScene3D) {
		SetMousePoint(centerX, centerY);

		int deltaX = mouseX - centerX;
		int deltaY = mouseY - centerY;

		horizontal_angle_ -= deltaX * kMouseSensitivity;
		vertical_angle_ -= deltaY * kMouseSensitivity;
	}
}

/// @brief カメラの揺れ（シェイク）オフセット量を計算する
/// @details 一時的なカメラ座標のオフセット（mvShakePosition）を算出する
void Camera::Shake()
{
	if (shake_time_counter_ < shake_time_)
	{
		mvShakePosition.y = sinf(shake_angle_) * (1.0f - (shake_time_counter_ / shake_time_)) * shake_width_;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		shake_angle_ += shake_angle_speed_ * step_time_;

		shake_time_counter_ += step_time_;
	}
	else
	{
		mvShakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

/// @brief カメラの揺れ（シェイク）パラメータを設定する
/// @param time 揺れの継続時間
/// @param width 揺れの振幅（幅）
/// @param angleSpeed 周期回転速度
/// @param stepTime 時間経過ステップ量
void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)
{
	shake_time_counter_ = 0.0f;
	shake_time_ = time;
	shake_width_ = width;
	shake_angle_speed_ = angleSpeed;
	step_time_ = stepTime;
}

/// @brief カメラの終了処理を行う
void Camera::Finalize()
{
}