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

	};

public:
    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
	SoundManager();

    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
    // デストラクタ
// [入力] なし [出力] なし [副作用] 音声データの解放
	~SoundManager();
	//初期化
    // 音声データを初期化・読み込みする
// [入力] なし [出力] なし [副作用] メモリの確保
	void Initialize();
	//終了処理
    // 音声データを破棄する
// [入力] なし [出力] なし [副作用] メモリの解放
	void Finalize();

public:       //サウンド再生系

	//BGM再生
	//SOUND_BGM...鳴らしたいBGMの種類
	//IsTop...最初から再生するかどうか
	//bool IsTop=true->デフォルトから引数と呼ばれる書き方。関数を呼び出ス際に渡さなくても認識される
    // BGMを再生する
// [入力] SOUND_BGM bgm, bool IsTop [出力] なし [副作用] 既存BGMの停止と再生
	void PlayBGM(SOUND_BGM bgm, bool IsTop = true);

	//SE再生
	//se...鳴らしたいSEの種類
    // SEを再生する
// [入力] SOUND_SE se [出力] なし [副作用] SEの再生
	void PlaySE(SOUND_SE se);

	//BGMの読み込み
    // BGMファイルを読み込む
// [入力] SOUND_BGM bgm, std::string filename [出力] なし [副作用] マップへの追加
	void LoadBGM(SOUND_BGM bgm, std::string filename);

	//SEの読み込み
    // SEファイルを読み込む
// [入力] SOUND_SE se, std::string filename [出力] なし [副作用] マップへの追加
	void LoadSE(SOUND_SE se, std::string filename);

	//BGmの停止
    // 現在のBGMを停止する
// [入力] なし [出力] なし [副作用] BGMの停止
	void StopBGM();

private:  //メンバ変数
	SOUND_BGM now_playing_bgm_;   //現在再生されているBGMの種類
	SOUND_SE now_playing_se_;     //現在再生されているSEの種類


	std::vector < std::pair<SOUND_BGM, int>>bgm_handle_list_;  //読み込んだBGMのハンドルのリスト
	std::vector<std::pair<SOUND_SE, int>>se_handle_list_;     //読み込んだSEハンドルのリスト
};