#include"Combo.h"


/// @brief Comboの初期化（コンストラクタ）
Combo::Combo()
	:Object2D(VGet(0,0,0))
	,m_Combo(0)
{
	 handle = LoadGraph("%d", true);

}

Combo::~Combo()
{

}


/// @brief Comboの描画処理
void Combo::Draw()
{
	/*for (int i = 0; i < 10; i++)
	{
		DrawGraph(100,100,handle,)
		
	}*/

}



/// @brief Comboの状態更新処理
void Combo::Update()
{
	
}


/// @brief ComboのHitAddCombo処理
void Combo::HitAddCombo()
{
	m_Combo++;
}
