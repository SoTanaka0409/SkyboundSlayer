#include"Save.h"
#include"Master.h"
#include"InputManager.h"
#include"Texture.h"
#include"ObjectManager.h"
#include"SceneManager.h"

// 静的メンバ変数定義

Save::Save()
	:Select(0)
	,SelectMax(0)
	,SelectMin(0)
{

	mpTexture = new Texture("Resource/2D/Save.png", VGet(500, 500, 0), true);
	
	
}

Save::~Save()
{

}

void Save::Draw()
{
	if (Master::mbSave)
	{
		mpTexture->Draw();
	}

}

void Save::Update()
{
	
	if (InputManager::CheckDownKey(KEY_INPUT_L))
	{
		Master::mbSave = true;
	}
	if (Master::mbSave)
	{
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::mbSave = false;
		}
		
	}

}

void Save::GameLoad(SaveDate* date)
{

}

void Save::GameSave(SaveDate* date)
{
	for (auto itr = AllSaveDate.begin(); itr != AllSaveDate.end(); itr++)
	{
		if ((*itr)->num == date->num)
		{

		}
	}

}
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