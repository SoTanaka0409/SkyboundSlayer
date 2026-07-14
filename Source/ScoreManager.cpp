
#define _CRT_SECURE_NO_WARNINGS
#include"ScoreManager.h"
#include"DxLib.h"

ScoreManager::ScoreManager(float score)
	:mnScore(score)//取得スコア
	, mnHighScore(score)//ハイスコあ
	, mnHighScore2(score)//２番目
	, mnHighScore3(score)//三番目
{

}
ScoreManager::~ScoreManager()
{

}
void ScoreManager::Initialize()
{

}
void ScoreManager::Update()
{

}
void ScoreManager::Draw()
{




	////１行ずつ読み込む
	//char strBuffer[256] = "";//1行読み込むためのバッファ


	////ファイルを閉じる

	////バイナリファイルの書き込み


	////書き込み

	////ファイルくろーズ
}

void ScoreManager::PrintSaveDate(SaveDate date)
{

}
void ScoreManager::SaveHighScore()
{
	/*GameScene_Bonus* pScore = new GameScene_Bonus;
	if (pScore->SetDoScoreFlag(true))
	{
	}*/


	FILE* fp = NULL;
	//fopen(レクトリ/ファイル名。拡張子、オープンフィールド
	fp = fopen("savedate.txt", "w");  //書き込み専用でファイルを開く

	if (fp == NULL)
	{
		return;
	}

	if (mnScore > mnHighScore)//現在のスコアが１番高かったら
	{
		fprintf(fp, "SCORE;%d\n",(int) mnScore);//一番上に持ってくる

		fprintf(fp, "SCORE;%d\n", (int)mnHighScore);
		fprintf(fp, "SCORE;%d\n", (int)mnHighScore2);
		fprintf(fp, "NAME ;%s\n",msName.c_str());
		fprintf(fp, "NAME ;%s\n", msName1.c_str());
		fprintf(fp, "NAME ;%s\n", msName2.c_str());
	}
	if (mnScore > mnHighScore2 && mnScore < mnHighScore)//現在のスコアが二番目に高かったら
	{
		fprintf(fp, "SCORE;%d\n", (int)mnHighScore);

		fprintf(fp, "SCORE;%d\n", (int)mnScore);
		fprintf(fp, "SCORE;%d\n", (int)mnHighScore2);
		fprintf(fp, "NAME ;%s\n", msName1.c_str());
		fprintf(fp, "NAME ;%s\n", msName.c_str());
		fprintf(fp, "NAME ;%s\n", msName2.c_str());

	}
	if (mnScore > mnHighScore3 && mnScore < mnHighScore2)//現在のスコアが三番目だったら
	{
		fprintf(fp, "SCORE;%d\n", (int)mnHighScore);

		fprintf(fp, "SCORE;%d\n", (int)mnHighScore2);
		fprintf(fp, "SCORE;%d\n", (int)mnScore);
		fprintf(fp, "NAME ;%s\n", msName1.c_str()); //c_str...string=charにする
		fprintf(fp, "NAME ;%s\n", msName2.c_str());
		fprintf(fp, "NAME ;%s\n", msName.c_str());
	}
	fclose(fp);

}
void ScoreManager::LoadHighScore()
{
	FILE* fp = NULL;

	//fopen(レクトリ/ファイル名。拡張子、オープンフィールド
	fp = fopen("savedate.txt", "r");  //読み込み専用でファイルを開く
	if (fp == NULL)
	{
		return;
	}

	int s1 = 0, s2 = 0, s3 = 0;
	fscanf(fp, "SCORE:%d\n", &s1); mnHighScore = (float)s1;
	fscanf(fp, "SCORE:%d\n", &s2); mnHighScore2 = (float)s2;
	fscanf(fp, "SCORE:%d\n", &s3); mnHighScore3 = (float)s3;

	char name[256];
	fscanf(fp, "NAME :%s\n", name);
	msName = name;
	fscanf(fp, "NAME :%s\n", name);
	msName2 = name;
	fscanf(fp, "NAME :%s\n", name);
	msName3 = name;


	/*fscanf(fp, "NAME ;%s\n", name);
	msName3= name;*/
	/*fscanf(fp, "NAME ;%s\n", name);
	msName1 = name;*/


	fclose(fp);


}

void ScoreManager::Finalize()
{


}