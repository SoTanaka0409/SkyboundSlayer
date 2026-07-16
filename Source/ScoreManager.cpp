
#define _CRT_SECURE_NO_WARNINGS
#include"ScoreManager.h"
#include"DxLib.h"

ScoreManager::ScoreManager(float score)
	:mnScore(score)
	, mnHighScore(score)
	, mnHighScore2(score)
	, mnHighScore3(score)
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
	fp = fopen("savedate.txt", "w");

	if (fp == NULL)
	{
		return;
	}

	if (mnScore > mnHighScore)
	{
		fprintf(fp, "SCORE;%d\n",(int) mnScore);

		fprintf(fp, "SCORE;%d\n", (int)mnHighScore);
		fprintf(fp, "SCORE;%d\n", (int)mnHighScore2);
		fprintf(fp, "NAME ;%s\n",msName.c_str());
		fprintf(fp, "NAME ;%s\n", msName1.c_str());
		fprintf(fp, "NAME ;%s\n", msName2.c_str());
	}
	if (mnScore > mnHighScore2 && mnScore < mnHighScore)
	{
		fprintf(fp, "SCORE;%d\n", (int)mnHighScore);

		fprintf(fp, "SCORE;%d\n", (int)mnScore);
		fprintf(fp, "SCORE;%d\n", (int)mnHighScore2);
		fprintf(fp, "NAME ;%s\n", msName1.c_str());
		fprintf(fp, "NAME ;%s\n", msName.c_str());
		fprintf(fp, "NAME ;%s\n", msName2.c_str());

	}
	if (mnScore > mnHighScore3 && mnScore < mnHighScore2)
	{
		fprintf(fp, "SCORE;%d\n", (int)mnHighScore);

		fprintf(fp, "SCORE;%d\n", (int)mnHighScore2);
		fprintf(fp, "SCORE;%d\n", (int)mnScore);
		fprintf(fp, "NAME ;%s\n", msName1.c_str());
		fprintf(fp, "NAME ;%s\n", msName2.c_str());
		fprintf(fp, "NAME ;%s\n", msName.c_str());
	}
	fclose(fp);

}
void ScoreManager::LoadHighScore()
{
	FILE* fp = NULL;

	fp = fopen("savedate.txt", "r");
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
