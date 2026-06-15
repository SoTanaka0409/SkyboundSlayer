#pragma once
#include"SceneManager.h"
#include"SoundManager.h"
#include"WeaponManager.h"
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


//ゲーム内で１つだけしか存在しないものを管理するクラス
// 例えば
// SceneManagerだったり（シーン管理関数）
// SoundManagerだったり（サウンド管理クラス)
// ResourceManagerだったり（リソース（画像やモデル）管理クラス）
// を、どこからも参照できるようにしておきたいので
// こういったクラスを作る
//
class Master
{
public:
	static SceneManager* mpSceneManager;  //シーンマネージャーポインタ
	static SoundManager* mpSoundManager;  //サウンドマネージャーのポインタ
	static ScoreManager* mpScoreManager;//スコアマネージャーのポインタ
	//static NormalResultScene* mpNormalResultScene;//０枚になった時のリザルトシーン
	static Camera* mpCamera;           //カメラのポインタ
	static Camera* mpCamerafa;
	static WeaponManager* mpWeaponManager;
	static ResourceManager* mpResourceManager;
	static Debug* mpDebug;
	static EnemySerch* mpEnemySerch;
	static DrawHp* mpDrawHp;
	static TutorialScene* mpTutorial;

	//static void SetName(char name) { Name = name; }//名前を保存しておくクラス

	static int TutorialCount;//
	static bool TutorialFlag;//ツーとリアルかどうか
	static int TutorialClearCount;//チュートリアルを終わらせるカウント
	static char NameTest[256];
	///////////////enemy////////////////////////////////////
	static int gameClearCount_Normal;//ノーマルモードをクリアするために倒す数
	static int gameClearCount_Hard;//ハードモードをクリアするために倒す数
	static int ClearEneCountReset_Normal;//りせっto 
	static int ClearEneCountReset_Hard;//りせっto
	/////////////////dino/////////////////////
	static int gameDinoCount;//倒した恐竜の数
	static int ClearDinocountReset;//倒した恐竜の数のリセット処理

	static bool RideOn;//恐竜に乗っているか
	
private: 
	
};