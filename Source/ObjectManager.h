#pragma once

#include<list>
#include<vector>
#include"Object3D.h"
#include"Object2D.h"
#include<map>


class ObjectManager
{
public:
    // コンストラクタ
	ObjectManager();

    // コンストラクタ
    // デストラクタ
	~ObjectManager();

    // オブジェクトの描画処理
	void Draw();

    // オブジェクトの更新処理
	void Update();

public:

	//3Dオブジェクト追加
	void AddObject(Object3D* object3D);

	//3dオブジェクトの全削除
    // 全3Dオブジェクトを削除する
	void DeleteAll3D();

	//削除する必要のあるオブジェクトがあれば削除する
	//note:すべてのオブジェクトの更新が終わった後に呼び出す
    // 削除フラグが立っている3Dオブジェクトを削除する
	void DeleteAll3DIfNeeded();

	//指定したたぐの２Ｄオブジェクトを取得
	//note:該当するオブジェクトが複数ある場合、最初に見つけたオブジェクトを返す
	Object3D* GetObject3DByTag(Object3D::Tag3D tag);

	//指定したタグの２Dオブジェクトのリストを取得
	//note:該当するオブジェクトが複数ある場合、リスト化してすべてのオブジェクトを返す
	const std::vector<Object3D*>& GetObject3DListByTag(Object3D::Tag3D tag);

	////////////////////////////////////////////////////////////////////////

	 //２Dオブジェクト追加
	void AddObject(Object2D* object2D);

	//2dオブジェクトの全削除
    // 全2Dオブジェクトを削除する
	void DeleteAll2D();

	//削除する必要のあるオブジェクトがあれば削除する
	//note:すべてのオブジェクトの更新が終わった後に呼び出す
    // 削除フラグが立っている2Dオブジェクトを削除する
	void DeleteAll2DIfNeeded();

	//指定したたぐの２Ｄオブジェクトを取得
	//note:該当するオブジェクトが複数ある場合、最初に見つけたオブジェクトを返す
	Object2D* GetObject2DByTag(Object2D::Tag2D tag);

	//指定したタグの２Dオブジェクトのリストを取得
	//note:該当するオブジェクトが複数ある場合、リスト化してすべてのオブジェクトを返す
	std::vector<Object2D*>GetObject2DListByTag(Object2D::Tag2D tag);

private:
	
	std::map<Object3D::Tag3D, std::vector<Object3D*>> cached_3d_lists_;
	bool cache_dirty_;
	std::list<Object3D*>object_3d_list_;   //3Dオブジェクトを管理するリスト
	
	std::list<Object2D*>object_2d_list_;

};

	





