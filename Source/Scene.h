#pragma once
#include"DxLib.h"

//前方宣言
class ObjectManager;
class ColliderManager;
//
//シーンの基底クラス
// 
// タイトル、ゲーム、リザルト画面など
// 画面を作る際はこのクラスを継承する
//
class Scene
{
public:

	Scene();

	virtual ~Scene();

	//初期化
	virtual void Initialize() = 0;
	//更新
	virtual void Update();
	//描画
	virtual void Draw();
	//終了処理
	virtual void Finalize() = 0;


	//オブジェクトマネージャーの取得
	ObjectManager* GetObjectManager() { return mpObjectManager; }
	ColliderManager* GetCollisionManager() { return mpColliderManager; }

private:
	ObjectManager* mpObjectManager;  //オブジェクト管理クラスのポインタ
	ColliderManager* mpColliderManager;
};