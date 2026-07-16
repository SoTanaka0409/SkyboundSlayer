#include"Camera.h"
#include"Config.h"
#include<cmath>
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Object3D.h"
#include"Scene.h"


Camera::Camera()

	:mfHorizontalAngle(0.0f)
	,mfVerticalAngle(0.0f)
	,position_(VGet(0.0f,0.0f,0.0f))
	,mvLookAtPosition(VGet(0.0f,0.0f,0.0f))
	,target_(nullptr)
	, mnShakeTime(0)
	, mnShakeTimeCount(0)
	, mfShakeAngle(0.0f)
	, mfShakeTimeCounter(0.0f)
	, mfShakeTime(0.0f)
	, mfShakeWidth(0.0f)
	, mfShakeAngleSpeed(0.0f)
	, mfStepTime(0.0f)
	, mvShakePosition(VGet(0.0f, 0.0f, 0.0f))
{
}

Camera::~Camera()
{
	
}

void Camera::Initialize()
{
	target_ = nullptr;
	//?J??????N???b?s???O????????
	SetCameraNearFar(100.0f, Config::CameraFar);//10050000???????????????

	//?w?i?F????i?D?F?j
	SetBackgroundColor(0, 0, 0);

	//?J??????????f
	SetCameraPositionAndTarget_UpVecY(position_, mvLookAtPosition);

	//?X?V???????x?s???????
	Update();

	
}

void Camera::Update()
{
	UpdateThirdPersonCamera();
}

void Camera::UpdateThirdPersonCamera()
{
	VECTOR targetPos = VGet(0,0,0);
	if (mIsCutsceneMode)
	{
		targetPos = mCutsceneTargetPos;
	} 
	else if (target_ != nullptr) 
	{
		targetPos = target_->GetPosition();
	}

	VECTOR temp; //汎用変数
	if (target_ == nullptr)
	{
		target_ = Master::mpPlayer;
	}

	UpdateRotation();

	if (mIsCutsceneMode || target_ != nullptr)
	{
		mvLookAtPosition = targetPos;
		mvLookAtPosition.y += 240.0f;
	}
	else
	{
		//ターゲットがない場合は一定の高さ
		mvLookAtPosition.y = 160.0f;
	}

	Shake();
	{
		const float distance = 500.0f;
		temp.x = 400.0f * cosf(mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f)) * sinf(mfHorizontalAngle / 180.0f * DX_PI_F);
		temp.y = 400.0f * sinf(-mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f));
		temp.z = -(distance * cosf(mfVerticalAngle / 180.0f * DX_PI_F) * cosf(mfHorizontalAngle / 180.0f * DX_PI_F));
		position_ = VAdd(temp, mvLookAtPosition);

		SetCameraPositionAndTarget_UpVecY(VAdd(position_, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
	}
}

void Camera::UpdateRotation()
{
	////?????L?[??J????????
		if (mfHorizontalAngle >= 180.0f)
		{
			mfHorizontalAngle -= 360.0f;
		}
		if (mfHorizontalAngle <= -180.0f)
		{
			mfHorizontalAngle += 360.0f;
		}

		if (mfVerticalAngle >= 80.0f)
		{
			mfVerticalAngle = 80.0f;
		}

		if (mfVerticalAngle <= -80.0f)
		{
			mfVerticalAngle = -80.0f;
		}
	

	float camAngleY = 0.0f; // ?????????i???E?j
	float camAngleX = 0.0f; // ?????????i???j

	// ???x
	const float MOUSE_SENSITIVITY = 0.05f;

	// ?J?????????
	float camDistance = 300.0f;

	

	// ?J?[?\?????\????

		// ?}?E?X???????擾
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	// ???S???W
	if (!CheckHitKey(KEY_INPUT_0))
	{
		int centerX = 640;
		int centerY = 360;
	}
	auto sceneType = Master::mpSceneManager->GetCurrentSceneType();
	if (sceneType == SceneManager::kScene3D) {
		SetMousePoint(centerX, centerY);

	

		int deltaX = mouseX - centerX;
		int deltaY = mouseY - centerY;

		// ??]?p?x??X?V
		mfHorizontalAngle -= deltaX * MOUSE_SENSITIVITY;
		mfVerticalAngle -= deltaY * MOUSE_SENSITIVITY;
	}

		// ?????]?????i?????????????????j
		/*if (camAngleX < -DX_PI_F / 2.0f) camAngleX = -DX_PI_F / 2.0f;
		if (camAngleX > DX_PI_F / 2.0f) camAngleX = DX_PI_F / 2.0f;*/

		//// ?J???????u??????_??v?Z
		//VECTOR temp = VGet(
		//	camTarget.x + camDistance * sinf(camAngleY) * cosf(camAngleX),
		//	camTarget.y + camDistance * sinf(camAngleX),
		//	camTarget.z + camDistance * cosf(camAngleY) * cosf(camAngleX)

		

		
}
// ??New??
// ???h??
void Camera::Shake()
{
	if (mfShakeTimeCounter < mfShakeTime)
	{
		// sinf ???p????h?????W??Z?o
		// note: ??UY???W??????h??????
		mvShakePosition.y = sinf(mfShakeAngle) * (1.0f - (mfShakeTimeCounter / mfShakeTime)) * mfShakeWidth;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		// ?h????????g?p???? sinf ??n???p?x???X????
		mfShakeAngle += mfShakeAngleSpeed * mfStepTime;

		// ?h???????o???????
		mfShakeTimeCounter += mfStepTime;
	}
	else
	{
		// ?h?炳???????????h????????????Z???W??O?????
		mvShakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

// ??New??
// ???h????
void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)
{
	mfShakeTimeCounter = 0.0f;
	mfShakeTime = time;
	mfShakeWidth = width;
	mfShakeAngleSpeed = angleSpeed;
	mfStepTime = stepTime;
}

void Camera::Finalize()
{



}




