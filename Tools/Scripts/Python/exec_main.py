# -*- coding: utf-8 -*-
import os

filepath = r'Source\Main.cpp'
try:
    with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
        lines = f.readlines()
        
    new_lines = []
    in_loop = False
    
    for i, line in enumerate(lines):
        if "Master::mpSoundManager->Initialize();" in line:
            new_lines.append("\tSetUseASyncLoadFlag(TRUE);\n")
            new_lines.append(line)
            continue
            
        if "Master::mpDrawHp->Update();" in line:
            new_lines.append("\t\tif (GetASyncLoadNum() > 0)\n")
            new_lines.append("\t\t{\n")
            new_lines.append("\t\t\tDrawFormatString(600, 360, GetColor(255, 255, 255), \"NOW LOADING... %d\", GetASyncLoadNum());\n")
            new_lines.append("\t\t}\n")
            new_lines.append("\t\telse\n")
            new_lines.append("\t\t{\n")
            new_lines.append("\t\t" + line)
            continue
            
        if "EffekseerManager::GetInstance()->Draw();" in line:
            new_lines.append("\t\t" + line)
            new_lines.append("\t\t}\n")
            continue
            
        if "Master::mpCamera->Update();" in line or \
           "Master::mpSceneManager->Update();" in line or \
           "Master::mpInfClassManager->Update();" in line or \
           "EffekseerManager::GetInstance()->Update();" in line or \
           "Master::mpSceneManager->Draw();" in line or \
           "Master::mpScoreManager->Draw();" in line:
            new_lines.append("\t\t" + line)
            continue
            
        new_lines.append(line)

    with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
        f.writelines(new_lines)
    print("Main.cpp updated successfully.")

except Exception as e:
    print(f"Error: {e}")
