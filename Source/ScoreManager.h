#pragma once
#include <string>
#include <vector>
#include <iostream>

class ScoreManager
{
public:
	// コンストラクタ
	ScoreManager(float score);
	
	// デストラクタ
	~ScoreManager();

	// 初期化
	void Initialize();
	
	// 更新
	void Update();
	
	// 描画
	void Draw();
	
	// 終了処理
	void Finalize();

	struct SaveDate
	{
		const char* name;
		float score;
	};

	// セーブデータ表示
	void PrintSaveDate(SaveDate date);

	// スコア加算
	void AddScore(float add)
	{
		score_ += add;
	}

	// スコア取得
	int GetScore()
	{
		return static_cast<int>(score_);
	}

	// スコアリセット
	void ResetScore()
	{
		score_ = 0;
		name_ = { 0 };
		ResetStats();
	}

	// ハイスコア取得
	int GetHighScore()
	{
		return static_cast<int>(high_score_);
	}

	// ハイスコア取得2
	int GetHighScore2()
	{
		return static_cast<int>(high_score2_);
	}

	// ハイスコア取得3
	int GetHighScore3()
	{
		return static_cast<int>(high_score3_);
	}

	// 名前ID取得
	int Getname()
	{
		return name_id_;
	}

	// 名前取得
	std::string GetName()
	{
		return name_;
	}

	// 名前取得1
	std::string GetName1()
	{
		return name1_;
	}

	// 名前取得2
	std::string GetName2()
	{
		return name2_;
	}

	// 名前取得3
	std::string GetName3()
	{
		return name3_;
	}

	// ハイスコア保存
	void SaveHighScore();
	
	// ハイスコア読み込み
	void LoadHighScore();

	// フラグ設定
	void SetDoFlag(bool flag) { is_normal_flag_ = flag; }
	
	// フラグ取得
	bool IsDoFlag() { return is_normal_flag_; }

	// 名前保存
	void SaveName();
	
	// 名前読み込み
	void LoadName();

	// 戦績リセット
	void ResetStats()
	{
		defeated_enemies_ = 0;
		used_potions_ = 0;
		final_hp_ = 0.0f;
		final_attack_ = 0.0f;
		final_speed_ = 0.0f;
		is_result_victory_ = true;
	}

	// 敵討伐数加算
	void AddDefeatedEnemy() { defeated_enemies_++; }
	
	// 敵討伐数取得
	int GetDefeatedEnemies() const { return defeated_enemies_; }

	// ポーション使用回数加算
	void AddUsedPotion() { used_potions_++; }
	
	// ポーション使用回数取得
	int GetUsedPotions() const { return used_potions_; }

	// 最終ステータス設定
	void SetFinalStats(float hp, float atk, float spd)
	{
		final_hp_ = hp;
		final_attack_ = atk;
		final_speed_ = spd;
	}
	
	// 最終HP取得
	float GetFinalHp() const { return final_hp_; }
	
	// 最終攻撃力取得
	float GetFinalAttack() const { return final_attack_; }
	
	// 最終スピード取得
	float GetFinalSpeed() const { return final_speed_; }

	void SetResultVictory(bool isWin) { is_result_victory_ = isWin; }
	bool IsResultVictory() const { return is_result_victory_; }

private:
	int name_id_;                 ///< プレイヤー名識別用のIDハンドル
	float score_;                 ///< 現在のゲームスコア
	std::string name_;            ///< 登録されたプレイヤー名
	std::string name1_;           ///< ランキング1位のプレイヤー名
	std::string name2_;           ///< ランキング2位のプレイヤー名
	std::string name3_;           ///< ランキング3位のプレイヤー名

	float high_score_;            ///< ハイスコア（1位のスコア）
	float high_score2_;           ///< 2位のハイスコア
	float high_score3_;           ///< 3位のハイスコア

	bool is_normal_flag_;         ///< 通常モード・通常難易度判定フラグ

	int defeated_enemies_ = 0;    ///< 倒した敵の総数（撃破数）
	int used_potions_ = 0;        ///< 使用したポーションの個数
	float final_hp_ = 0.0f;       ///< 終了時の最終体力（HP）
	float final_attack_ = 0.0f;   ///< 終了時の最終攻撃力
	float final_speed_ = 0.0f;    ///< 終了時の最終移動速度
	bool is_result_victory_ = true; ///< リザルト結果の勝敗フラグ（true: 勝利, false: 敗北）
};
