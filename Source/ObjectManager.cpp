#include"ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

ObjectManager::ObjectManager()
	: mCacheDirty(true)
{
}

ObjectManager::~ObjectManager()
{
	DeleteAll3D();
	DeleteAll2D();
}

//譖ｴ譁ｰ
void ObjectManager::Update()
{
	//2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｮ譖ｴ譁ｰ
	//mObject2DList.begin()...繝ｪ繧ｹ繝域姶髣倥ｮ隕∫ｴ縺ｸ縺ｮ繧､繝繝ｬ繝ｼ繧ｿ繝ｼ繧貞叙蠕励☆繧
	//mObject2DList.end()...繝ｪ繧ｹ繝医ｮ譛ｫ蟆ｾ縺ｮ隕∫ｴ縺ｸ縺ｮ繧､繝繝ｬ繝ｼ繧ｿ繝ｼ繧貞叙蠕励☆繧
	//繧､繝繝ｬ繝ｼ繧ｿ繝ｼ縺ｨ縺ｯｼ溘ゅゅＴtd::List縺ｮ隕∫ｴ縺ｮ縺薙→繧偵＆縺.
	//繝ｪ繧ｹ繝医ｮ蛻ｩ轤ｹ...隕∫ｴ縺ｨ隕∫ｴ縺ｮ髢薙↓譁ｰ縺励＞隕∫ｴ繧貞ｷｮ縺苓ｾｼ繧縺薙→縺悟ｮｹ譏薙↓縺ｧ縺阪ｋ
	//std::List...蜿梧婿邉ｻ騾｣邨舌Μ繧ｹ繝(隕∫ｴ縺ｮ蜑榊ｾ後ｮ隕∫ｴ縺ｫ繧｢繧ｯ繧ｻ繧ｹ縺吶ｋ縺薙→縺檎ｰ｡蜊)

	for (std::list < Object3D*>::iterator itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		(*itr)->Update();
	}

	for (std::list < Object2D*>::iterator itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		(*itr)->Update();
	}
	// 3D縺ｮ譖ｴ譁ｰ縺檎ｵゅｏ縺｣縺滓凾轤ｹ縺ｧ繧ｫ繝｡繝ｩ縺ｨ縺ｮ霍晞屬繧定ｨ育ｮ励☆繧
	for (std::list<Object3D*>::iterator itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		VECTOR cameraPos = Master::mpCamera->GetPosition();
		VECTOR objPos = (*itr)->GetPosition();
		(*itr)->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	// 蠖薙◆繧雁愛螳壹が繝悶ず繧ｧ繧ｯ繝医ｮ譖ｴ譁ｰ
	ColliderManager::GetInstance()->Update();

}

//謠冗判
void ObjectManager::Draw()
{
	//auto...蝙区耳隲悶ゑｼ昴ｈ繧雁承蛛ｴ縺ｮ蝙九ｒ謗ｨ貂ｬ縺励※縺上ｌ繧倶ｾｿ蛻ｩ縺ｪ螂ｴ
	//atd::List<Object2D*>::iterator==auto
	
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{

		if ((*itr)->IsDrawFlag() == true)
		{
			(*itr)->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{

		if ((*itr)->IsDrawFlag() == true)
		{
			(*itr)->Draw();

		}
	}
	
}
void ObjectManager::AddObject(Object3D* object3D)
{
	mObject3DList.push_back(object3D);
	mCacheDirty = true;


}

//3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｮ蜈ｨ蜑企勁
void ObjectManager::DeleteAll3D()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		delete *itr;
	}
	mObject3DList.clear();
	mCacheDirty = true;
}

Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		mObject3DList.begin(),
		mObject3DList.end(),
		[&](Object3D* obj) {return obj->GetTag() == tag; } //繝ｩ繝繝蠑
		//[&]...莉雁屓縺ｮ蝣ｴ蜷医［Object3DList縺ｮ隕∫ｴ繧端蜿らｧ]縺吶ｋ縺ｨ縺縺諢丞袖蜷医＞
		//(Object3D *obj)...蜿らｧ縺励◆繧ｪ繝悶ず繧ｧ繧ｯ繝医ｮ蝙九→蠑墓焚蜷
		//{...}...蜃ｦ逅蜀螳ｹｼ井ｻ雁屓縺ｯ譚｡莉ｶ)
		//[](){...}縺薙ｮ蠖｢縺後Λ繝繝蠑上ｮ蝓ｺ譛ｬ
	);
	if (itr != mObject3DList.end())
	{
		return (*itr);//繧ｪ繝悶ず繧ｧ繧ｯ繝医′隕九▽縺九▲縺
	}
	return nullptr;  //繧ｪ繝悶ず繧ｧ繧ｯ繝医′隕九▽縺九ｉ縺ｪ縺九▲縺
}

