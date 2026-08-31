#pragma once
#include "DxLib.h"

// 前方宣言
class ObjectManager;
class ColliderManager;

/// @brief 全ての画面（タイトル、ゲーム本編、リザルト等）の基底となる抽象シーンクラス
/// @details 新しいシーン画面を作成する場合は本クラスを継承して実装する
class Scene
{
public:
	/// @brief Sceneクラスのコンストラクタ
	Scene();

	/// @brief Sceneクラスの仮想デストラクタ
	virtual ~Scene();

	/// @brief シーンの初期化処理（純粋仮想関数）
	/// @details リソースの読み込みや各オブジェクトの生成を行う
	virtual void Initialize() = 0;

	/// @brief シーンの毎フレーム更新処理
	/// @details 登録されたオブジェクトマネージャー等の更新を呼び出す
	virtual void Update();

	/// @brief シーンの描画処理
	/// @details 登録されたオブジェクトマネージャー等の描画を呼び出す
	virtual void Draw();

	/// @brief シーンの終了・解放処理（純粋仮想関数）
	/// @details シーン破棄時のリソース解放処理を行う
	virtual void Finalize() = 0;

	/// @brief シーンに紐づくObjectManagerのポインタを取得する
	/// @return ObjectManager* オブジェクト管理クラスへのポインタ
	ObjectManager* GetObjectManager() { return object_manager_; }

	/// @brief シーンに紐づくColliderManagerのポインタを取得する
	/// @return ColliderManager* コライダー管理クラスへのポインタ
	ColliderManager* GetCollisionManager() { return collider_manager_; }

private:
	ObjectManager* object_manager_;     ///< このシーンに所属する3D/2Dオブジェクトを一括管理するマネージャー
	ColliderManager* collider_manager_; ///< このシーンの衝突判定を一括管理するマネージャー
};