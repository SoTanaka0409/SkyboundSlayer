# -*- coding: utf-8 -*-
import os

cpp_path = r'Source\ResultWin.cpp'

new_content = '''#include "ResultWin.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Texture.h"

ResultWin::ResultWin() : mnAllTimer(0), mNameStr(0)
{
}

ResultWin::~ResultWin()
{
}

void ResultWin::Initialize()
{
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);
}

void ResultWin::Update()
{
	mnAllTimer++;
}

void ResultWin::Draw()
{
	// 画面全体を明るく神聖な雰囲気に（白・ゴールド系の半透明フィルター）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawBox(0, 0, 1920, 1080, GetColor(255, 255, 200), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int size = GetFontSize();
	Master::mpScoreManager->SaveHighScore();
	
	if (mnAllTimer >= 50)
	{
		SetFontSize(80);
		DrawFormatString(320, 300, GetColor(255, 215, 0), "Trial Cleared!");
	}
	
	if (mnAllTimer >= 100)
	{
		SetFontSize(40);
		DrawFormatString(320, 500, GetColor(255, 255, 255), "賞賛しよう、%s", Master::NameTest);
	}
	
	if (mnAllTimer >= 200)
	{
		SetFontSize(40);
		DrawFormatString(400, 750, GetColor(255, 255, 255), "画面をタップ（クリック）して次へ");
		if (InputManager::CheckMouseClickLeft() || InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			// ResultWinはName画面へ遷移する
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_NAME);
		}
	}
	
	SetFontSize(size);
}

void ResultWin::Finalize()
{
}
'''

with open(cpp_path, 'w', encoding='shift_jis', errors='replace') as f:
    f.write(new_content)

print("ResultWin.cpp updated.")
