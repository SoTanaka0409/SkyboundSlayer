#pragma once
#include"DxLib.h"
#include"Model.h"
#include"TitleScene.h"
//クラスの前方宣言
class Object3D;

class Camera
{
public:
	Camera();//コンストラクタ

	~Camera();//デストラクタ


	void Initialize();//初期化

	void Finalize();

	void Update();//更新

	void UpdateRotation();//回転処理

	VECTOR GetPosition() { return mvPosition; }//座標取得
	VECTOR GetlookAtPosition() { return mvLookAtPosition; }//注視点取得

	float GetLookCamera() { return mfVerticalAngle; }
	void SetLookCamera(float hor) { mfVerticalAngle = hor; }

	VECTOR GetLookCamera2() { return dir; }//VECTORバージョン
	void SetLookCamera2(VECTOR dir2) { dir = dir2; }

	void SetCamera1(bool camera1) { Camera1 = camera1; }
	bool GetCamera1() { return Camera1; }
	void SetCamera3(bool camera3) { Camera3 = camera3; }
	bool GetCamera3() { return Camera3; }

	// ★New★
	// 画面揺れ
	void Shake();
	void SetupShake(float time, float width, float angleSpeed, float stepTime = 1.0f);

private:
	float mfHorizontalAngle;  //水平方向アングル
	float mfVerticalAngle;   // 垂直方向アングル

	VECTOR mvPosition; //カメラ座標
	VECTOR mvLookAtPosition;   //カメラの注移転座標   座標固定 (今回は)
	VECTOR dir;

	Object3D* mpTarget;    //カメラを向ける対象
	Model* mpModel;


	int centerX = 640;
	int centerY = 360;
	const float ROTATE_SPEED = 0.2f;//回転速度
	float mfTargetAngle;//目標の回転地
	float mfAngle;//現在の回転地

	bool Camera3;//三人称視点
	bool Camera1;//一人称視点

	int mnShakeTime;
	int mnShakeTimeCount;

	float mfShakeAngle;
	float mfShakeTimeCounter;
	float mfShakeTime;
	float mfShakeWidth;
	float mfShakeAngleSpeed;
	float mfStepTime;
	VECTOR mvShakePosition;
};