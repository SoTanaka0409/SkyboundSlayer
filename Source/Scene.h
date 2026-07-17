#pragma once
#include"DxLib.h"

//前方宣言
class ObjectManager;
class ColliderManager;
//シーンの基底クラス
// タイトル、ゲーム、リザルト画面など
// 画面を作る際はこのクラスを継承する
class Scene
{
public:

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	Scene();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual ~Scene();

	//初期化
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual void Initialize() = 0;
	//更新
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual void Update();
	//描画
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual void Draw();
	//終了処理
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	virtual void Finalize() = 0;


	//オブジェクトマネージャーの取得
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	ObjectManager* GetObjectManager() { return object_manager_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    	ColliderManager* GetCollisionManager() { return collider_manager_; }

private:
	ObjectManager* object_manager_;  //オブジェクト管理クラスのポインタ
	ColliderManager* collider_manager_;
};