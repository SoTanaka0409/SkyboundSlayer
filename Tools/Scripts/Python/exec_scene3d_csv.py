# -*- coding: utf-8 -*-
import os
import re

cpp_path = r'Source\Scene3D.cpp'
with open(cpp_path, 'r', encoding='shift_jis', errors='ignore') as f:
    cpp_content = f.read()

# I will replace the block from "float ObjectSize=10.0f;" until the "SkyBox* pSkyBox = new SkyBox"
start_str = 'float ObjectSize=10.0f;'
end_str = 'SkyBox* pSkyBox = new SkyBox'

start_idx = cpp_content.find(start_str)
end_idx = cpp_content.find(end_str)

if start_idx != -1 and end_idx != -1:
    new_block = '''
    // データ駆動設計: CSVからステージオブジェクトを読み込んで配置
    std::ifstream file("Resource/CSV/stage_objects.csv");
    if (file.is_open())
    {
        std::string line;
        std::getline(file, line); // ヘッダースキップ
        while (std::getline(file, line))
        {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr, texture, colSizeStr;
            
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
            VECTOR pos = VAdd(Config::GetStageCenter(), VGet(x, y, z));
            VECTOR scale = VGet(sx, sy, sz);
            
            if (type == "StageObject")
            {
                float colSize = 0.0f;
                if (!colSizeStr.empty()) colSize = std::stof(colSizeStr);
                new StageObject(pos, model, scale, "", colSize);
            }
            else if (type == "Stage")
            {
                new Stage(pos, model, model, scale, texture);
            }
        }
    }
    
    '''
    cpp_content = cpp_content[:start_idx] + new_block + cpp_content[end_idx:]
    
    # Add includes at the top
    if '#include <fstream>' not in cpp_content:
        cpp_content = '#include <fstream>\n#include <sstream>\n' + cpp_content
        
    with open(cpp_path, 'w', encoding='shift_jis', errors='replace') as f:
        f.write(cpp_content)
    print("Scene3D.cpp updated for CSV loading.")
else:
    print("Failed to find replacement boundaries.")
