#pragma once
#include"Model.h"
#include"Object3D.h"
class StageObject:public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename = "", float hitRadius = 0.0f, float yOffset = 0.0f);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetHitRadius() const { return hit_radius_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsHitEnabled() const { return hit_radius_ > 0.0f; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~StageObject();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

private:
	Model* model_;
	float scale_;
	float hit_radius_;
	bool has_followed_terrain_;
	float y_offset_;
};
