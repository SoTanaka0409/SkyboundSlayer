#pragma once
#include <string>
#include <vector>
#include <iostream>

class ScoreManager
{
public:
	// コンストラクタ
	// [入力] float score: 初期スコア [出力] なし [副作用] メンバ変数の初期化
	ScoreManager(float score);
	
	// デストラクタ
	// [入力] なし [出力] なし [副作用] なし
	~ScoreManager();

	// 初期化
	// [入力] なし [出力] なし [副作用] なし
	void Initialize();
	
	// 更新
	// [入力] なし [出力] なし [副作用] なし
	void Update();
	
	// 描画
	// [入力] なし [出力] なし [副作用] なし
	void Draw();
	
	// 終了処理
	// [入力] なし [出力] なし [副作用] なし
	void Finalize();

	struct SaveDate
	{
		const char* name;
		float score;
	};

	// セーブデータ表示
	// [入力] SaveDate date: 表示データ [出力] なし [副作用] なし
	void PrintSaveDate(SaveDate date);

	// スコア加算
	// [入力] float add: 加算量 [出力] なし [副作用] score_ に加算
	void AddScore(float add)
	{
		score_ += add;
	}

	// スコア取得
	// [入力] なし [出力] int: 現在のスコア [副作用] なし
	int GetScore()
	{
		return static_cast<int>(score_);
	}

	// スコアリセット
	// [入力] なし [出力] なし [副作用] スコアと戦績を初期化
	void ResetScore()
	{
		score_ = 0;
		name_ = { 0 };
		ResetStats();
	}

	// ハイスコア取得
	// [入力] なし [出力] int: ハイスコア1位 [副作用] なし
	int GetHighScore()
	{
		return static_cast<int>(high_score_);
	}

	// ハイスコア取得2
	// [入力] なし [出力] int: ハイスコア2位 [副作用] なし
	int GetHighScore2()
	{
		return static_cast<int>(high_score2_);
	}

	// ハイスコア取得3
	// [入力] なし [出力] int: ハイスコア3位 [副作用] なし
	int GetHighScore3()
	{
		return static_cast<int>(high_score3_);
	}

	// 名前ID取得
	// [入力] なし [出力] int: 名前ID [副作用] なし
	int Getname()
	{
		return name_id_;
	}

	// 名前取得
	// [入力] なし [出力] std::string: 名前1位 [副作用] なし
	std::string GetName()
	{
		return name_;
	}

	// 名前取得1
	// [入力] なし [出力] std::string: 名前 [副作用] なし
	std::string GetName1()
	{
		return name1_;
	}

	// 名前取得2
	// [入力] なし [出力] std::string: 名前2位 [副作用] なし
	std::string GetName2()
	{
		return name2_;
	}

	// 名前取得3
	// [入力] なし [出力] std::string: 名前3位 [副作用] なし
	std::string GetName3()
	{
		return name3_;
	}

	// ハイスコア保存
	// [入力] なし [出力] なし [副作用] テキストファイルへの書き込み
	void SaveHighScore();
	
	// ハイスコア読み込み
	// [入力] なし [出力] なし [副作用] テキストファイルからの読み込みとメンバ更新
	void LoadHighScore();

	// フラグ設定
	// [入力] bool flag [出力] なし [副作用] is_normal_flag_ を更新
	void SetDoFlag(bool flag) { is_normal_flag_ = flag; }
	
	// フラグ取得
	// [入力] なし [出力] bool: フラグ状態 [副作用] なし
	bool IsDoFlag() { return is_normal_flag_; }

	// 名前保存
	// [入力] なし [出力] なし [副作用] なし
	void SaveName();
	
	// 名前読み込み
	// [入力] なし [出力] なし [副作用] なし
	void LoadName();

	// 戦績リセット
	// [入力] なし [出力] なし [副作用] 戦績関連メンバ変数を初期化
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
	// [入力] なし [出力] なし [副作用] defeated_enemies_ 加算
	void AddDefeatedEnemy() { defeated_enemies_++; }
	
	// 敵討伐数取得
	// [入力] なし [出力] int: 討伐数 [副作用] なし
	int GetDefeatedEnemies() const { return defeated_enemies_; }

	// ポーション使用回数加算
	// [入力] なし [出力] なし [副作用] used_potions_ 加算
	void AddUsedPotion() { used_potions_++; }
	
	// ポーション使用回数取得
	// [入力] なし [出力] int: 使用回数 [副作用] なし
	int GetUsedPotions() const { return used_potions_; }

	// 最終ステータス設定
	// [入力] float hp, float atk, float spd [出力] なし [副作用] ステータスメンバ更新
	void SetFinalStats(float hp, float atk, float spd)
	{
		final_hp_ = hp;
		final_attack_ = atk;
		final_speed_ = spd;
	}
	
	// 最終HP取得
	// [入力] なし [出力] float: HP [副作用] なし
	float GetFinalHp() const { return final_hp_; }
	
	// 最終攻撃力取得
	// [入力] なし [出力] float: 攻撃力 [副作用] なし
	float GetFinalAttack() const { return final_attack_; }
	
	// 最終スピード取得
	// [入力] なし [出力] float: スピード [副作用] なし
	float GetFinalSpeed() const { return final_speed_; }

	void SetResultVictory(bool isWin) { is_result_victory_ = isWin; }
	bool IsResultVictory() const { return is_result_victory_; }

private:
	int name_id_;
	float score_;
	std::string name_;
	std::string name1_;
	std::string name2_;
	std::string name3_;

	float high_score_;
	float high_score2_;
	float high_score3_;

	bool is_normal_flag_;

	int defeated_enemies_ = 0;
	int used_potions_ = 0;
	float final_hp_ = 0.0f;
	float final_attack_ = 0.0f;
	float final_speed_ = 0.0f;
	bool is_result_victory_ = true;
};