//謖螳壹＠縺溘◆縺舌ｮ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｮ繝ｪ繧ｹ繝医ｒ蜿門ｾ
const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	if (mCacheDirty)
	{
		mCached3DLists.clear();
		for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
		{
			mCached3DLists[(*itr)->GetTag()].push_back(*itr);
		}
		mCacheDirty = false;
	}
	return mCached3DLists[tag];
}

void ObjectManager::DeleteAll3DIfNeeded()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
	{
		if ((*itr)->IsDeleteFlag() == true)
		{
			Object3D* temp = *itr;

			//繝ｪ繧ｹ繝医°繧牙炎髯､
			itr = mObject3DList.erase(itr);
			mCacheDirty = true;

			//繧ｪ繝悶ず繧ｧ繧ｯ繝医◎縺ｮ繧ゅｮ繧貞炎髯､
			delete temp;
			temp = nullptr;
		}
		else
		{
			//縺､縺弱ｮitr縺ｫ騾ｲ繧√ｋ
			itr++;
		}
	}

}

//ｼ奪繧ｪ繝悶ず繧ｧ繧ｯ繝医ｮ霑ｽ蜉
void ObjectManager::AddObject(Object2D* object2D)
{
	mObject2DList.push_back(object2D);
}

//2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｮ蜈ｨ蜑企勁
void ObjectManager::DeleteAll2D()
{
	Master::mpInfClassManager->LogList.clear();
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); /* 縺薙％縺ｯ遨ｺ縺ｪ縺ｮ縺ｧ豕ｨ諢 */)
	{
		Object2D* temp = *itr;

		//繝ｪ繧ｹ繝医°繧牙炎髯､
		itr = mObject2DList.erase(itr);

		//繧ｪ繝悶ず繧ｧ繧ｯ繝医◎縺ｮ繧ゅｮ繧貞炎髯､
		delete temp;
		temp = nullptr;
	}
}

//蜑企勁縺吶ｋ蠢隕√ｮ縺ゅｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医′縺ゅｌ縺ｰ蜑企勁縺吶ｋ
void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); /* 縺薙％縺ｯ遨ｺ縺ｪ縺ｮ縺ｧ豕ｨ諢 */)
	{
		if ((*itr)->IsDeleteFlag() == true)//逵∫払縺吶ｋ縺ｨ(*itr)->IsDeleteFlag()
		{
			Object2D* temp = *itr;

			//繝ｪ繧ｹ繝医°繧牙炎髯､
			itr = mObject2DList.erase(itr);

			//繧ｪ繝悶ず繧ｧ繧ｯ繝医◎縺ｮ繧ゅｮ繧貞炎髯､
			delete temp;
			temp = nullptr;
		}
		else
		{
			//谺｡縺ｮ隕∫ｴ縺ｸ騾ｲ繧√ｋ
			itr++;
		}

	}
}

//謖螳壹＠縺溘ち繧ｰ縺ｮｼ奪繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜿門ｾ
Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
	//std::find繧貞茜逕ｨ縺励※蟇ｾ雎｡縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ謗｢縺
	auto itr = std::find_if(
		mObject2DList.begin(),
		mObject2DList.end(),
		[&](Object2D* obj) {return obj->GetTag() == tag; } //繝ｩ繝繝蠑
		//[&]...莉雁屓縺ｮ蝣ｴ蜷医［Object2DList縺ｮ隕∫ｴ繧端蜿らｧ]縺吶ｋ縺ｨ縺縺諢丞袖蜷医＞
		//(Object2D *obj)...蜿らｧ縺励◆繧ｪ繝悶ず繧ｧ繧ｯ繝医ｮ蝙九→蠑墓焚蜷
		//{...}...蜃ｦ逅蜀螳ｹｼ井ｻ雁屓縺ｯ譚｡莉ｶ)
		//[](){...}縺薙ｮ蠖｢縺後Λ繝繝蠑上ｮ蝓ｺ譛ｬ
	);

	//隕九▽縺九▲縺溘°縺ｩ縺縺九ｒ蛻､螳
	if (itr != mObject2DList.end())
	{
		return (*itr);//繧ｪ繝悶ず繧ｧ繧ｯ繝医′隕九▽縺九▲縺
	}
	return nullptr;  //繧ｪ繝悶ず繧ｧ繧ｯ繝医′隕九▽縺九ｉ縺ｪ縺九▲縺
}

//謖螳壹＠縺溘◆縺舌ｮ2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｮ繝ｪ繧ｹ繝医ｒ蜿門ｾ
std::vector<Object2D*>ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	std::vector<Object2D*>ret;

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		//tag縺ｨ縺翫↑縺倥ち繧ｰ繧呈戟縺｣縺ｦ縺繧九が繝悶ず繧ｧ繧ｯ繝医′縺ゅｌ縺ｰvector縺ｫ蜈･繧後ｋ
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}

	return ret;
}