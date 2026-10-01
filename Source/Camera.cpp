#include "Camera.h"
#include "Config.h"
#include <cmath>
#include "Master.h"
#include "ObjectManager.h"
#include "GameScene.h"
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
	: m_HorizontalAngle(0.0f)
	, m_VerticalAngle(0.0f)
	, m_Position(VGet(0.0f, 0.0f, 0.0f))
	, mvLookAtPosition(VGet(0.0f, 0.0f, 0.0f))
	, m_Target(nullptr)
	, m_ShakeAngle(0.0f)
	, m_ShakeTimeCounter(0.0f)
	, m_ShakeTime(0.0f)
	, m_ShakeWidth(0.0f)
	, m_ShakeAngleSpeed(0.0f)
	, m_StepTime(0.0f)
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
	m_Target = nullptr;
	SetCameraNearFar(100.0f, Config::CameraFar);

	SetBackgroundColor(0, 0, 0);

	SetCameraPositionAndTarget_UpVecY(m_Position, mvLookAtPosition);

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
	if (m_IsCutsceneMode)
	{
		targetPos = mCutsceneTargetPos;
	}
	else if (m_Target != nullptr)
	{
		targetPos = m_Target->GetPosition();
	}

	if (m_Target == nullptr)
	{
		m_Target = Master::m_Player;
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
	if (m_IsCutsceneMode || m_Target != nullptr)
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
	temp.x = kCameraCalcRadius * cosf(m_VerticalAngle / 180.0f * (3.1415926535897932384626433832795f)) * sinf(m_HorizontalAngle / 180.0f * DX_PI_F);
	temp.y = kCameraCalcRadius * sinf(-m_VerticalAngle / 180.0f * (3.1415926535897932384626433832795f));
	temp.z = -(kCameraDistance * cosf(m_VerticalAngle / 180.0f * DX_PI_F) * cosf(m_HorizontalAngle / 180.0f * DX_PI_F));
	m_Position = VAdd(temp, mvLookAtPosition);

	SetCameraPositionAndTarget_UpVecY(VAdd(m_Position, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
}

/// @brief マウス入力によるカメラの角度更新処理を行う
/// @details 水平・垂直角度のクランプ処理およびマウス差分による回転加算を行う
void Camera::UpdateRotation()
{
	if (m_HorizontalAngle >= 180.0f)
	{
		m_HorizontalAngle -= 360.0f;
	}
	if (m_HorizontalAngle <= -180.0f)
	{
		m_HorizontalAngle += 360.0f;
	}

	if (m_VerticalAngle >= 80.0f)
	{
		m_VerticalAngle = 80.0f;
	}

	if (m_VerticalAngle <= -80.0f)
	{
		m_VerticalAngle = -80.0f;
	}

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	int centerX = 640;
	int centerY = 360;

	auto sceneType = Master::m_SceneManager->GetCurrentSceneType();
	if (sceneType == SceneManager::kGameScene && !Master::m_IsPauseOn) {
		SetMousePoint(centerX, centerY);

		int deltaX = mouseX - centerX;
		int deltaY = mouseY - centerY;

		m_HorizontalAngle -= deltaX * kMouseSensitivity;
		m_VerticalAngle -= deltaY * kMouseSensitivity;
	}
}

/// @brief カメラの揺れ（シェイク）オフセット量を計算する
/// @details 一時的なカメラ座標のオフセット（mvShakePosition）を算出する
void Camera::Shake()
{
	if (m_ShakeTimeCounter < m_ShakeTime)
	{
		mvShakePosition.y = sinf(m_ShakeAngle) * (1.0f - (m_ShakeTimeCounter / m_ShakeTime)) * m_ShakeWidth;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		m_ShakeAngle += m_ShakeAngleSpeed * m_StepTime;

		m_ShakeTimeCounter += m_StepTime;
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
	m_ShakeTimeCounter = 0.0f;
	m_ShakeTime = time;
	m_ShakeWidth = width;
	m_ShakeAngleSpeed = angleSpeed;
	m_StepTime = stepTime;
}

/// @brief カメラの終了処理を行う
void Camera::Finalize()
{
}