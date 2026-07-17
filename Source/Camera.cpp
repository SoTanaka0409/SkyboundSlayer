#include"Camera.h"
#include"Config.h"
#include<cmath>
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Object3D.h"
#include"Scene.h"



/*
 * 目的（Cameraのコンストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] メンバ変数の初期化
 */
Camera::Camera()
	:horizontal_angle_(0.0f)
	,vertical_angle_(0.0f)
	,position_(VGet(0.0f,0.0f,0.0f))
	,mvLookAtPosition(VGet(0.0f,0.0f,0.0f))
	,target_(nullptr)
	, shake_angle_(0.0f)
	, shake_time_counter_(0.0f)
	, shake_time_(0.0f)
	, shake_width_(0.0f)
	, shake_angle_speed_(0.0f)
	, step_time_(0.0f)
	, mvShakePosition(VGet(0.0f, 0.0f, 0.0f))
{
}


/*
 * 目的（Cameraのデストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
Camera::~Camera()
{
	
}


/*
 * 目的（カメラの初期設定を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] 初期座標と回転のリセット
 */
void Camera::Initialize()
{
	target_ = nullptr;
	SetCameraNearFar(100.0f, Config::CameraFar);//10050000???????????????

	SetBackgroundColor(0, 0, 0);

	SetCameraPositionAndTarget_UpVecY(position_, mvLookAtPosition);

	Update();

	
}


/*
 * 目的（毎フレームのカメラ状態を更新するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 揺れや追従状態の計算
 */
void Camera::Update()
{
	UpdateThirdPersonCamera();
}


/*
 * 目的（3人称視点カメラの更新を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] プレイヤー座標に基づく追従処理、コリジョン判定
 */
void Camera::UpdateThirdPersonCamera()
{
	VECTOR targetPos = VGet(0,0,0);
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

/*
 * 目的（CameraのUpdateLookAtPosition処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Camera::UpdateLookAtPosition(VECTOR targetPos)
{
	if (is_cutscene_mode_ || target_ != nullptr)
	{
		mvLookAtPosition = targetPos;
		mvLookAtPosition.y += kTargetOffsetY;
	}
	else
	{
		//ターゲットがない場合は一定の高さ
		mvLookAtPosition.y = kDefaultOffsetY;
	}
}

/*
 * 目的（CameraのUpdateCameraPosition処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Camera::UpdateCameraPosition()
{
	VECTOR temp; //汎用変数
	temp.x = kCameraCalcRadius * cosf(vertical_angle_ / 180.0f * (3.1415926535897932384626433832795f)) * sinf(horizontal_angle_ / 180.0f * DX_PI_F);
	temp.y = kCameraCalcRadius * sinf(-vertical_angle_ / 180.0f * (3.1415926535897932384626433832795f));
	temp.z = -(kCameraDistance * cosf(vertical_angle_ / 180.0f * DX_PI_F) * cosf(horizontal_angle_ / 180.0f * DX_PI_F));
	position_ = VAdd(temp, mvLookAtPosition);

	SetCameraPositionAndTarget_UpVecY(VAdd(position_, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
}


/*
 * 目的（マウス入力によるカメラ回転を更新するため）
 * [入力] なし
 * [出力] なし
 * [副作用] target_rot_の更新
 */
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
	
	if (!CheckHitKey(KEY_INPUT_0))
	{
		int centerX = 640;
		int centerY = 360;
	}
	
	auto sceneType = Master::scene_manager_->GetCurrentSceneType();
	if (sceneType == SceneManager::kScene3D) {
		SetMousePoint(centerX, centerY);

		int deltaX = mouseX - centerX;
		int deltaY = mouseY - centerY;

		horizontal_angle_ -= deltaX * kMouseSensitivity;
		vertical_angle_ -= deltaY * kMouseSensitivity;
	}
}

/*
 * 目的（カメラの揺れ（シェイク）計算を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] 一時的なカメラ座標のオフセット
 */
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


/*
 * 目的（カメラの揺れパラメータを設定するため）
 * [入力] float time, float width, float angleSpeed, float stepTime
 * [出力] なし
 * [副作用] シェイク用変数の初期化
 */
void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)
{
	shake_time_counter_ = 0.0f;
	shake_time_ = time;
	shake_width_ = width;
	shake_angle_speed_ = angleSpeed;
	step_time_ = stepTime;
}


/*
 * 目的（カメラの終了処理を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void Camera::Finalize()
{



}




