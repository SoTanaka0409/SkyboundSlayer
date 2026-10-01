#include"ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"


/// @brief ObjectManagerのコンストラクタ
/// @details 各種変数の初期化
ObjectManager::ObjectManager()
	: m_CacheDirty(true)
{
}


/// @brief ObjectManagerのデストラクタ
/// @details オブジェクトの破棄
ObjectManager::~ObjectManager()
{
	DeleteAll3D();
	DeleteAll2D();
}


/// @brief 全オブジェクトの状態を更新するため
/// @details 各オブジェクトのUpdate呼び出し
void ObjectManager::Update()
{

	for (std::list < Object3D*>::iterator itr = m_Object3dList.begin(); itr != m_Object3dList.end(); itr++)
	{
		(*itr)->Update();
	}

	for (std::list < Object2D*>::iterator itr = m_Object2dList.begin(); itr != m_Object2dList.end(); itr++)
	{
		(*itr)->Update();
	}
	for (std::list<Object3D*>::iterator itr = m_Object3dList.begin(); itr != m_Object3dList.end(); itr++)
	{
		VECTOR cameraPos = Master::m_Camera->GetPosition();
		VECTOR objPos = (*itr)->GetPosition();
		(*itr)->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	ColliderManager::GetInstance()->Update();

}


/// @brief 全オブジェクトを描画するため
/// @details 各オブジェクトのDraw呼び出し
void ObjectManager::Draw()
{
	
	for (auto itr = m_Object3dList.begin(); itr != m_Object3dList.end(); itr++)
	{

		if ((*itr)->IsDrawFlag() == true)
		{
			(*itr)->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (auto itr = m_Object2dList.begin(); itr != m_Object2dList.end(); itr++)
	{

		if ((*itr)->IsDrawFlag() == true)
		{
			(*itr)->Draw();

		}
	}
	
}

/// @brief オブジェクトをリストに追加するため
/// @param オブジェクトポインタ
/// @details リストへの要素追加
void ObjectManager::AddObject(Object3D* object3D)
{
	m_Object3dList.push_back(object3D);
	m_CacheDirty = true;


}


/// @brief 全ての3Dオブジェクトを削除するため
/// @details 3Dオブジェクトリストのクリア
void ObjectManager::DeleteAll3D()
{
	for (auto itr = m_Object3dList.begin(); itr != m_Object3dList.end(); itr++)
	{
		delete *itr;
	}
	m_Object3dList.clear();
	m_CacheDirty = true;
}

Object3D* 
/// @brief 特定のタグを持つ3Dオブジェクトを取得するため
/// @param Object3D::Tag3D tag
/// @return Object3D*
ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		m_Object3dList.begin(),
		m_Object3dList.end(),
		[&](Object3D* obj) {return obj->GetTag() == tag; }
	);
	if (itr != m_Object3dList.end())
	{
		return (*itr);
	}
	return nullptr;
}

const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	if (m_CacheDirty)
	{
		m_Cached3dLists.clear();
		for (auto itr = m_Object3dList.begin(); itr != m_Object3dList.end(); itr++)
		{
			m_Cached3dLists[(*itr)->GetTag()].push_back(*itr);
		}
		m_CacheDirty = false;
	}
	return m_Cached3dLists[tag];
}


/// @brief 削除フラグが立っている3Dオブジェクトを削除するため
/// @details リストからの要素削除
void ObjectManager::DeleteAll3DIfNeeded()
{
	for (auto itr = m_Object3dList.begin(); itr != m_Object3dList.end();)
	{
		if ((*itr)->IsDeleteFlag() == true)
		{
			Object3D* temp = *itr;

			itr = m_Object3dList.erase(itr);
			m_CacheDirty = true;

			delete temp;
			temp = nullptr;
		}
		else
		{
			itr++;
		}
	}

}


/// @brief オブジェクトをリストに追加するため
/// @param オブジェクトポインタ
/// @details リストへの要素追加
void ObjectManager::AddObject(Object2D* object2D)
{
	m_Object2dList.push_back(object2D);
}


/// @brief 全ての2Dオブジェクトを削除するため
/// @details 2Dオブジェクトリストのクリア
void ObjectManager::DeleteAll2D()
{
	Master::m_InfClassManager->LogList.clear();
	for (auto itr = m_Object2dList.begin(); itr != m_Object2dList.end();)
	{
		Object2D* temp = *itr;

		itr = m_Object2dList.erase(itr);

		delete temp;
		temp = nullptr;
	}
}


/// @brief 削除フラグが立っている2Dオブジェクトを削除するため
/// @details リストからの要素削除
void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = m_Object2dList.begin(); itr != m_Object2dList.end();)
	{
		if ((*itr)->IsDeleteFlag() == true)
		{
			Object2D* temp = *itr;

			itr = m_Object2dList.erase(itr);

			delete temp;
			temp = nullptr;
		}
		else
		{
			itr++;
		}

	}
}

Object2D* 
/// @brief 特定のタグを持つ2Dオブジェクトを取得するため
/// @param Object2D::Tag2D tag
/// @return Object2D*
ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
	auto itr = std::find_if(
		m_Object2dList.begin(),
		m_Object2dList.end(),
		[&](Object2D* obj) {return obj->GetTag() == tag; }
	);

	if (itr != m_Object2dList.end())
	{
		return (*itr);
	}
	return nullptr;
}

std::vector<Object2D*>ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	std::vector<Object2D*>ret;

	for (auto itr = m_Object2dList.begin(); itr != m_Object2dList.end(); itr++)
	{
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}

	return ret;
}
