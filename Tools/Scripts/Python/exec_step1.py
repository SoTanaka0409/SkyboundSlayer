# -*- coding: utf-8 -*-
import os
import re

def replace_in_file(filepath, old_str, new_str, is_regex=False):
    try:
        with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
            content = f.read()
            
        if is_regex:
            new_content = re.sub(old_str, new_str, content)
            if content != new_content:
                with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
                    f.write(new_content)
                print("Updated " + filepath)
            else:
                print("No changes made in " + filepath)
        else:
            if old_str in content:
                content = content.replace(old_str, new_str)
                with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
                    f.write(content)
                print("Updated " + filepath)
            else:
                print("String not found in " + filepath)
    except Exception as e:
        print("Failed to update " + filepath + ": " + str(e))

# 1. ObjectManager.h
obj_mgr_h = r'Source\ObjectManager.h'
old_h1 = '''	std::vector<Object3D*>GetObject3DListByTag(Object3D::Tag3D tag);'''
new_h1 = '''	const std::vector<Object3D*>& GetObject3DListByTag(Object3D::Tag3D tag);'''

old_h2 = '''#include"Object2D.h"'''
new_h2 = '''#include"Object2D.h"\n#include<map>'''

old_h3 = '''	std::list<Object3D*>mObject3DList;   //3D'''
new_h3 = '''	std::map<Object3D::Tag3D, std::vector<Object3D*>> mCached3DLists;\n\tbool mCacheDirty;\n\tstd::list<Object3D*>mObject3DList;   //3D'''

replace_in_file(obj_mgr_h, old_h1, new_h1)
replace_in_file(obj_mgr_h, old_h2, new_h2)
replace_in_file(obj_mgr_h, old_h3, new_h3)

# 2. ObjectManager.cpp
obj_mgr_cpp = r'Source\ObjectManager.cpp'
old_cpp1 = '''ObjectManager::ObjectManager()
{

}'''
new_cpp1 = '''ObjectManager::ObjectManager()\n\t: mCacheDirty(true)\n{\n}'''

old_cpp2 = '''void ObjectManager::AddObject(Object3D* object3D)
{
	mObject3DList.push_back(object3D);
}'''
new_cpp2 = '''void ObjectManager::AddObject(Object3D* object3D)
{
	mObject3DList.push_back(object3D);
	mCacheDirty = true;
}'''

old_cpp3 = '''void ObjectManager::DeleteAll3D()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		delete (*itr);
	}
	mObject3DList.clear();
}'''
new_cpp3 = '''void ObjectManager::DeleteAll3D()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		delete (*itr);
	}
	mObject3DList.clear();
	mCacheDirty = true;
}'''

old_cpp4 = '''			itr = mObject3DList.erase(itr);
		}
		else'''
new_cpp4 = '''			itr = mObject3DList.erase(itr);
			mCacheDirty = true;
		}
		else'''

old_cpp5 = '''std::vector<Object3D*>ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	std::vector<Object3D*>ret;

	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		//tagとおなじタグを持ってぁEオブジェクトがあればvectorに入れる
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}

	return ret;
}'''
# Due to Shift-JIS comments inside old_cpp5, it's safer to replace using regex targeting the function signature.
replace_in_file(obj_mgr_cpp, old_cpp1, new_cpp1)
replace_in_file(obj_mgr_cpp, old_cpp2, new_cpp2)
replace_in_file(obj_mgr_cpp, old_cpp3, new_cpp3)
replace_in_file(obj_mgr_cpp, old_cpp4, new_cpp4)

def replace_func(filepath):
    try:
        with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
            content = f.read()
        
        # find the function std::vector<Object3D*>ObjectManager::GetObject3DListByTag... and replace it
        start_idx = content.find("std::vector<Object3D*>ObjectManager::GetObject3DListByTag")
        if start_idx != -1:
            end_idx = content.find("void ObjectManager::DeleteAll3DIfNeeded()", start_idx)
            if end_idx != -1:
                new_func = '''const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
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

'''
                content = content[:start_idx] + new_func + content[end_idx:]
                with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
                    f.write(content)
                print("Updated GetObject3DListByTag in " + filepath)
    except Exception as e:
        print("Failed regex replace: " + str(e))

replace_func(obj_mgr_cpp)

# 3. Callers - replace 'auto name = ...GetObject3DListByTag' with 'const auto& name = ...'
callers = [
    r'Source\Dino.cpp', r'Source\DrawHp.cpp', r'Source\Enemy.cpp', r'Source\GameManager.cpp',
    r'Source\Object3D.cpp', r'Source\Player3D.cpp'
]
for caller in callers:
    replace_in_file(caller, r'\bauto\s+(\w+)\s*=\s*Master::mpSceneManager->GetCurrentScene\(\)->GetObjectManager\(\)->GetObject3DListByTag', r'const auto& \1 = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag', is_regex=True)

# 4. Remove Bush includes
bush_callers = [r'Source\Player3D.cpp', r'Source\Scene3D.cpp', r'Source\Tutorial.cpp', r'Source\TutorialScene.cpp']
for caller in bush_callers:
    replace_in_file(caller, r'#include\s*["<]Bush\.h[">]\s*\n?', '', is_regex=True)

# 5. Remove Tag3D_Bush
replace_in_file(r'Source\Object3D.h', '\t\tTag3D_Bush=2301,\n', '')

