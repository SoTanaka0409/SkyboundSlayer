#include"SoundManager.h"
#include"DxLib.h"

// 入力：なし
// 出力：なし
// 副作用：音量、有効化フラグ、および再生中トラックの初期状態のセットアップ
SoundManager::SoundManager()
	:now_playing_bgm_((SOUND_BGM)-1)  //初期状態は何も再生されていない状態
	, now_playing_se_((SOUND_SE)-1)    //初期状態は何も再生されていない状態
	, is_bgm_enabled_(true)
	, is_se_enabled_(true)
	, bgm_volume_(200)
	, se_volume_(220)
{
}

// 入力：なし
// 出力：なし
// 副作用：なし
SoundManager::~SoundManager()
{
}

// 入力：なし
// 出力：なし
// 副作用：ゲーム全編で使用するBGMおよびSEファイルの一括ロードとメモリ展開
void SoundManager::Initialize()
{
	// アーキテクチャ設計：プレイ中のロードによるカクつき（処理落ち）や音ズレを防ぐため、起動時やシーン切り替え時の非同期ロード中に全音声アセットをオンメモリ化しておく
	// BGMの読み込み
	LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/BGM/natsuyasuminotanken.mp3");
	LoadBGM(SOUND_BGM::BGM_GAME, "Resource/BGM/MusMus-BGM-081.mp3"); //増やしたい場合
	LoadBGM(SOUND_BGM::BGM_RESULT, "Resource/BGM/MusMus-BGM-084.mp3");

	// SEの読み込み
	LoadSE(SOUND_SE::SE_FIRE, "Resource/SE/se_fire_magic01.mp3");
	LoadSE(SOUND_SE::SE_ATTACK, "Resource/SE/se_swing13-1.mp3");
	LoadSE(SOUND_SE::SE_ATTACKSLIDE, "Resource/SE/se_sword6.mp3");
	LoadSE(SOUND_SE::SE_SLIDE, "Resource/SE/バタンと倒れる.mp3");
	LoadSE(SOUND_SE::SE_HEAL, "Resource/SE/回復魔法2.mp3");
	
	LoadSE(SOUND_SE::SE_ATTACKSLIDE, "Resource/SE/se_sword6.mp3");
	LoadSE(SOUND_SE::SE_JUMP, "Resource/SE/ジャンプ.mp3");
	LoadSE(SOUND_SE::SE_WARP, "Resource/SE/ワープ.mp3");
	LoadSE(SOUND_SE::SE_POWER, "Resource/SE/ステータス上昇魔法2.mp3");
	LoadSE(SOUND_SE::SE_SHOP, "Resource/SE/レジスターで精算.mp3");
	LoadSE(SOUND_SE::SE_SELECT, "Resource/SE/決定ボタンを押す7.mp3");
	LoadSE(SOUND_SE::SE_WINDOW, "Resource/SE/メニューを開く4.mp3");
}

// 入力：なし
// 出力：なし
// 副作用：メモリ上に確保されたすべてのサウンドハンドルの安全な破棄
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

// 入力：bgm = 再生するBGMの列挙型ID, isTop = 曲の最初から強制的に再生し直すかどうかのフラグ
// 出力：なし
// 副作用：指定BGMのループ再生開始、および現在再生中のBGM状態更新
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

// 入力：se = 再生するSEの列挙型ID
// 出力：なし
// 副作用：指定SEのバックグラウンド（多重）再生開始
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

// 入力：bgm = 紐づける列挙型ID, filename = ファイルパス
// 出力：なし
// 副作用：音声データのロード、初期音量の適用、および管理用リストへの登録
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

// 入力：se = 紐づける列挙型ID, filename = ファイルパス
// 出力：なし
// 副作用：音声データのロード、初期音量の適用、および管理用リストへの登録
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

// 入力：なし
// 出力：なし
// 副作用：現在再生中のBGMトラックの停止処理
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

// 入力：enabled = 有効化フラグ
// 出力：なし
// 副作用：ミュート状態の切り替え。無効化時は即座に停止し、有効化時は現在のトラックをレジューム再生する
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

// 入力：enabled = 有効化フラグ
// 出力：なし
// 副作用：SEのミュート状態の切り替え
void SoundManager::SetSeEnabled(bool enabled)
{
	is_se_enabled_ = enabled;
}

// 入力：volume = 設定する音量（0〜255）
// 出力：なし
// 副作用：クランプ処理を挟んだ音量変数の更新、およびロード済みの全BGMハンドルへの即時適用
void SoundManager::SetBgmVolume(int volume)
{
	bgm_volume_ = max(0, min(255, volume));
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		ChangeVolumeSoundMem(bgm_volume_, it->second);
	}
}

// 入力：volume = 設定する音量（0〜255）
// 出力：なし
// 副作用：クランプ処理を挟んだ音量変数の更新、およびロード済みの全SEハンドルへの即時適用
void SoundManager::SetSeVolume(int volume)
{
	se_volume_ = max(0, min(255, volume));
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		ChangeVolumeSoundMem(se_volume_, it->second);
	}
}

// 入力：なし
// 出力：なし
// 副作用：BGMの有効/無効のトグル切り替え
void SoundManager::ToggleBgmEnabled()
{
	SetBgmEnabled(!is_bgm_enabled_);
}

// 入力：なし
// 出力：なし
// 副作用：SEの有効/無効のトグル切り替え
void SoundManager::ToggleSeEnabled()
{
	SetSeEnabled(!is_se_enabled_);
}