#include"SoundManager.h"
#include"DxLib.h"


/*
 * 目的（SoundManagerのコンストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
SoundManager::SoundManager()
	:now_playing_bgm_((SOUND_BGM)-1)  //初期状態は何も再生されていない状態
	, now_playing_se_((SOUND_SE)-1)    //初期状態は何も再生されていない状態
{


}

/*
 * 目的（SoundManagerのデストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
SoundManager::~SoundManager()
{

}


/*
 * 目的（すべてのサウンドリソースを初期化・ロードするため）
 * [入力] なし
 * [出力] なし
 * [副作用] サウンドファイルのメモリ展開
 */
void SoundManager::Initialize()
{
	//BGMの読み込み
	LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/BGM/natsuyasuminotanken.mp3");
	LoadBGM(SOUND_BGM::BGM_GAME, "Resource/BGM/MusMus-BGM-081.mp3");//増やしたい場合
	LoadBGM(SOUND_BGM::BGM_RESULT, "Resource/BGM/MusMus-BGM-084.mp3");
	

	//SEの読み込み
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




/*
 * 目的（サウンドリソースを解放するため）
 * [入力] なし
 * [出力] なし
 * [副作用] サウンドハンドルの削除
 */
void SoundManager::Finalize()
{
	//BGmの破棄」
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		DeleteSoundMem(it->second);
	}
	//Seの破棄
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		DeleteSoundMem(it->second);
	}
}


/*
 * 目的（指定したBGMを再生するため）
 * [入力] SOUND_BGM bgm, bool IsTop
 * [出力] なし
 * [副作用] BGMの再生と他のBGMの停止
 */
void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
	//現在再生されているBGMかつ、最初からの再生出ないのならreturnする
	if (now_playing_bgm_ == bgm && !isTop)
	{
		return;
	}

	//BGmの破棄」
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == bgm)
		{
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
			//現在の再生種類を更新
			now_playing_bgm_ = bgm;

			break;
		}
	}
}


/*
 * 目的（指定したSEを再生するため）
 * [入力] SOUND_SE se
 * [出力] なし
 * [副作用] SEの再生
 */
void SoundManager::PlaySE(SOUND_SE se)
{
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		if (it->first == se)
		{
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			//現在の再生種類を更新
			now_playing_se_ = se;

			break;
		}
	}
}


/*
 * 目的（BGMファイルを読み込んでマップに追加するため）
 * [入力] SOUND_BGM bgm, std::string filename
 * [出力] なし
 * [副作用] 音声データのロード
 */
void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
	bool check = false;//重複して読み込んでいるかどうか
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == bgm)
		{
			check = true;

			break;
		}
	}
	//重複して読み込まれていたら何もしない
	if (check)
	{
		return;
	}

	//ファイル読み込み
	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;  //読み込み失敗したら何もしない
	}

	//読み込んだハンドルをリストに追加
	bgm_handle_list_.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
}


/*
 * 目的（SEファイルを読み込んでマップに追加するため）
 * [入力] SOUND_SE se, std::string filename
 * [出力] なし
 * [副作用] 音声データのロード
 */
void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
	bool check = false;//重複して読み込んでいるかどうか
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		if (it->first == se)
		{
			check = true;

			break;
		}
	}
	//重複して読み込まれていたら何もしない
	if (check)
	{
		return;
	}

	//ファイル読み込み
	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;  //読み込み失敗したら何もしない
	}

	//読み込んだハンドルをリストに追加
	se_handle_list_.push_back(std::pair<SOUND_SE, int>(se, handle));
}

/*
 * 目的（再生中のBGMを停止するため）
 * [入力] なし
 * [出力] なし
 * [副作用] BGMの停止
 */
void SoundManager::StopBGM()
{
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == now_playing_bgm_)
		{
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);//BGmを停止
				break;
			}
		}
	}
}