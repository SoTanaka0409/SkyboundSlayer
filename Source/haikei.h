#pragma once
#pragma once
#include"DxLib.h"
#include"Texture.h"
#include"Object2D.h"
#include"SceneManager.h"




class haikei : public Object2D
{

public:
	//コンストラクタ
	haikei(VECTOR initPos, std::string filename);
	//デストラクタ
	~haikei();

	void Update()override;//更新

	void Draw()override;//描画
private:
	Texture* mpTexture;
};