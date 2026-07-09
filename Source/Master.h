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
#include"Debug.h"
#include"EnemySearch.h"
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
//繧ｲ繝ｼ繝蜀・〒・代▽縺縺代＠縺句ｭ伜惠縺励↑縺・ｂ縺ｮ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
// 萓九∴縺ｰ
// SceneManager縺縺｣縺溘ｊ・医す繝ｼ繝ｳ邂｡逅・未謨ｰ・・
// SoundManager縺縺｣縺溘ｊ・医し繧ｦ繝ｳ繝臥ｮ｡逅・け繝ｩ繧ｹ)
// ResourceManager縺縺｣縺溘ｊ・医Μ繧ｽ繝ｼ繧ｹ・育判蜒上ｄ繝｢繝・Ν・臥ｮ｡逅・け繝ｩ繧ｹ・・
// 繧偵√←縺薙°繧峨ｂ蜿ら・縺ｧ縺阪ｋ繧医≧縺ｫ縺励※縺翫″縺溘＞縺ｮ縺ｧ
// 縺薙≧縺・▲縺溘け繝ｩ繧ｹ繧剃ｽ懊ｋ
//
class Player3D;
class Master
{
public:
	static Player3D* mpPlayer;
	static SceneManager* mpSceneManager;  //繧ｷ繝ｼ繝ｳ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ繝昴う繝ｳ繧ｿ
	static SoundManager* mpSoundManager;  //繧ｵ繧ｦ繝ｳ繝峨・繝阪・繧ｸ繝｣繝ｼ縺ｮ繝昴う繝ｳ繧ｿ
	static ScoreManager* mpScoreManager;//繧ｹ繧ｳ繧｢繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｮ繝昴う繝ｳ繧ｿ
	//static NormalResultScene* mpNormalResultScene;//・先椢縺ｫ縺ｪ縺｣縺滓凾縺ｮ繝ｪ繧ｶ繝ｫ繝医す繝ｼ繝ｳ
	static Camera* mpCamera;           //繧ｫ繝｡繝ｩ縺ｮ繝昴う繝ｳ繧ｿ
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
	

	//static void SetName(char name) { Name = name; }//蜷榊燕繧剃ｿ晏ｭ倥＠縺ｦ縺翫￥繧ｯ繝ｩ繧ｹ

	static int TutorialCount;//
	static bool TutorialFlag;//繝・・縺ｨ繝ｪ繧｢繝ｫ縺九←縺・°
	static int TutorialClearCount;//繝√Η繝ｼ繝医Μ繧｢繝ｫ繧堤ｵゅｏ繧峨○繧九き繧ｦ繝ｳ繝・
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
