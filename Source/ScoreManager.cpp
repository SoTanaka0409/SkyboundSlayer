
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

	//FILE* fp;
	//char buffer;
	//while (1)
	//{
	//	buffer = fgetc(fp);//１文字読み込み
	//	if (buffer == EOF) //EOF...End Of Fileの略。ファイルの内容が最後まで読み込めたかどうかの判定。
	//	{
	//		break;

	//	}
	//	(40);
	//	unsigned int Color;
	//	Color = GetColor(0, 255, 0);
	//	DrawFormatString(60, 200, Color, "%c", buffer); 
	//}

	//fseek(fp, 0, SEEK_SET);

	////１行ずつ読み込む
	//char strBuffer[256] = "";//1行読み込むためのバッファ
	//while (1)
	//{
	//	fgets(strBuffer, 256, fp);//１行読み込み
	//	if (feof(fp))  //fpがEOFかどうかチェック
	//	{
	//		break;
	//	}
	//	(40);
	//	unsigned int Color;
	//	Color = GetColor(0, 255, 0);
	//	DrawFormatString(60, 200, Color, "%c",buffer);
	//}


	////ファイルを閉じる
	//fclose(fp);

	////バイナリファイルの書き込み
	//fp = fopen("savedate.bin", "w");
	//if (fp == NULL)
	//{
	//	printf("ファイルの読み込みに失敗しました。\n");
	//	return ;


	//}
	////書き込み
	//fwrite(&date, sizeof(SaveDate), 1, fp);

	////ファイルくろーズ
	//fclose(fp);
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

	fscanf(fp, "SCORE:%d\n", (int)&mnHighScore);
	fscanf(fp, "SCORE:%d\n", (int)&mnHighScore2);
	fscanf(fp, "SCORE:%d\n", (int)&mnHighScore3);

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