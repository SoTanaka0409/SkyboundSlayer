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
	,mvPosition(VGet(0.0f,0.0f,0.0f))
	,mvLookAtPosition(VGet(0.0f,0.0f,0.0f))
	,mpTarget(nullptr)
	,Camera1(true)
	,Camera3(false)
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
	Camera1 = true;
}

Camera::~Camera()
{
	
}

void Camera::Initialize()
{
	mpTarget = nullptr;
	//カメラのクリッピング距離の設定
	SetCameraNearFar(100.0f, Config::CameraFar);//10050000までの距離が見える

	//背景色を設定（灰色）
	SetBackgroundColor(128, 128, 128);

	//カメラの設定を反映
	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);

	//更新処理を一度行っていく
	Update();

	
}

void Camera::Update()
{
	/*if (mpTitleScene->GetResetCameraPlayer() == true)
	{
		mpTarget = nullptr;
		mpTitleScene->ResetCameraPlayer(false);
	}*/
	
	VECTOR temp; //作業用変数
	if (mpTarget == nullptr)
	{
		mpTarget = Master::mpPlayer;

	}
	if(Camera1==true)
	{
		// ★New★
   // 画面揺れ処理
		Shake();
		Camera3 = false;
		const float distance = 300.0f;
		temp.x = 250.0f * cosf(mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f)) * sinf(mfHorizontalAngle / 180.0f * DX_PI_F);
		temp.y = 250.0f * sinf(mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f));
		temp.z = -(distance * cosf(mfVerticalAngle / 180.0f * DX_PI_F) * cosf(mfHorizontalAngle / 180.0f * DX_PI_F));
		dir = VGet(sinf(mfVerticalAngle), 0.0f, cosf(mfVerticalAngle));
		VECTOR dir2 = VGet(sinf(mfHorizontalAngle), 0.0f, cosf(mfHorizontalAngle));
		auto mpPlayer = Master::mpPlayer;
		Player3D* pPlayer = Master::mpPlayer;
		
		
		UpdateRotation();
		

		//ターゲットがいなかったら
		
		if (mpTarget != nullptr)
		{
			mvLookAtPosition = VAdd(mpTarget->GetPosition(), temp);
			mvPosition = VAdd(mpTarget->GetPosition(), dir);

			mvPosition.y += 160.0f + dir.x + dir.z;


		}
		else
		{
			//注視点を少し上にずらす
			mvPosition.y = 160.0f;
		}

		// ★New★
		// 画面揺れの分を加算するように変更
		// カメラ設定を反映
		SetCameraPositionAndTarget_UpVecY(VAdd(mvPosition, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
		//回転設定
	}
	if (Camera3 == true)
	{
		Camera1 = false;
		
		
		//上で求めた座標に注視点の座標を足したものがカメラの座標となる
		
		
		
		auto mpPlayer = Master::mpPlayer;
		Player3D* pPlayer = Master::mpPlayer;
		
		UpdateRotation();
		//ターゲットがいなかったら
		/*if (mpTarget == nullptr)
		{
			mpTarget = Master::mpPlayer;
		}*/
		if (mpTarget != nullptr)
		{
			mvLookAtPosition = mpTarget->GetPosition();
			mvLookAtPosition.y += 240.0f;
			
		}
		else
		{
			//注視点を少し上にずらす
			mvLookAtPosition.y = 160.0f;
		}
		// ★New★
   // 画面揺れ処理
		Shake();
		{
			VECTOR temp; //作業用変数
			const float distance = 500.0f;
			temp.x = 400.0f * cosf(mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f)) * sinf(mfHorizontalAngle / 180.0f * DX_PI_F);
			temp.y = 400.0f * sinf(-mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f));
			temp.z = -(distance * cosf(mfVerticalAngle / 180.0f * DX_PI_F) * cosf(mfHorizontalAngle / 180.0f * DX_PI_F));
			mvPosition = VAdd(temp, mvLookAtPosition);
			/*mvLookAtPosition = VSub(mpTarget->GetPosition(), mvPosition);
			mvLookAtPosition = VNorm(mvLookAtPosition);*/
			// ★New★
		// 画面揺れの分を加算するように変更
		 // カメラ設定を反映
			SetCameraPositionAndTarget_UpVecY(VAdd(mvPosition, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));

			//上で求めた座標に注視点の座標を足したものがカメラの座標となる
			//カメラ設定を反映
		}
	}
	
}

void Camera::UpdateRotation()
{
	////方向キーでカメラ操作
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
	

	float camAngleY = 0.0f; // 水平方向（左右）
	float camAngleX = 0.0f; // 垂直方向（上下）

	// 感度
	const float MOUSE_SENSITIVITY = 0.05f;

	// カメラの距離
	float camDistance = 300.0f;

	

	// カーソルを非表示に

		// マウスの移動量を取得
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	// 中心座標
	if (!CheckHitKey(KEY_INPUT_0))
	{
		int centerX = 640;
		int centerY = 360;
	}
	SetMousePoint(centerX, centerY);

	

		int deltaX = mouseX - centerX;
		int deltaY = mouseY - centerY;

		// 回転角度を更新
		mfHorizontalAngle -= deltaX * MOUSE_SENSITIVITY;
		mfVerticalAngle -= deltaY * MOUSE_SENSITIVITY;

		// 上下の回転を制限（視線が裏返らないように）
		/*if (camAngleX < -DX_PI_F / 2.0f) camAngleX = -DX_PI_F / 2.0f;
		if (camAngleX > DX_PI_F / 2.0f) camAngleX = DX_PI_F / 2.0f;*/

		//// カメラの位置と注視点を計算
		//VECTOR temp = VGet(
		//	camTarget.x + camDistance * sinf(camAngleY) * cosf(camAngleX),
		//	camTarget.y + camDistance * sinf(camAngleX),
		//	camTarget.z + camDistance * cosf(camAngleY) * cosf(camAngleX)

		

		
}
// ★New★
// 画面揺れ
void Camera::Shake()
{
	if (mfShakeTimeCounter < mfShakeTime)
	{
		// sinf を利用して揺らし座標を算出
		// note: 一旦Y座標だけを揺らしてみる
		mvShakePosition.y = sinf(mfShakeAngle) * (1.0f - (mfShakeTimeCounter / mfShakeTime)) * mfShakeWidth;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		// 揺らし処理に使用する sinf に渡す角度の変更処理
		mfShakeAngle += mfShakeAngleSpeed * mfStepTime;

		// 揺らす時間を経過させる
		mfShakeTimeCounter += mfStepTime;
	}
	else
	{
		// 揺らされていない場合は揺らし処理による加算座標を０にする
		mvShakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

// ★New★
// 画面揺れ設定
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