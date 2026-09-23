#include "Rule.h"
#include "Texture.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"



/// @brief Ruleの初期化（コンストラクタ）
Rule::Rule()
	: page_(1)
	, bg_handle1_(-1)
	, bg_handle2_(-1)
{
}

Rule::~Rule()
{
}



/// @brief Ruleの初期化処理
void Rule::Initialize()
{
	page_ = 1;
	bg_handle1_ = LoadGraph("Resource/image/rule/01_rule_bg_1.png");
	bg_handle2_ = LoadGraph("Resource/image/rule/02_rule_bg_2.png");
}



/// @brief Ruleの状態更新処理
void Rule::Update()
{
	Scene::Update();
	HandlePageInput();
}



/// @brief RuleのHandlePageInput処理
void Rule::HandlePageInput()
{
	if (InputManager::CheckDownKey(KEY_INPUT_RIGHT) || InputManager::CheckDownKey(KEY_INPUT_D))
	{
		if (page_ == 1)
		{
			page_ = 2;
		}
	}
	else if (InputManager::CheckDownKey(KEY_INPUT_LEFT) || InputManager::CheckDownKey(KEY_INPUT_A))
	{
		if (page_ == 2)
		{
			page_ = 1;
		}
	}

	if (InputManager::CheckDownKey(KEY_INPUT_BACK) )
	{
		Master::scene_manager_->SetNextScene(SceneManager::kSceneTitle);
	}
}



/// @brief Ruleの描画処理
void Rule::Draw()
{
	Scene::Draw();
	DrawRuleBackground();

	if (page_ == 1)
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



/// @brief RuleのDrawRuleBackground処理
void Rule::DrawRuleBackground()
{
	int handle = (page_ == 1) ? bg_handle1_ : bg_handle2_;
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



/// @brief RuleのDrawRulePage1処理
void Rule::DrawRulePage1()
{
	SetFontSize(48);
	DrawFormatString(150, 150, GetColor(255, 255, 255), "【 ルール説明 (1/2) 】");

	SetFontSize(40);
	int textY = 250;
	const int lineHeight = 60;
	DrawFormatString(150, textY, GetColor(200, 255, 200), "ゲームの進行について：");
	textY += (int)(lineHeight * 1.5);
	DrawFormatString(150, textY, GetColor(255, 255, 255), "通常Phase1～Phase3までは、出現する敵を全て倒します。");
	textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 255), "すべての敵を倒すと次のPhaseに進みます。");
	textY += (int)(lineHeight * 2);
	DrawFormatString(150, textY, GetColor(200, 255, 200), "ボスの討伐：");
	textY += (int)(lineHeight * 1.5);
	DrawFormatString(150, textY, GetColor(255, 255, 255), "Phase3をクリアした後、ボスポータルに向かいます。");
	textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 255), "ポータルに入るとボス戦が始まり、ボスを倒せばゲームクリアです！");
}



/// @brief RuleのDrawRulePage2処理
void Rule::DrawRulePage2()
{
	SetFontSize(48);
	DrawFormatString(150, 150, GetColor(255, 255, 255), "【 操作説明 (2/2) 】");

	SetFontSize(40);
	int textY = 250;
	const int lineHeight = 55;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[W] [A] [S] [D]  ... 移動"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[SPACE]          ... 回避"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 200, 200), "[左クリック]     ... 攻撃"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[E]              ... 攻撃方法の切り替え"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[←] [→]          ... アイテム選択"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(200, 255, 200), "[R]              ... アイテム使用"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[Enter]          ... ショップを開く"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 255, 200), "[F5] / [F6]      ... [デバッグ] 敵を全滅 / ボス戦へ移動"); textY += lineHeight;
	DrawFormatString(150, textY, GetColor(255, 200, 200), "[ESC]            ... ゲーム終了");
}



/// @brief RuleのDrawRuleFooter処理
void Rule::DrawRuleFooter()
{
	SetFontSize(32);
	DrawFormatString(150, 900, GetColor(200, 200, 200), "  [A]/[左矢印] 前のページ   |   次のページ [D]/[右矢印]  ");
	DrawFormatString(150, 950, GetColor(150, 150, 150), "[BackSpace] タイトルに戻る");
}



/// @brief RuleのFinalize処理
void Rule::Finalize()
{
	if (bg_handle1_ != -1)
	{
		DeleteGraph(bg_handle1_);
		bg_handle1_ = -1;
	}
	if (bg_handle2_ != -1)
	{
		DeleteGraph(bg_handle2_);
		bg_handle2_ = -1;
	}
}
