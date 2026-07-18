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

// 深いクラス階層間でのポインタのバケツリレーを解消するため、システム群やプレイヤーへのグローバルな参照を保持するサービスロケーター
class Master
{
public:
	// 敵AIの追従やカメラ制御、UI更新など、あらゆる場所からプレイヤーの現在状態へ即座にアクセスするために保持する
	static Player3D* player_;

	// インスタンス生成・破棄のライフサイクル管理はメインループ等に任せ、純粋な「アクセス窓口」としてのみ機能させるコアシステム群
	static SceneManager* scene_manager_;
	static SoundManager* sound_manager_;
	static ScoreManager* score_manager_;
	static Camera* camera_;
	static ResourceManager* resource_manager_;
	static Debug* debug_;
	static DrawHp* draw_hp_;
	static EnemyManager* enemy_manager_;
	static ItemManager* item_manager_;
	static InfClassManager* inf_class_manager_;
	static BuffManager* buff_manager_;
	static Chat* chat_;
	static Save* save_;

	// メニュー展開やショップ利用時など、各アクターのUpdate処理を安全にフリーズ（スキップ）させるための実行制御フラグ
	static bool is_pause_on_;
	static bool is_stat_shop_on_;
	static bool is_near_shop_on_;
	static bool is_safe_point_on_;
	static bool is_save_;

	// カットシーン（シネマティック演出）中にプレイヤーの操作権を剥奪し、意図せぬ移動や進行を防ぐためのロックフラグ
	static bool is_cutscene_playing_;

	// ボス撃破時のスローモーションや爆発など、リザルト画面遷移までの「余韻（カタルシス）」の時間を管理するカウンター
	static int game_clear_count_;
};