#pragma once
#include<string>
#include"vector"
#include"Dxlib.h"


class Object2D
{
public:
	enum Tag2D
	{
		None2D = 0,//設定なし
		Tag2D_Dino1 = 1,
		Tag2D_Dino2=100,
		
	};

public:
	Object2D(VECTOR initPos);

	virtual ~Object2D();

	virtual void Update();

	virtual void Draw();

public://ゲッター、セッター
	void SetPosition(VECTOR pos) { mvPosition = pos; };
	VECTOR GetPosition() { return mvPosition; }

	void SetRotation(VECTOR rot) { mvRotation = rot; }
	VECTOR GETRotation() { return mvRotation; }

	void SetDeleteFlag(bool flag) { mbDeleteFlag = flag; }
	bool IsDeleteFlag() { return mbDeleteFlag; }

	void SetDrawFlag(bool flag) { mbDrawFlag = flag; } //敵の削除フラグ設定　
	bool IsDrawFlag() { return mbDrawFlag; }          //敵の削除フラグの作成

	//タグ
	void SetTag(Tag2D tag) { mnTag = tag; }
	Tag2D GetTag() { return mnTag; }

protected:
	VECTOR mvPosition;  //座標
	VECTOR mvRotation;   //回転

private:
	bool mbDeleteFlag;  //削除フラグ
	Tag2D mnTag;   //タグ
	bool mbDrawFlag;//描画フラグ


};
