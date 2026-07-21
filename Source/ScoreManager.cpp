#define _CRT_SECURE_NO_WARNINGS
#include"ScoreManager.h"
#include"DxLib.h"

/// @brief ScoreManagerのコンストラクタ
/// @param float score
/// @details 各種変数の初期化
ScoreManager::ScoreManager(float score)
	:score_(score)
	, high_score_(score)
	, high_score2_(score)
	, high_score3_(score)
{

}

/// @brief ScoreManagerのデストラクタ
ScoreManager::~ScoreManager()
{

}

/// @brief 初期化処理
/// @details スコアの読み込み
void ScoreManager::Initialize()
{

}

/// @brief スコアの更新処理
void ScoreManager::Update()
{

}

/// @brief スコアの描画処理
/// @details 画面描画
void ScoreManager::Draw()
{

}

/// @brief セーブデータの情報を表示するため
/// @param SaveDate date
/// @details 画面描画
void ScoreManager::PrintSaveDate(SaveDate date)
{

}

/// @brief ハイスコアをファイルに保存するため
/// @details テキストファイルへの書き込み
void ScoreManager::SaveHighScore()
{
	FILE* fp = NULL;
	fp = fopen("savedate.txt", "w");

	// ファイルが開けなければ終了
	if (fp == NULL)
	{
		return;
	}

	// 1位更新時の処理
	if (score_ > high_score_)
	{
		fprintf(fp, "SCORE;%d\n",(int)score_);
		fprintf(fp, "SCORE;%d\n", (int)high_score_);
		fprintf(fp, "SCORE;%d\n", (int)high_score2_);
		fprintf(fp, "NAME ;%s\n", name_.c_str());
		fprintf(fp, "NAME ;%s\n", name1_.c_str());
		fprintf(fp, "NAME ;%s\n", name2_.c_str());
	}
	// 2位更新時の処理
	else if (score_ > high_score2_ && score_ < high_score_)
	{
		fprintf(fp, "SCORE;%d\n", (int)high_score_);
		fprintf(fp, "SCORE;%d\n", (int)score_);
		fprintf(fp, "SCORE;%d\n", (int)high_score2_);
		fprintf(fp, "NAME ;%s\n", name1_.c_str());
		fprintf(fp, "NAME ;%s\n", name_.c_str());
		fprintf(fp, "NAME ;%s\n", name2_.c_str());
	}
	// 3位更新時の処理
	else if (score_ > high_score3_ && score_ < high_score2_)
	{
		fprintf(fp, "SCORE;%d\n", (int)high_score_);
		fprintf(fp, "SCORE;%d\n", (int)high_score2_);
		fprintf(fp, "SCORE;%d\n", (int)score_);
		fprintf(fp, "NAME ;%s\n", name1_.c_str());
		fprintf(fp, "NAME ;%s\n", name2_.c_str());
		fprintf(fp, "NAME ;%s\n", name_.c_str());
	}
	fclose(fp);
}

/// @brief ハイスコアをファイルから読み込むため
/// @details ファイル読み込みと変数の更新
void ScoreManager::LoadHighScore()
{
	FILE* fp = NULL;

	fp = fopen("savedate.txt", "r");
	
	// ファイルがなければ終了
	if (fp == NULL)
	{
		return;
	}

	int s1 = 0, s2 = 0, s3 = 0;
	fscanf(fp, "SCORE:%d\n", &s1); high_score_ = (float)s1;
	fscanf(fp, "SCORE:%d\n", &s2); high_score2_ = (float)s2;
	fscanf(fp, "SCORE:%d\n", &s3); high_score3_ = (float)s3;

	char name_buf[256];
	fscanf(fp, "NAME :%s\n", name_buf);
	name_ = name_buf;
	fscanf(fp, "NAME :%s\n", name_buf);
	name2_ = name_buf;
	fscanf(fp, "NAME :%s\n", name_buf);
	name3_ = name_buf;

	fclose(fp);
}

/// @brief 名前をファイルに保存するため
/// @details テキストファイルへの書き込み
void ScoreManager::SaveName()
{
}

/// @brief 名前をファイルから読み込むため
/// @details ファイル読み込みと変数の更新
void ScoreManager::LoadName()
{
}

/// @brief スコアの終了処理
/// @details スコアの保存
void ScoreManager::Finalize()
{

}
