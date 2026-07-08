# -*- coding: utf-8 -*-
import os

filepath = r'Source\Main.cpp'
try:
    with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
        content = f.read()
    
    old_str = "if (DxLib_Init() == -1)"
    new_str = "SetDoubleStartValidFlag(TRUE);\n\t" + old_str
    
    if old_str in content and "SetDoubleStartValidFlag" not in content:
        content = content.replace(old_str, new_str)
        with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
            f.write(content)
        print("Updated " + filepath)
    else:
        print("String not found or already updated in " + filepath)
except Exception as e:
    print("Failed to update " + filepath + ": " + str(e))
