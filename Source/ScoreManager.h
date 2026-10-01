#pragma once
#include <string>
#include <vector>
#include <iostream>

class ScoreManager
{
public:
	/// @brief コンストラクタ
	ScoreManager(float score);
	
	/// @brief デストラクタ
	~ScoreManager();

	/// @brief 初期化
	void Initialize();
	
	/// @brief 更新
	void Update();
	
	/// @brief 描画
	void Draw();
	
	/// @brief 終了処理
	void Finalize();

	struct SaveDate
	{
		const char* name;
		float score;
	};

	/// @brief セーブデータ表示
	void PrintSaveDate(SaveDate date);

	/// @brief スコア加算
	void AddScore(float add)
	{
		m_Score += add;
	}

	/// @brief スコア取得
	int GetScore()
	{
		return static_cast<int>(m_Score);
	}

	/// @brief スコアリセット
	void ResetScore()
	{
		m_Score = 0;
		m_Name = { 0 };
		ResetStats();
	}

	/// @brief ハイスコア取得
	int GetHighScore()
	{
		return static_cast<int>(m_HighScore);
	}

	/// @brief ハイスコア取得2
	int GetHighScore2()
	{
		return static_cast<int>(m_HighScore2);
	}

	/// @brief ハイスコア取得3
	int GetHighScore3()
	{
		return static_cast<int>(m_HighScore3);
	}

	/// @brief 名前ID取得
	int Getname()
	{
		return m_NameId;
	}

	/// @brief 名前取得
	std::string GetName()
	{
		return m_Name;
	}

	/// @brief 名前取得1
	std::string GetName1()
	{
		return m_Name1;
	}

	/// @brief 名前取得2
	std::string GetName2()
	{
		return m_Name2;
	}

	/// @brief 名前取得3
	std::string GetName3()
	{
		return m_Name3;
	}

	/// @brief ハイスコア保存
	void SaveHighScore();
	
	/// @brief ハイスコア読み込み
	void LoadHighScore();

	/// @brief フラグ設定
	void SetDoFlag(bool flag) { m_IsNormalFlag = flag; }
	
	/// @brief フラグ取得
	bool IsDoFlag() { return m_IsNormalFlag; }

	/// @brief 名前保存
	void SaveName();
	
	/// @brief 名前読み込み
	void LoadName();

	/// @brief 戦績リセット
	void ResetStats()
	{
		m_DefeatedEnemies = 0;
		m_UsedPotions = 0;
		m_FinalHp = 0.0f;
		m_FinalAttack = 0.0f;
		m_FinalSpeed = 0.0f;
		m_IsResultVictory = true;
	}

	/// @brief 敵討伐数加算
	void AddDefeatedEnemy() { m_DefeatedEnemies++; }
	
	/// @brief 敵討伐数取得
	int GetDefeatedEnemies() const { return m_DefeatedEnemies; }

	/// @brief ポーション使用回数加算
	void AddUsedPotion() { m_UsedPotions++; }
	
	/// @brief ポーション使用回数取得
	int GetUsedPotions() const { return m_UsedPotions; }

	/// @brief 最終ステータス設定
	void SetFinalStats(float hp, float atk, float spd)
	{
		m_FinalHp = hp;
		m_FinalAttack = atk;
		m_FinalSpeed = spd;
	}
	
	/// @brief 最終HP取得
	float GetFinalHp() const { return m_FinalHp; }
	
	/// @brief 最終攻撃力取得
	float GetFinalAttack() const { return m_FinalAttack; }
	
	/// @brief 最終スピード取得
	float GetFinalSpeed() const { return m_FinalSpeed; }

	void SetResultVictory(bool isWin) { m_IsResultVictory = isWin; }
	bool IsResultVictory() const { return m_IsResultVictory; }

private:
	int m_NameId;                 ///< プレイヤー名識別用のIDハンドル
	float m_Score;                 ///< 現在のゲームスコア
	std::string m_Name;            ///< 登録されたプレイヤー名
	std::string m_Name1;           ///< ランキング1位のプレイヤー名
	std::string m_Name2;           ///< ランキング2位のプレイヤー名
	std::string m_Name3;           ///< ランキング3位のプレイヤー名

	float m_HighScore;            ///< ハイスコア（1位のスコア）
	float m_HighScore2;           ///< 2位のハイスコア
	float m_HighScore3;           ///< 3位のハイスコア

	bool m_IsNormalFlag;         ///< 通常モード・通常難易度判定フラグ

	int m_DefeatedEnemies = 0;    ///< 倒した敵の総数（撃破数）
	int m_UsedPotions = 0;        ///< 使用したポーションの個数
	float m_FinalHp = 0.0f;       ///< 終了時の最終体力（HP）
	float m_FinalAttack = 0.0f;   ///< 終了時の最終攻撃力
	float m_FinalSpeed = 0.0f;    ///< 終了時の最終移動速度
	bool m_IsResultVictory = true; ///< リザルト結果の勝敗フラグ（true: 勝利, false: 敗北）
};
