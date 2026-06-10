#pragma once
#include<string>
#include<vector>

class SoundManager
{
public:     //enum,strucyなどの定義

	//BGMの種類
	enum SOUND_BGM
	{
		BGM_TITLE = 0, //タイトル画面　
		BGM_GAME,   //ゲーム画面
		BGM_RESULT,  //リザルト画面

	};

	//sEの種類
	enum SOUND_SE
	{
		SE_FIRE, //決定音
		SE_ATTACK,
		SE_JUMP,
		SE_SLIDE,
		SE_ATTACKSLIDE,
		SE_SHOP,
		SE_HEAL,
		SE_WARP,
		SE_POWER,
		SE_SELECT,
		SE_WINDOW,
		SE_LEVELUP,

	};

public:
	SoundManager();

	~SoundManager();
	//初期化
	void Initialize();
	//終了処理
	void Finalize();

public:       //サウンド再生系

	//BGM再生
	//SOUND_BGM...鳴らしたいBGMの種類
	//IsTop...最初から再生するかどうか
	//bool IsTop=true->デフォルトから引数と呼ばれる書き方。関数を呼び出ス際に渡さなくても認識される
	void PlayBGM(SOUND_BGM bgm, bool IsTop = true);

	//SE再生
	//se...鳴らしたいSEの種類
	void PlaySE(SOUND_SE se);

	//BGMの読み込み
	void LoadBGM(SOUND_BGM bgm, std::string filename);

	//SEの読み込み
	void LoadSE(SOUND_SE se, std::string filename);

	//BGmの停止
	void StopBGM();

private:  //メンバ変数
	SOUND_BGM mnNowPlayingBgm;   //現在再生されているBGMの種類
	SOUND_SE mnNowPlayingSe;     //現在再生されているSEの種類


	std::vector < std::pair<SOUND_BGM, int>>mnBgmHandleList;  //読み込んだBGMのハンドルのリスト
	std::vector<std::pair<SOUND_SE, int>>mnSeHandleList;     //読み込んだSEハンドルのリスト
};