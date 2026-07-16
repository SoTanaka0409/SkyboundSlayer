#pragma once
#include <string>
#include <vector>
#include <iostream>

class ScoreManager
{
public:
	ScoreManager(float score);
	~ScoreManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	struct SaveDate
	{
		const char* name;
		float Score;
	};
	void PrintSaveDate(SaveDate date);

	void AddScore(float add)
	{
		mnScore += add;
	}

	int GetScore()
	{
		return static_cast<int>(mnScore);
	}

	void ResetScore()
	{
		mnScore = 0;
		msName = { 0 };
		ResetStats();
	}

	int GetHighScore()
	{
		return static_cast<int>(mnHighScore);
	}

	int GetHighScore2()
	{
		return static_cast<int>(mnHighScore2);
	}

	int GetHighScore3()
	{
		return static_cast<int>(mnHighScore3);
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

	void SaveHighScore();
	void LoadHighScore();

	void SetDoFlag(bool flag) { mnNflag = flag; }
	bool IsDoFlag() { return mnNflag; }

	void SaveName();
	void LoadName();

	void ResetStats()
	{
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

	void SetFinalStats(float hp, float atk, float spd)
	{
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
	std::string msName;
	std::string msName1;
	std::string msName2;
	std::string msName3;

	float mnHighScore;
	float mnHighScore2;
	float mnHighScore3;

	bool mnNflag;

	int mnDefeatedEnemies = 0;
	int mnUsedPotions = 0;
	float mfFinalHp = 0.0f;
	float mfFinalAttack = 0.0f;
	float mfFinalSpeed = 0.0f;
};
