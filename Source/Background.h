#pragma once
#pragma once
#include"DxLib.h"
#include"Texture.h"
#include"Object2D.h"
#include"SceneManager.h"




class Background : public Object2D
{

public:
	/// @brief コンストラクタ
	Background(VECTOR initPos, std::string filename);
	/// @brief デストラクタ
	~Background();
	void Update()override;//更新
	void Draw()override;//描画
private:
	Texture* m_Texture;
};