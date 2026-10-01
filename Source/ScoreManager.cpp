#define _CRT_SECURE_NO_WARNINGS
#include"ScoreManager.h"
#include"DxLib.h"

/// @brief ScoreManagerのコンストラクタ
/// @param float score
/// @details 各種変数の初期化
ScoreManager::ScoreManager(float score)
	:m_Score(score)
	, m_HighScore(score)
	, m_HighScore2(score)
	, m_HighScore3(score)
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
	if (m_Score > m_HighScore)
	{
		fprintf(fp, "SCORE;%d\n",(int)m_Score);
		fprintf(fp, "SCORE;%d\n", (int)m_HighScore);
		fprintf(fp, "SCORE;%d\n", (int)m_HighScore2);
		fprintf(fp, "NAME ;%s\n", m_Name.c_str());
		fprintf(fp, "NAME ;%s\n", m_Name1.c_str());
		fprintf(fp, "NAME ;%s\n", m_Name2.c_str());
	}
	// 2位更新時の処理
	else if (m_Score > m_HighScore2 && m_Score < m_HighScore)
	{
		fprintf(fp, "SCORE;%d\n", (int)m_HighScore);
		fprintf(fp, "SCORE;%d\n", (int)m_Score);
		fprintf(fp, "SCORE;%d\n", (int)m_HighScore2);
		fprintf(fp, "NAME ;%s\n", m_Name1.c_str());
		fprintf(fp, "NAME ;%s\n", m_Name.c_str());
		fprintf(fp, "NAME ;%s\n", m_Name2.c_str());
	}
	// 3位更新時の処理
	else if (m_Score > m_HighScore3 && m_Score < m_HighScore2)
	{
		fprintf(fp, "SCORE;%d\n", (int)m_HighScore);
		fprintf(fp, "SCORE;%d\n", (int)m_HighScore2);
		fprintf(fp, "SCORE;%d\n", (int)m_Score);
		fprintf(fp, "NAME ;%s\n", m_Name1.c_str());
		fprintf(fp, "NAME ;%s\n", m_Name2.c_str());
		fprintf(fp, "NAME ;%s\n", m_Name.c_str());
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
	fscanf(fp, "SCORE:%d\n", &s1); m_HighScore = (float)s1;
	fscanf(fp, "SCORE:%d\n", &s2); m_HighScore2 = (float)s2;
	fscanf(fp, "SCORE:%d\n", &s3); m_HighScore3 = (float)s3;

	char name_buf[256];
	fscanf(fp, "NAME :%s\n", name_buf);
	m_Name = name_buf;
	fscanf(fp, "NAME :%s\n", name_buf);
	m_Name2 = name_buf;
	fscanf(fp, "NAME :%s\n", name_buf);
	m_Name3 = name_buf;

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
