#pragma once
#include "Object3D.h"
#include <string>

class EffekseerObject : public Object3D
{
public:
	// name: 逋ｻ骭ｲ蜷・	// filepath: 繝輔ぃ繧､繝ｫ繝代せ
	// initPos: 蜀咲函髢句ｧ句ｺｧ讓・	// parent: 霑ｽ蠕灘ｯｾ雎｡縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝茨ｼ井ｸ崎ｦ√↑蝣ｴ蜷医・nullptr・・	// isFollow: parent縺ｫ霑ｽ蠕薙☆繧九°縺ｩ縺・°縺ｮ繝輔Λ繧ｰ
	// magnification: 諡｡螟ｧ邇・	// speed: 蜀咲函騾溷ｺｦ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	EffekseerObject(const std::string& name, const char* filepath, VECTOR initPos, Object3D* parent = nullptr, bool isFollow = false, float magnification = 1.0f, float speed = 1.0f);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual ~EffekseerObject();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update() override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw() override;

private:
	int playing_handle_;
	Object3D* parent_;
	bool is_follow_;
	VECTOR offset_;
};
