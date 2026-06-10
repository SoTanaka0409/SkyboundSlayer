#pragma once
#include"DxLib.h"
#include<string>

class Collider;

class Object3D
{
public:
	enum Tag3D
	{
		None3D = 0,//設定なし
		Tag3D_Player3D = 2000,
		Tag3D_Wall3D = 2100,
		Tag3D_Enemy3D = 2200,
		Tag3D_Stage = 2300,
		Tag3D_Bush=2301,
		Tag3D_Bullet = 2400,
		Tag3D_Weapon = 2500,
		Tag3D_Dino = 2600,
		Tag_3D_Camera=2700,
		Tag_3D_Shield=2800,
		Tag3D_Obj=2900,
		Tag3D_DinoFriend=3000,
		Tag3D_Object = 3050,
		Tag3D_RideDino=3500,	
		//Tag3D_charactor=3600,
		//Tag3D_Object=3900,
	};
	enum StatusState
	{
		None_Status=0,
		Status_Attack,
		Status_Speed,
		Status_Hp,
		
	}state;
	// ★New!! Zソート用★
public: // ソート関数定義

	// Zソート用の設定
	void SetCameraDistance(float distance) { mfCurrentCameraDistance = distance; }

	// カメラ距離を基準にソートするためのオペレーター
	// note: カメラとの距離が離れるほどリストの手前に来るようにする（先に描画したい）
	struct CompareZOrder {
		bool operator()(Object3D* a, Object3D* b) const {
			return a->mfCurrentCameraDistance > b->mfCurrentCameraDistance;
		}
	};



public:
	Object3D(VECTOR initPos);

	~Object3D();

	virtual void Update();

	virtual void Draw();

	virtual void OnEnter(Collider* collider, Collider* check);
	virtual void OnTrigger(Collider* collider, Collider* check);
	virtual void OnExit(Collider* collider, Collider* check);
	//virtual void Callback() = 0;


public://ゲッター、セッターw
	void SetPosition(VECTOR pos) { mvPosition = pos; };
	VECTOR GetPosition() { return mvPosition; }

	void SetOldPosition(VECTOR pos) { mvOldPosition = pos; }
	VECTOR GetOldPosition() { return mvOldPosition; }

	void SetRotation(VECTOR rot) { mvRotation = rot; }
	VECTOR GETRotation() { return mvRotation; }

	void SetDeleteFlag(bool flag) { mbDeleteFlag = flag; }
	bool IsDeleteFlag() { return mbDeleteFlag; }

	void SetDrawFlag(bool flag) { mbDrawFlag = flag; } //敵の削除フラグ設定　
	bool IsDrawFlag() { return mbDrawFlag; }          //敵の削除フラグの作成

	//タグ
	void SetTag(Tag3D tag) { mnTag = tag; }
	Tag3D GetTag() { return mnTag; }

protected:
	VECTOR mvPosition;  //座標
	VECTOR mvRotation;   //回転
	VECTOR mvOldPosition;
private:
	bool mbDeleteFlag;  //削除フラグ
	Tag3D mnTag;   //タグ
	bool mbDrawFlag;//描画フラグ
	float mfCurrentCameraDistance;     // 現在のカメラとの距離
	
};