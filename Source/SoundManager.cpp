#include"SoundManager.h"
#include"DxLib.h"

/// @details 音量、有効化フラグ、および再生中トラックの初期状態のセットアップ
SoundManager::SoundManager()
	:now_playing_bgm_((SOUND_BGM)-1)  //初期状態は何も再生されていない状態
	, now_playing_se_((SOUND_SE)-1)    //初期状態は何も再生されていない状態
	, is_bgm_enabled_(true)
	, is_se_enabled_(true)
	, bgm_volume_(200)
	, se_volume_(220)
{
}

SoundManager::~SoundManager()
{
}

/// @details ゲーム全編で使用するBGMおよびSEファイルの一括ロードとメモリ展開
void SoundManager::Initialize()
{
	// アーキテクチャ設計：プレイ中のロードによるカクつき（処理落ち）や音ズレを防ぐため、起動時やシーン切り替え時の非同期ロード中に全音声アセットをオンメモリ化しておく
	// BGMの読み込み
	LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/sound/BGM/01_title_bgm.mp3");
	LoadBGM(SOUND_BGM::BGM_GAME, "Resource/sound/BGM/02_game_bgm.mp3"); //増やしたい場合
	LoadBGM(SOUND_BGM::BGM_RESULT, "Resource/sound/BGM/03_result_bgm.mp3");

	// SEの読み込み
	LoadSE(SOUND_SE::SE_FIRE, "Resource/sound/SE/01_fire_magic.mp3");
	LoadSE(SOUND_SE::SE_ATTACK, "Resource/sound/SE/02_normal_attack.mp3");
	LoadSE(SOUND_SE::SE_ATTACKSLIDE, "Resource/sound/SE/03_slide_attack.mp3");
	LoadSE(SOUND_SE::SE_SLIDE, "Resource/sound/SE/04_slide_move.mp3");
	LoadSE(SOUND_SE::SE_HEAL, "Resource/sound/SE/05_heal.mp3");
	
	LoadSE(SOUND_SE::SE_ATTACKSLIDE, "Resource/sound/SE/03_slide_attack.mp3");
	LoadSE(SOUND_SE::SE_JUMP, "Resource/sound/SE/06_jump.mp3");
	LoadSE(SOUND_SE::SE_WARP, "Resource/sound/SE/07_warp.mp3");
	LoadSE(SOUND_SE::SE_POWER, "Resource/sound/SE/08_powerup.mp3");
	LoadSE(SOUND_SE::SE_SHOP, "Resource/sound/SE/09_buy.mp3");
	LoadSE(SOUND_SE::SE_SELECT, "Resource/sound/SE/10_decide.mp3");
	LoadSE(SOUND_SE::SE_WINDOW, "Resource/sound/SE/11_menu.mp3");
}

/// @details メモリ上に確保されたすべてのサウンドハンドルの安全な破棄
void SoundManager::Finalize()
{
	// BGMの破棄
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		DeleteSoundMem(it->second);
	}
	// SEの破棄
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		DeleteSoundMem(it->second);
	}
}

/// @param bgm = 再生するBGMの列挙型ID, isTop = 曲の最初から強制的に再生し直すかどうかのフラグ
/// @details 指定BGMのループ再生開始、および現在再生中のBGM状態更新
void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
	if (!is_bgm_enabled_)
	{
		now_playing_bgm_ = bgm;
		return;
	}

	// UX仕様：既に同じBGMが再生中の場合は処理をスキップし、シーン遷移等で曲が不自然に途切れたり頭出しされたりするのを防ぐ
	if (now_playing_bgm_ == bgm && !isTop)
	{
		return;
	}

	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == bgm)
		{
			ChangeVolumeSoundMem(bgm_volume_, it->second);
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
			now_playing_bgm_ = bgm;
			break;
		}
	}
}

/// @param se = 再生するSEの列挙型ID
/// @details 指定SEのバックグラウンド（多重）再生開始
void SoundManager::PlaySE(SOUND_SE se)
{
	if (!is_se_enabled_)
	{
		return;
	}

	// UX仕様：アクションゲームにおいて効果音（剣の振りや被弾音）は頻繁に重複するため、DX_PLAYTYPE_BACK を指定して音の同時発音・重畳を担保する
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		if (it->first == se)
		{
			ChangeVolumeSoundMem(se_volume_, it->second);
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			now_playing_se_ = se;
			break;
		}
	}
}

/// @param bgm = 紐づける列挙型ID, filename = ファイルパス
/// @details 音声データのロード、初期音量の適用、および管理用リストへの登録
void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
	bool check = false;

	// アーキテクチャ設計：同一ファイルの重複ロードによるメモリリークや無駄なRAM消費を防ぐため、リスト内を走査して登録済みなら即座にスキップするガード処理
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == bgm)
		{
			check = true;
			break;
		}
	}
	if (check)
	{
		return;
	}

	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;
	}

	ChangeVolumeSoundMem(bgm_volume_, handle);
	bgm_handle_list_.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
}

/// @param se = 紐づける列挙型ID, filename = ファイルパス
/// @details 音声データのロード、初期音量の適用、および管理用リストへの登録
void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
	bool check = false;

	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		if (it->first == se)
		{
			check = true;
			break;
		}
	}
	if (check)
	{
		return;
	}

	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;
	}

	ChangeVolumeSoundMem(se_volume_, handle);
	se_handle_list_.push_back(std::pair<SOUND_SE, int>(se, handle));
}

/// @details 現在再生中のBGMトラックの停止処理
void SoundManager::StopBGM()
{
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == now_playing_bgm_)
		{
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);
				break;
			}
		}
	}
}

/// @param enabled = 有効化フラグ
/// @details ミュート状態の切り替え。無効化時は即座に停止し、有効化時は現在のトラックをレジューム再生する
void SoundManager::SetBgmEnabled(bool enabled)
{
	is_bgm_enabled_ = enabled;
	if (!is_bgm_enabled_)
	{
		StopBGM();
	}
	else if (now_playing_bgm_ != (SOUND_BGM)-1)
	{
		PlayBGM(now_playing_bgm_, false);
	}
}

/// @param enabled = 有効化フラグ
/// @details SEのミュート状態の切り替え
void SoundManager::SetSeEnabled(bool enabled)
{
	is_se_enabled_ = enabled;
}

/// @param volume = 設定する音量（0-255）
/// @details クランプ処理を挟んだ音量変数の更新、およびロード済みの全BGMハンドルへの即時適用
void SoundManager::SetBgmVolume(int volume)
{
	bgm_volume_ = max(0, min(255, volume));
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		ChangeVolumeSoundMem(bgm_volume_, it->second);
	}
}

/// @param volume = 設定する音量（0-255）
/// @details クランプ処理を挟んだ音量変数の更新、およびロード済みの全SEハンドルへの即時適用
void SoundManager::SetSeVolume(int volume)
{
	se_volume_ = max(0, min(255, volume));
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		ChangeVolumeSoundMem(se_volume_, it->second);
	}
}

/// @details BGMの有効
void SoundManager::ToggleBgmEnabled()
{
	SetBgmEnabled(!is_bgm_enabled_);
}

/// @details SEの有効
void SoundManager::ToggleSeEnabled()
{
	SetSeEnabled(!is_se_enabled_);
}
