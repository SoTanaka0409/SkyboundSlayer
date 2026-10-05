#include"InfClass.h"
#include"ItemManager.h"
#include"Master.h"


/// @brief InfClassの初期化（コンストラクタ）
InfClass::InfClass(int Log_timer,std::string Inf,int num)
	:Object2D(VGet(0,0,0))
	,LogNumber(num)
	
{
	LogCount = 0;
	elase_flag_ = false;
	LogTime = Log_timer;
	LogDate = Inf;
}


/// @brief InfClassの描画処理
void InfClass::Draw()
{
	int drawX = static_cast<int>(position_.x);
	int drawY = static_cast<int>(position_.y);
	
	// フェードアウト処理
	int alpha = 255;
	int remain = LogTime - LogCount;
	if (remain < 30) {
		alpha = (int)(255.0f * remain / 30.0f);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// 黒い半透明の背景を少し広めに描画
	DrawBox(drawX - 10, drawY - 5, drawX + 400, drawY + 30, GetColor(0, 0, 0), true);

	if (LogNumber == 1)
	{
		DrawFormatString(drawX, drawY, GetColor(150, 255, 150), "%sを獲得した", LogDate.c_str());
	}
	else if (LogNumber == 2)
	{
		DrawFormatString(drawX, drawY, GetColor(150, 200, 255), "%sを使用した", LogDate.c_str());
	}
	else if (LogNumber == 3)
	{
		DrawFormatString(drawX, drawY, GetColor(255, 150, 150), "%sが現れた", LogDate.c_str());
	}
	else if (LogNumber == 4)
	{
		DrawFormatString(drawX, drawY, GetColor(255, 200, 150), "%sは使用できません", LogDate.c_str());
	}
	else if(LogNumber==5)
	{
		DrawFormatString(drawX, drawY, GetColor(255, 255, 255), "%s", LogDate.c_str());
	}
	else
	{
		DrawFormatString(drawX, drawY, GetColor(200, 200, 200), "未確認なログです");
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// @brief InfClassの状態更新処理
void InfClass::Update()
{
	LogCount++;
	if (LogTime < LogCount)
	{
		elase_flag_ = true;
		SetDeleteFlag(true);
		SetDrawFlag(false);
	}
}
