#include"Combo.h"


/*
 * 目的（ComboのCombo処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Combo::Combo()
	:Object2D(VGet(0,0,0))
	,combo_(0)
{
	 handle = LoadGraph("%d", true);

}

Combo::~Combo()
{

}


/*
 * 目的（ComboのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Combo::Draw()
{
	/*for (int i = 0; i < 10; i++)
	{
		DrawGraph(100,100,handle,)
		
	}*/

}



/*
 * 目的（ComboのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Combo::Update()
{
	
}


/*
 * 目的（ComboのHitAddCombo処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Combo::HitAddCombo()
{
	combo_++;
}
