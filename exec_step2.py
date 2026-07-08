# -*- coding: utf-8 -*-
import os

filepath = r'Source\ObjectManager.cpp'
with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
    lines = f.readlines()

new_lines = []
for i in range(len(lines)):
    line = lines[i]
    
    # 1. AddObject
    if "void ObjectManager::AddObject(Object3D* object3D)" in line:
        new_lines.append(line)
        continue
    if "mObject3DList.push_back(object3D);" in line and "ObjectManager::AddObject" in "".join(lines[max(0, i-5):i]):
        new_lines.append(line)
        new_lines.append("\tmCacheDirty = true;\n")
        continue

    # 2. DeleteAll3D
    if "void ObjectManager::DeleteAll3D()" in line:
        new_lines.append(line)
        continue
    if "mObject3DList.clear();" in line and "ObjectManager::DeleteAll3D()" in "".join(lines[max(0, i-10):i]):
        new_lines.append(line)
        new_lines.append("\tmCacheDirty = true;\n")
        continue

    # 3. DeleteAll3DIfNeeded
    if "itr = mObject3DList.erase(itr);" in line and "ObjectManager::DeleteAll3DIfNeeded()" in "".join(lines[max(0, i-15):i]):
        new_lines.append(line)
        new_lines.append("\t\t\tmCacheDirty = true;\n")
        continue
        
    new_lines.append(line)

with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
    f.writelines(new_lines)

print("Injected mCacheDirty into ObjectManager.cpp")
