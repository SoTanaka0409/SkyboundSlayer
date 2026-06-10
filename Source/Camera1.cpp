#include"Camera1.h"
#include<cmath>
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Object3D.h"
#include"SceneManager.h"
#include"Player3D.h"
#include"Scene.h"

Camera1::Camera1()
	:mfHorizontalAngle(0.0f)
	, mfVerticalAngle(0.0f)
	, mvPosition(VGet(0.0f, 0.0f, 0.0f))
	, mvLookAtPosition(VGet(0.0f, 0.0f, 0.0f))
	, mpTarget(nullptr)
{


}

Camera1::~Camera1()
{

}

void Camera1::Initialize()
{
	//カメラのクリッピング距離の設定
	SetCameraNearFar(100.0f, 50000.0f);//100～50000までの距離が見える

	//背景色を設定（灰色）
	SetBackgroundColor(128, 128, 128);

	//カメラの設定を反映
	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);

	//更新処理を一度行っていく
	Update();

}

void Camera1::Update()
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);
	UpdateRotation();
	//ターゲットがいなかったら
	if (mpTarget == nullptr)
	{
		mpTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	}
	if (mpTarget != nullptr)
	{
		mvLookAtPosition = mpTarget->GetPosition();
		mvLookAtPosition.y += 80.0f;
	}
	else
	{
		//注視点を少し上にずらす
		mvLookAtPosition.y = 80.0f;
	}

	{
		VECTOR temp; //作業用変数


		mvLookAtPosition = VSub(pPlayer->GetPosition() , mvPosition);
		mvLookAtPosition = VNorm(mvLookAtPosition);

		//上で求めた座標に注視点の座標を足したものがカメラの座標となる
		//mvPosition = VAdd(temp, mvLookAtPosition);
		//mvPosition = VAdd(temp, mvLookAtPosition);
		//カメラ設定を反映
		SetCameraPositionAndTarget_UpVecY(pPlayer->GetPosition(), mvLookAtPosition);
	}

}

void Camera1::UpdateRotation()
{
	////方向キーでカメラ操作
	//if (CheckHitKey(KEY_INPUT_LEFT))
	//{
	//	mfHorizontalAngle += 5.0f;
	if (mfHorizontalAngle >= 180.0f)
	{
		mfHorizontalAngle -= 360.0f;
	}
	//}
	//if (CheckHitKey(KEY_INPUT_RIGHT))
	//{
	//	mfHorizontalAngle -= 5.0f;
	if (mfHorizontalAngle <= -180.0f)
	{
		mfHorizontalAngle += 360.0f;
	}
	//}
	//if (CheckHitKey(KEY_INPUT_UP))
	//{
	//	mfVerticalAngle += 5.0f;

	if (mfVerticalAngle >= 80.0f)
	{
		mfVerticalAngle = 80.0f;
	}
	//}
	//if (CheckHitKey(KEY_INPUT_DOWN))
	//{
	//	mfVerticalAngle -= 5.0f;

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
	//SetMouseDispFlag(FALSE);

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
	mfVerticalAngle += deltaY * MOUSE_SENSITIVITY;

	// 上下の回転を制限（視線が裏返らないように）
	/*if (camAngleX < -DX_PI_F / 2.0f) camAngleX = -DX_PI_F / 2.0f;
	if (camAngleX > DX_PI_F / 2.0f) camAngleX = DX_PI_F / 2.0f;*/

	//// カメラの位置と注視点を計算
	//VECTOR camTarget = VGet(0.0f, 0.0f, 0.0f);
	//VECTOR temp = VGet(
	//	camTarget.x + camDistance * sinf(camAngleY) * cosf(camAngleX),
	//	camTarget.y + camDistance * sinf(camAngleX),
	//	camTarget.z + camDistance * cosf(camAngleY) * cosf(camAngleX)
	//);


}