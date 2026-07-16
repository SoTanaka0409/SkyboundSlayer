#pragma once
#include "SceneManager.h"
#include "SoundManager.h"
#include "Camera.h"
#include "ScoreManager.h"
#include "ResourceManager.h"
#include "Debug.h"
#include "DrawHp.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "InfClassManager.h"
#include "InfClass.h"
#include "BuffManager.h"
#include "Chat.h"
#include "Save.h"
#include <string>
#include <vector>
#include <iostream>

class Player3D;

class Master
{
public:
	static Player3D* mpPlayer;
	static SceneManager* mpSceneManager;
	static SoundManager* mpSoundManager;
	static ScoreManager* mpScoreManager;
	static Camera* mpCamera;
	static ResourceManager* mpResourceManager;
	static Debug* mpDebug;
	static DrawHp* mpDrawHp;
	static EnemyManager* mpEnemyManager;
	static ItemManager* mpItemManager;
	static InfClassManager* mpInfClassManager;
	static BuffManager* mpBuffManager;
	static Chat* mpChat;
	static Save* mpSave;

	static bool PauseOn;
	static bool StatShopClassOn;
	static bool NearShopOn;
	static bool SafePointOn;
	static bool mbSave;
	static bool CutscenePlaying;
	static int GameClearCount;
};
