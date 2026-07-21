#pragma once
class Model;

#include "Object3D.h"

class SkyBox : public Object3D
{
public:
	SkyBox(std::string filename,VECTOR pos);		// コンストラクタ
	~SkyBox();							// デストラクタ
	void Update() override;	// 更新処理
	void Draw() override;	// 描画処理
	void SetScale(VECTOR scale);									// 拡大値（スケール値）の設定（Modelクラスへの橋渡し）
	void SetModelTexture(std::string filename, int index = 0);	// モデルのテクスチャ変更（Modelクラスへの橋渡し）

private:
	Model* model_;			// モデルクラスのポインタ
};

