#pragma once
#include"SceneManager.h"
#include"SoundManager.h"
//#include"ScoreManager.h"
//#include"NormalResultScene.h"
#include"Camera.h"
#include"Camera1.h"
//#include"ResultChange.h"
#include"ScoreManager.h"
#include"ResourceManager.h"
#include"Debugh.h"
#include"EnemySerch.h"
#include"DrawHp.h"
#include<string>
#include<vector>
#include<iostream>
#include"TutorialScene.h"
#include"LevelUp.h"
#include"EnemyManager.h"

#include"ItemManager.h"
#include"InfClassManager.h"
#include"InfClass.h"
#include"BuffManager.h"
#include"Chat.h"
#include"Save.h"
//
//ゲーム内で１つだけしか存在しないものを管理するクラス
// 例えば
// SceneManagerだったり（シーン管理関数）
// SoundManagerだったり（サウンド管理クラス)
// ResourceManagerだったり（リソース（画像やモデル）管理クラス）
// を、どこからも参照できるようにしておきたいので
// こういったクラスを作る
//
class Player3D;
class Master
{
public:
	static Player3D* mpPlayer;
	static SceneManager* mpSceneManager;  //シーンマネージャーポインタ
	static SoundManager* mpSoundManager;  //サウンドマネージャーのポインタ
	static ScoreManager* mpScoreManager;//スコアマネージャーのポインタ
	//static NormalResultScene* mpNormalResultScene;//０枚になった時のリザルトシーン
	static Camera* mpCamera;           //カメラのポインタ
	static Camera* mpCamerafa;
	static ResourceManager* mpResourceManager;
	static Debug* mpDebug;
	static EnemySerch* mpEnemySerch;
	static DrawHp* mpDrawHp;
	static TutorialScene* mpTutorial;
	static LevelUp* mpLevelUp;
	static EnemyManager* mpEnemyManager;
	static ItemManager* mpItemManager;
	static InfClassManager* mpInfClassManager;
	static BuffManager* mpBuffManager;
	static Chat* mpChat;
	static Save* mpSave;
	

	//static void SetName(char name) { Name = name; }//名前を保存しておくクラス

	static int TutorialCount;//
	static bool TutorialFlag;//ツーとリアルかどうか
	static int TutorialClearCount;//チュートリアルを終わらせるカウント
	static char NameTest[256];
	///////////////enemy////////////////////////////////////

	///////////////game////////////////
	static bool PauseOn;
	static bool StatShopClassOn;
	static bool NearShopOn;
	static bool SafePointOn;
	static bool mbSave;
	static int GameClearCount;
private: 
	
};
