# -*- coding: utf-8 -*-
import os

filepath = r'Source\Main.cpp'
with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
    content = f.read()

# Add include
content = content.replace('#include"ColliderManager.h"', '#include"ColliderManager.h"\n#include"EffectPool.h"')

# Add Update and Draw
content = content.replace('EffekseerManager::GetInstance()->Update();', 'EffekseerManager::GetInstance()->Update();\n\t\t\t\tEffectPool::GetInstance()->Update();')
content = content.replace('EffekseerManager::GetInstance()->Draw();', 'EffekseerManager::GetInstance()->Draw();\n\t\t\t\tEffectPool::GetInstance()->Draw();')

with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
    f.write(content)
print("Main.cpp updated.")
