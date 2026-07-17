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
		
	};

public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Object2D(VECTOR initPos);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual ~Object2D();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Draw();

public://ゲッター、セッター
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetPosition(VECTOR pos) { position_ = pos; };
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VECTOR GetPosition() { return position_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetRotation(VECTOR rot) { rotation_ = rot; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VECTOR GETRotation() { return rotation_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsDeleteFlag() { return delete_flag_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetDrawFlag(bool flag) { draw_flag_ = flag; } //敵の削除フラグ設定　
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsDrawFlag() { return draw_flag_; }          //敵の削除フラグの作成

	//タグ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetTag(Tag2D tag) { tag_ = tag; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Tag2D GetTag() { return tag_; }

protected:
	VECTOR position_;  //座標
	VECTOR rotation_;   //回転

private:
	bool delete_flag_;  //削除フラグ
	Tag2D tag_;   //タグ
	bool draw_flag_;//描画フラグ


};
