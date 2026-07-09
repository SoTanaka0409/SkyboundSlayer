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
	void SetPosition(VECTOR pos) { position_ = pos; };
	VECTOR GetPosition() { return position_; }

	void SetRotation(VECTOR rot) { rotation_ = rot; }
	VECTOR GETRotation() { return rotation_; }

	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() { return delete_flag_; }

	void SetDrawFlag(bool flag) { draw_flag_ = flag; } //敵の削除フラグ設定　
	bool IsDrawFlag() { return draw_flag_; }          //敵の削除フラグの作成

	//タグ
	void SetTag(Tag2D tag) { tag_ = tag; }
	Tag2D GetTag() { return tag_; }

protected:
	VECTOR position_;  //座標
	VECTOR rotation_;   //回転

private:
	bool delete_flag_;  //削除フラグ
	Tag2D tag_;   //タグ
	bool draw_flag_;//描画フラグ


};
