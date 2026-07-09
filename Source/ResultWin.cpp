#include "ResultWin.h"
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
	// ��ʑS�̂𖾂邭�_���ȕ��͋C�Ɂi���E�S�[���h�n�̔������t�B���^�[�j
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
		DrawFormatString(320, 500, GetColor(255, 255, 255), "�܎^���悤�A%s", Master::NameTest);
	}
	
	if (mnAllTimer >= 200)
	{
		SetFontSize(40);
		DrawFormatString(400, 750, GetColor(255, 255, 255), "��ʂ�^�b�v�i�N���b�N�j���Ď���");
		if (InputManager::CheckMouseClickLeft() || InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			// ResultWin��Name��ʂ֑J�ڂ���
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}
	
	SetFontSize(size);
}

void ResultWin::Finalize()
{
}
