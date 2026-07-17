#include"Save.h"
#include"Master.h"
#include"InputManager.h"
#include"Texture.h"
#include"ObjectManager.h"
#include"SceneManager.h"

/*
 * 目的（SaveのSave処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
// 静的メンバ変数定義
Save::Save()
	:Select(0)
	,SelectMax(0)
	,SelectMin(0)
{

	texture_ = new Texture("Resource/2D/Save.png", VGet(500, 500, 0), true);
	
	
}

Save::~Save()
{

}


/*
 * 目的（SaveのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Save::Draw()
{
	if (Master::is_save_)
	{
		texture_->Draw();
	}

}


/*
 * 目的（SaveのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Save::Update()
{
	
	if (InputManager::CheckDownKey(KEY_INPUT_L))
	{
		Master::is_save_ = true;
	}
	if (Master::is_save_)
	{
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::is_save_ = false;
		}
		
	}

}


/*
 * 目的（SaveのGameLoad処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Save::GameLoad(SaveDate* date)
{

}


/*
 * 目的（SaveのGameSave処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Save::GameSave(SaveDate* date)
{
	for (auto itr = AllSaveDate.begin(); itr != AllSaveDate.end(); itr++)
	{
		if ((*itr)->num == date->num)
		{

		}
	}

}

/*
 * 目的（SaveのSelectClass処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Save::SelectClass()
{
	if (InputManager::CheckDownKey(KEY_INPUT_UP))
	{
		Select--;
	}
	if(InputManager::CheckDownKey(KEY_INPUT_DOWN))
	{
		Select++;
	}

}