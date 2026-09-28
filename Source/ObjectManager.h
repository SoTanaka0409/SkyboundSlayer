#pragma once

#include<list>
#include<vector>
#include"Object3D.h"
#include"Object2D.h"
#include<map>


class ObjectManager
{
public:
    /// @brief コンストラクタ
	ObjectManager();

    /// @brief コンストラクタ
    /// @brief デストラクタ
	~ObjectManager();

    /// @brief オブジェクトの描画処理
	void Draw();

    /// @brief オブジェクトの更新処理
	void Update();

public:

	/// @brief 3Dオブジェクト追加
	void AddObject(Object3D* object3D);

	/// @brief 3dオブジェクトの全削除
    /// @brief 全3Dオブジェクトを削除する
	void DeleteAll3D();

	/// @brief 削除する必要のあるオブジェクトがあれば削除する
	/// @brief note:すべてのオブジェクトの更新が終わった後に呼び出す
    /// @brief 削除フラグが立っている3Dオブジェクトを削除する
	void DeleteAll3DIfNeeded();

	/// @brief 指定したたぐの２Ｄオブジェクトを取得
	/// @brief note:該当するオブジェクトが複数ある場合、最初に見つけたオブジェクトを返す
	Object3D* GetObject3DByTag(Object3D::Tag3D tag);

	/// @brief 指定したタグの２Dオブジェクトのリストを取得
	/// @brief note:該当するオブジェクトが複数ある場合、リスト化してすべてのオブジェクトを返す
	const std::vector<Object3D*>& GetObject3DListByTag(Object3D::Tag3D tag);

	////////////////////////////////////////////////////////////////////////

	 /// @brief ２Dオブジェクト追加
	void AddObject(Object2D* object2D);

	/// @brief 2dオブジェクトの全削除
    /// @brief 全2Dオブジェクトを削除する
	void DeleteAll2D();

	/// @brief 削除する必要のあるオブジェクトがあれば削除する
	/// @brief note:すべてのオブジェクトの更新が終わった後に呼び出す
    /// @brief 削除フラグが立っている2Dオブジェクトを削除する
	void DeleteAll2DIfNeeded();

	/// @brief 指定したたぐの２Ｄオブジェクトを取得
	/// @brief note:該当するオブジェクトが複数ある場合、最初に見つけたオブジェクトを返す
	Object2D* GetObject2DByTag(Object2D::Tag2D tag);

	/// @brief 指定したタグの２Dオブジェクトのリストを取得
	/// @brief note:該当するオブジェクトが複数ある場合、リスト化してすべてのオブジェクトを返す
	std::vector<Object2D*>GetObject2DListByTag(Object2D::Tag2D tag);

private:
	
	std::map<Object3D::Tag3D, std::vector<Object3D*>> cached_3d_lists_;
	bool cache_dirty_;
	std::list<Object3D*>object_3d_list_;   //3Dオブジェクトを管理するリスト
	
	std::list<Object2D*>object_2d_list_;

};

	





