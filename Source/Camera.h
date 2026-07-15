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

	void Update(); // 更新
	void UpdateThirdPersonCamera(); // 三人称カメラ更新処理

	void UpdateRotation();//回転処理

	VECTOR GetPosition() { return position_; }//座標取得
	VECTOR GetlookAtPosition() { return mvLookAtPosition; }//注視点取得

	float GetLookCamera() { return mfVerticalAngle; }
	void AddHorizontalAngle(float angle) { mfHorizontalAngle += angle; }
	void SetLookCamera(float hor) { mfVerticalAngle = hor; }

	VECTOR GetLookCamera2() { return dir; }//VECTORバージョン
	void SetLookCamera2(VECTOR dir2) { dir = dir2; }	void SetCutsceneMode(bool isCutscene) { mIsCutsceneMode = isCutscene; }
	void SetCutsceneTarget(VECTOR pos) { mCutsceneTargetPos = pos; }

	// ★New★
	// 画面揺れ
	void Shake();
	void SetupShake(float time, float width, float angleSpeed, float stepTime = 1.0f);

private:
	bool mIsCutsceneMode = false;
	VECTOR mCutsceneTargetPos = VGet(0.0f, 0.0f, 0.0f);
	float mfHorizontalAngle;  //水平方向アングル
	float mfVerticalAngle;   // 垂直方向アングル

	VECTOR position_; //カメラ座標
	VECTOR mvLookAtPosition;   //カメラの注移転座標   座標固定 (今回は)
	VECTOR dir;

	Object3D* target_;    //カメラを向ける対象
	Model* model_;


	int centerX = 640;
	int centerY = 360;
	const float ROTATE_SPEED = 0.2f;//回転速度
	float mfTargetAngle;//目標の回転地
	float mfAngle;//現在の回転地

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



