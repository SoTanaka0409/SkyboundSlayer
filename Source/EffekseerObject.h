#pragma once
#include "Object3D.h"
#include <string>

class EffekseerObject : public Object3D
{
public:
	// name: 登録名
	// filepath: ファイルパス
	// initPos: 再生開始座標
	// parent: 追従対象のオブジェクト（不要な場合はnullptr）
	// isFollow: parentに追従するかどうかのフラグ
	// magnification: 拡大率
	// speed: 再生速度
	EffekseerObject(const std::string& name, const char* filepath, VECTOR initPos, Object3D* parent = nullptr, bool isFollow = false, float magnification = 1.0f, float speed = 1.0f);
	virtual ~EffekseerObject();

	void Update() override;
	void Draw() override;

private:
	int mPlayingHandle;
	Object3D* mpParent;
	bool mbIsFollow;
	VECTOR mOffset;
};
