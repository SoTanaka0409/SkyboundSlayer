# -*- coding: utf-8 -*-
import os

cpp_path = r'Source\Scene3D.cpp'
with open(cpp_path, 'r', encoding='shift_jis', errors='ignore') as f:
    cpp_content = f.read()

old_code = '''            std::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr, texture, colSizeStr;
            
            std::getline(ss, type, ',');
            std::getline(ss, model, ',');
            std::getline(ss, xStr, ',');
            std::getline(ss, yStr, ',');
            std::getline(ss, zStr, ',');
            std::getline(ss, sxStr, ',');
            std::getline(ss, syStr, ',');
            std::getline(ss, szStr, ',');
            std::getline(ss, texture, ',');
            std::getline(ss, colSizeStr, ',');
            
            float x = std::stof(xStr);
            float y = std::stof(yStr);
            float z = std::stof(zStr);
            float sx = std::stof(sxStr);
            float sy = std::stof(syStr);
            float sz = std::stof(szStr);
            VECTOR pos = VAdd(Config::GetStageCenter(), VGet(x, y, z));'''

new_code = '''            std::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr, texture, colSizeStr, isRelativeStr;
            
            std::getline(ss, type, ',');
            std::getline(ss, model, ',');
            std::getline(ss, xStr, ',');
            std::getline(ss, yStr, ',');
            std::getline(ss, zStr, ',');
            std::getline(ss, sxStr, ',');
            std::getline(ss, syStr, ',');
            std::getline(ss, szStr, ',');
            std::getline(ss, texture, ',');
            std::getline(ss, colSizeStr, ',');
            std::getline(ss, isRelativeStr, ',');
            
            float x = std::stof(xStr);
            float y = std::stof(yStr);
            float z = std::stof(zStr);
            float sx = std::stof(sxStr);
            float sy = std::stof(syStr);
            float sz = std::stof(szStr);
            int isRelative = 0;
            if (!isRelativeStr.empty()) isRelative = std::stoi(isRelativeStr);
            
            VECTOR pos = VGet(x, y, z);
            if (isRelative == 1) {
                pos = VAdd(Config::GetStageCenter(), pos);
            }'''

cpp_content = cpp_content.replace(old_code, new_code)

with open(cpp_path, 'w', encoding='shift_jis', errors='replace') as f:
    f.write(cpp_content)

print("Scene3D.cpp updated.")
