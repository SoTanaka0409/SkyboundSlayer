#pragma once
#pragma once
#include<string>
#include<vector>
#include<iostream>

class ScoreManager
{
public:
	ScoreManager(float score);
	~ScoreManager();

	//初期化
	void Initialize();
	void Update();
	void Draw();
	//終了処理
	void Finalize();

	struct SaveDate
	{
		const char* name;
		float Score;
	};
	void PrintSaveDate(SaveDate date);

	// スコア加算する関数
	void AddScore(float add)
	{
		mnScore += add;
	}
	// スコア取得関数
	int GetScore()
	{
		return mnScore;
	}
	// スコアリセット処理
	void ResetScore()
	{
		mnScore = 0;
		msName = { 0 };
		ResetStats();
	}
	//////////////////
	int GetHighScore()
	{
		return mnHighScore;
	}
	int GetHighScore2()
	{
		return mnHighScore2;
	}
	int GetHighScore3()
	{
		return mnHighScore3;
	}
	int Getname()
	{
		return Name;
	}
	std::string GetName()
	{
		return msName;
	}
	std::string GetName1()
	{
		return msName1;
	}
	std::string GetName2()
	{
		return msName2;
	}
	std::string GetName3()
	{
		return msName3;
	}

	//ハイスコアを保存
	void SaveHighScore();//保存処理をここで行う
	//ハイスコアロード
	void LoadHighScore();  //ロード処理を関数で行う

	void SetDoFlag(bool flag) { mnNflag = flag; } //敵の削除フラグ設定　
	bool IsDoFlag() { return mnNflag; }

	void SaveName();

	void LoadName();

	// RPG Stats Tracking
	void ResetStats() {
		mnDefeatedEnemies = 0;
		mnUsedPotions = 0;
		mfFinalHp = 0.0f;
		mfFinalAttack = 0.0f;
		mfFinalSpeed = 0.0f;
	}

	void AddDefeatedEnemy() { mnDefeatedEnemies++; }
	int GetDefeatedEnemies() const { return mnDefeatedEnemies; }

	void AddUsedPotion() { mnUsedPotions++; }
	int GetUsedPotions() const { return mnUsedPotions; }

	void SetFinalStats(float hp, float atk, float spd) {
		mfFinalHp = hp;
		mfFinalAttack = atk;
		mfFinalSpeed = spd;
	}
	float GetFinalHp() const { return mfFinalHp; }
	float GetFinalAttack() const { return mfFinalAttack; }
	float GetFinalSpeed() const { return mfFinalSpeed; }

private:
	int Name;
	float mnScore;
	std:: string msName;//保存処理をこの関数で行う
	std::string msName1;
	std::string msName2;
	std::string msName3;

	float mnHighScore;//ロード処理をこの関数で行う
	float mnHighScore2;
	float mnHighScore3;

	bool mnNflag;//Normalかどうかの判定

	int mnDefeatedEnemies = 0;
	int mnUsedPotions = 0;
	float mfFinalHp = 0.0f;
	float mfFinalAttack = 0.0f;
	float mfFinalSpeed = 0.0f;
};