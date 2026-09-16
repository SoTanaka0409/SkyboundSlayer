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
#include <iostream>
#include <string>
#include <vector>

class Player3D;

/// @brief 深いクラス階層間でのポインタバケツリレーを解消し、各種マネージャーやプレイヤー情報へどこからでもアクセス可能にするサービスロケータークラス
class Master
{
public:
	static Player3D* player_;                 ///< 敵AIの追従やカメラ制御、UI更新などから即座にアクセスするための自プレイヤー参照ポインタ

	static SceneManager* scene_manager_;     ///< シーン遷移・生成・破棄を管理するマネージャー
	static SoundManager* sound_manager_;     ///< BGM・SE再生を統合管理するサウンドマネージャー
	static ScoreManager* score_manager_;     ///< スコアおよびハイスコアの保持・読み込みを管理するマネージャー
	static Camera* camera_;                   ///< 三人称カメラおよびカットシーン演出制御用カメラポインタ
	static ResourceManager* resource_manager_; ///< モデル・テクスチャ・音声リソースのキャッシュ管理マネージャー
	static Debug* debug_;                     ///< デバッグ情報の描画やデバッグ機能のフラグ管理ポインタ
	static DrawHp* draw_hp_;                 ///< HPゲージ等のUI描画処理マネージャー
	static EnemyManager* enemy_manager_;     ///< 敵オブジェクトのスポーン・管理を行うマネージャー
	static ItemManager* item_manager_;       ///< 所持アイテム・使用効果を管理するマネージャー
	static InfClassManager* inf_class_manager_; ///< 画面上のログ・インフォメーションUI管理マネージャー
	static BuffManager* buff_manager_;       ///< プレイヤーのバフ・デバフ効果を管理するマネージャー
	static Chat* chat_;                       ///< チャットUI・メッセージ描画マネージャー
	static Save* save_;                       ///< セーブデータ入出力を担当するマネージャー

	static int hit_stop_timer_;
	static bool is_pause_on_;                 ///< メニュー展開時にアクターの更新処理を停止・ポーズするためのフラグ
	static bool is_stat_shop_on_;             ///< ステータス強化ショップ画面が開いているかを示すフラグ
	static bool is_near_shop_on_;             ///< プレイヤーがショップNPCの近くにいるかを示す判定フラグ
	static bool is_safe_point_on_;            ///< セーフエリア・安全地帯にいるかを示す判定フラグ
	static bool is_save_;                     ///< セーブ処理中・要求が発生しているかを示すフラグ

	static bool is_cutscene_playing_;         ///< シネマティックカットシーン再生中につきプレイヤー操作をロックするためのフラグ

	static int game_clear_count_;             ///< ボス撃破後の演出やリザルト画面遷移までの演出タイマーカウンター
};