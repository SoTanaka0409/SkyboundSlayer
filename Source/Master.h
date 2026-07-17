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
	// プレイヤーオブジェクトへのポインタ
	static Player3D* player_;
	// シーン管理クラスへのポインタ
	static SceneManager* scene_manager_;
	// サウンド管理クラスへのポインタ
	static SoundManager* sound_manager_;
	// スコア管理クラスへのポインタ
	static ScoreManager* score_manager_;
	// カメラ管理クラスへのポインタ
	static Camera* camera_;
	// リソース管理クラスへのポインタ
	static ResourceManager* resource_manager_;
	// デバッグ機能管理クラスへのポインタ
	static Debug* debug_;
	// HP描画管理クラスへのポインタ
	static DrawHp* draw_hp_;
	// 敵キャラクター管理クラスへのポインタ
	static EnemyManager* enemy_manager_;
	// アイテム管理クラスへのポインタ
	static ItemManager* item_manager_;
	// インフォメーションクラス（通知など）の管理ポインタ
	static InfClassManager* inf_class_manager_;
	// バフ効果管理クラスへのポインタ
	static BuffManager* buff_manager_;
	// チャット機能管理クラスへのポインタ
	static Chat* chat_;
	// セーブデータ管理クラスへのポインタ
	static Save* save_;

	// ポーズ画面が開かれているかどうかのフラグ
	static bool is_pause_on_;
	// ステータスショップが開かれているかどうかのフラグ
	static bool is_stat_shop_on_;
	// ショップNPCの近くにいるかどうかのフラグ
	static bool is_near_shop_on_;
	// セーフポイント（安全地帯）にいるかどうかのフラグ
	static bool is_safe_point_on_;
	// セーブ中かどうかのフラグ
	static bool is_save_;
	// カットシーン再生中かどうかのフラグ
	static bool is_cutscene_playing_;
	// ゲームクリア演出の進行状態カウント
	static int game_clear_count_;
};
