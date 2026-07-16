#include "Rule.h"
#include "Texture.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"

Rule::Rule()
	: mPage(1)
	, mBgHandle1(-1)
	, mBgHandle2(-1)
{
}

Rule::~Rule()
{
}

void Rule::Initialize()
{
	mPage = 1;
	mBgHandle1 = LoadGraph("Resource/2D/rule_bg_1.png");
	mBgHandle2 = LoadGraph("Resource/2D/rule_bg_2.png");
}

void Rule::Update()
{
	Scene::Update();
	HandlePageInput();
}

void Rule::HandlePageInput()
{
	if (InputManager::CheckDownKey(KEY_INPUT_RIGHT) || InputManager::CheckDownKey(KEY_INPUT_D))
	{
		if (mPage == 1)
		{
			mPage = 2;
		}
	}
	else if (InputManager::CheckDownKey(KEY_INPUT_LEFT) || InputManager::CheckDownKey(KEY_INPUT_A))
	{
		if (mPage == 2)
		{
			mPage = 1;
		}
	}

	if (InputManager::CheckDownKey(KEY_INPUT_BACK) || InputManager::CheckDownKey(KEY_INPUT_ESCAPE))
	{
		Master::mpSceneManager->SetNextScene(SceneManager::kSceneTitle);
	}
}

void Rule::Draw()
{
	Scene::Draw();
	DrawRuleBackground();

	if (mPage == 1)
	{
		DrawRulePage1();
	}
	else
	{
		DrawRulePage2();
	}

	DrawRuleFooter();
	SetFontSize(24);
}

void Rule::DrawRuleBackground()
{
	int handle = (mPage == 1) ? mBgHandle1 : mBgHandle2;
	if (handle != -1)
	{
		DrawExtendGraph(0, 0, 1980, 1080, handle, TRUE);
	}
	else
	{
		DrawBox(0, 0, 1980, 1080, GetColor(0, 0, 0), TRUE);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(100, 100, 1880, 980, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Rule::DrawRulePage1()
{
	SetFontSize(48);
	DrawFormatString(150, 150, GetColor(255, 255, 255), "【 ルール説明 (1/2) 】");

	SetFontSize(40);
	int textY = 250;
	const int lineHeight = 60;
	DrawFormatString(150, textY, GetColor(200, 255, 200), "ゲームの進行について：");
	textY += (int)(lineHeight * 1.5);
	DrawFormatString(150, textY, GetColor(255, 255, 255), "通常Phase1?Phase3までは、出現する敵を全て倒します。");
	textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 255), "すべての敵を倒すと次のPhaseに進みます。");
	textY += (int)(lineHeight * 2);
	DrawFormatString(150, textY, GetColor(200, 255, 200), "ボスの討伐：");
	textY += (int)(lineHeight * 1.5);
	DrawFormatString(150, textY, GetColor(255, 255, 255), "Phase3をクリアした後、ボスポータルに向かいます。");
	textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 255), "ポータルに入るとボス戦が始まり、ボスを倒せばゲームクリアです！");
}

void Rule::DrawRulePage2()
{
	SetFontSize(48);
	DrawFormatString(150, 150, GetColor(255, 255, 255), "【 操作説明 (2/2) 】");

	SetFontSize(40);
	int textY = 250;
	const int lineHeight = 65;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[W] [A] [S] [D]  ... 移動"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[SPACE]          ... ジャンプ / 回避"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 200, 200), "[左クリック]     ... 攻撃"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(200, 255, 200), "[R]              ... アイテム使用"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[E]              ... インタラクト / ショップを開く / 調べる"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[Tab]            ... インベントリ / ステータス"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[ESC]            ... ポーズ / メニュー");
}

void Rule::DrawRuleFooter()
{
	SetFontSize(32);
	DrawFormatString(150, 900, GetColor(200, 200, 200), "? [A]/[左矢印] 前のページ   |   次のページ [D]/[右矢印] ?");
	DrawFormatString(150, 950, GetColor(150, 150, 150), "[BackSpace] / [ESC] タイトルに戻る");
}

void Rule::Finalize()
{
	if (mBgHandle1 != -1)
	{
		DeleteGraph(mBgHandle1);
		mBgHandle1 = -1;
	}
	if (mBgHandle2 != -1)
	{
		DeleteGraph(mBgHandle2);
		mBgHandle2 = -1;
	}
}
