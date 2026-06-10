#pragma once
#include"DxLib.h"

//クラスの前方宣言
class Object3D;

class Camera1
{
public:
	Camera1();//コンストラクタ

	~Camera1();//デストラクタ


	void Initialize();//初期化

	void Update();//更新

	void UpdateRotation();//回転処理

	VECTOR GetPosition() { return mvPosition; }//座標取得
	VECTOR GetlookAtPosition() { return mvLookAtPosition; }//注視点取得

private:
	float mfHorizontalAngle;  //水平方向アングル
	float mfVerticalAngle;   // 垂直方向アングル

	VECTOR mvPosition; //カメラ座標
	VECTOR mvLookAtPosition;   //カメラの注移転座標   座標固定 (今回は)

	Object3D* mpTarget;    //カメラを向ける対象
	int centerX = 640;
	int centerY = 360;
};