#include"SoundManager.h"
#include"DxLib.h"

SoundManager::SoundManager()
	:mnNowPlayingBgm((SOUND_BGM)-1)  //初期状態は何も再生されていない状態
	, mnNowPlayingSe((SOUND_SE)-1)    //初期状態は何も再生されていない状態
{


}
SoundManager::~SoundManager()
{

}

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
	LoadSE(SOUND_SE::SE_LEVELUP, "Resource/SE/levelUp.mp3");
}



void SoundManager::Finalize()
{
	//BGmの破棄」
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}
	//Seの破棄
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}
}

void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
	//現在再生されているBGMかつ、最初からの再生出ないのならreturnする
	if (mnNowPlayingBgm == bgm && !isTop)
	{
		return;
	}

	//BGmの破棄」
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == bgm)
		{
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
			//現在の再生種類を更新
			mnNowPlayingBgm = bgm;

			break;
		}
	}
}

void SoundManager::PlaySE(SOUND_SE se)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			//現在の再生種類を更新
			mnNowPlayingSe = se;

			break;
		}
	}
}

void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
	bool check = false;//重複して読み込んでいるかどうか
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
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
	mnBgmHandleList.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
}

void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
	bool check = false;//重複して読み込んでいるかどうか
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
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
	mnSeHandleList.push_back(std::pair<SOUND_SE, int>(se, handle));
}
void SoundManager::StopBGM()
{
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == mnNowPlayingBgm)
		{
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);//BGmを停止
				break;
			}
		}
	}
}