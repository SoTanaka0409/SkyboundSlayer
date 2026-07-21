#pragma once
#include "DxLib.h"
#include <string>
#include <utility>
#include <vector>

/// @brief BGM・SEの読み込み、再生、停止、音量設定を一括管理するサウンドマネージャークラス
class SoundManager
{
public:
	/// @brief 再生するBGMの種類を識別する列挙型
	enum SOUND_BGM
	{
		BGM_TITLE = 0, ///< タイトル画面のBGM
		BGM_GAME,      ///< ゲーム本編画面のBGM
		BGM_RESULT,    ///< リザルト画面のBGM
	};

	/// @brief 再生する効果音（SE）の種類を識別する列挙型
	enum SOUND_SE
	{
		SE_FIRE,        ///< 決定音・発射音
		SE_ATTACK,      ///< 通常攻撃音
		SE_JUMP,        ///< ジャンプ音
		SE_SLIDE,       ///< 回避・スライド音
		SE_ATTACKSLIDE, ///< スライド攻撃音
		SE_SHOP,        ///< ショップ利用音
		SE_HEAL,        ///< 回復効果音
		SE_WARP,        ///< ワープ演出音
		SE_POWER,       ///< 強化・パワーアップ音
		SE_SELECT,      ///< カーソル選択音
		SE_WINDOW,      ///< ウィンドウ開閉音
	};

public:
	/// @brief SoundManagerのコンストラクタ
	SoundManager();

	/// @brief SoundManagerのデストラクタ
	~SoundManager();

	/// @brief 各種音声ファイルを事前ロードし、音量等の初期化を行う
	void Initialize();

	/// @brief 読み込み済みの音声ハンドル・リソースを安全に破棄・解放する
	void Finalize();

	/// @brief 指定したBGMの再生を開始する
	/// @param bgm 再生したいBGMの種類（SOUND_BGM）
	/// @param IsTop 曲の最初（先頭）から再生するかどうか（既定値: true）
	void PlayBGM(SOUND_BGM bgm, bool IsTop = true);

	/// @brief 指定した効果音（SE）を再生する
	/// @param se 再生したいSEの種類（SOUND_SE）
	void PlaySE(SOUND_SE se);

	/// @brief 指定したBGMファイルを読み込んでリストに登録する
	/// @param bgm 登録するBGMの識別ID
	/// @param filename 読み込む音声ファイルのパス
	void LoadBGM(SOUND_BGM bgm, std::string filename);

	/// @brief 指定したSEファイルを読み込んでリストに登録する
	/// @param se 登録するSEの識別ID
	/// @param filename 読み込む音声ファイルのパス
	void LoadSE(SOUND_SE se, std::string filename);

	/// @brief 現在再生されているBGMを停止する
	void StopBGM();

	/// @brief BGMの有効/無効設定状態を取得する
	/// @return bool BGM再生が有効であればtrue
	bool IsBgmEnabled() const { return is_bgm_enabled_; }

	/// @brief SEの有効/無効設定状態を取得する
	/// @return bool SE再生が有効であればtrue
	bool IsSeEnabled() const { return is_se_enabled_; }

	/// @brief 現在設定されているBGMの音量（0〜255）を取得する
	/// @return int BGM音量値
	int GetBgmVolume() const { return bgm_volume_; }

	/// @brief 現在設定されているSEの音量（0〜255）を取得する
	/// @return int SE音量値
	int GetSeVolume() const { return se_volume_; }

	/// @brief BGMの有効/無効を設定する
	/// @param enabled 有効にする場合はtrue
	void SetBgmEnabled(bool enabled);

	/// @brief SEの有効/無効を設定する
	/// @param enabled 有効にする場合はtrue
	void SetSeEnabled(bool enabled);

	/// @brief BGMの音量を設定・適用する
	/// @param volume 設定する音量値（0〜255）
	void SetBgmVolume(int volume);

	/// @brief SEの音量を設定・適用する
	/// @param volume 設定する音量値（0〜255）
	void SetSeVolume(int volume);

	/// @brief BGMの有効/無効フラグを反転トグル切り替えする
	void ToggleBgmEnabled();

	/// @brief SEの有効/無効フラグを反転トグル切り替えする
	void ToggleSeEnabled();

private:
	SOUND_BGM now_playing_bgm_;  ///< 現在再生中のBGM種類
	SOUND_SE now_playing_se_;    ///< 直近で再生されたSE種類

	std::vector<std::pair<SOUND_BGM, int>> bgm_handle_list_; ///< ロード済みBGMハンドル管理リスト
	std::vector<std::pair<SOUND_SE, int>> se_handle_list_;   ///< ロード済みSEハンドル管理リスト

	bool is_bgm_enabled_;        ///< BGM再生が有効かどうかを示すフラグ
	bool is_se_enabled_;         ///< SE再生が有効かどうかを示すフラグ
	int bgm_volume_;             ///< BGMの音量（0〜255）
	int se_volume_;              ///< SEの音量（0〜255）
};