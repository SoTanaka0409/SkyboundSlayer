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
	static Player3D* m_Player;                 ///< 敵AIの追従やカメラ制御、UI更新などから即座にアクセスするための自プレイヤー参照ポインタ

	static SceneManager* m_SceneManager;     ///< シーン遷移・生成・破棄を管理するマネージャー
	static SoundManager* m_SoundManager;     ///< BGM・SE再生を統合管理するサウンドマネージャー
	static ScoreManager* m_ScoreManager;     ///< スコアおよびハイスコアの保持・読み込みを管理するマネージャー
	static Camera* m_Camera;                   ///< 三人称カメラおよびカットシーン演出制御用カメラポインタ
	static ResourceManager* m_ResourceManager; ///< モデル・テクスチャ・音声リソースのキャッシュ管理マネージャー
	static Debug* m_Debug;                     ///< デバッグ情報の描画やデバッグ機能のフラグ管理ポインタ
	static DrawHp* m_DrawHp;                 ///< HPゲージ等のUI描画処理マネージャー
	static EnemyManager* m_EnemyManager;     ///< 敵オブジェクトのスポーン・管理を行うマネージャー
	static ItemManager* m_ItemManager;       ///< 所持アイテム・使用効果を管理するマネージャー
	static InfClassManager* m_InfClassManager; ///< 画面上のログ・インフォメーションUI管理マネージャー
	static BuffManager* m_BuffManager;       ///< プレイヤーのバフ・デバフ効果を管理するマネージャー
	static Chat* m_Chat;                       ///< チャットUI・メッセージ描画マネージャー
	static Save* m_Save;                       ///< セーブデータ入出力を担当するマネージャー

	static int m_HitStopTimer;
	static bool m_IsPauseOn;                 ///< メニュー展開時にアクターの更新処理を停止・ポーズするためのフラグ
	static bool m_IsStatShopOn;             ///< ステータス強化ショップ画面が開いているかを示すフラグ
	static bool m_IsNearShopOn;             ///< プレイヤーがショップNPCの近くにいるかを示す判定フラグ
	static bool m_IsSafePointOn;            ///< セーフエリア・安全地帯にいるかを示す判定フラグ
	static bool m_IsSave;                     ///< セーブ処理中・要求が発生しているかを示すフラグ

	static bool m_IsCutscenePlaying;         ///< シネマティックカットシーン再生中につきプレイヤー操作をロックするためのフラグ

	static int m_GameClearCount;             ///< ボス撃破後の演出やリザルト画面遷移までの演出タイマーカウンター

	static bool m_IsQuitConfirm;             ///< ESC終了確認ダイアログ表示中フラグ
	static int  m_QuitConfirmTimer;          ///< ESC連打防止ロックタイマー（60 = 1秒 @ 60fps、この間は2回目のESCを受け付けない）
};
