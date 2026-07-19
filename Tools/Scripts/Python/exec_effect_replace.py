# -*- coding: utf-8 -*-
import os

def replace_in_file(filepath):
    try:
        with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
            content = f.read()
            
        content = content.replace('new Effect(', 'EffectPool::GetInstance()->Play(')
        
        # Add include if necessary
        if 'EffectPool::GetInstance' in content and '#include "EffectPool.h"' not in content and '#include"EffectPool.h"' not in content:
            content = content.replace('#include"Effect.h"', '#include"Effect.h"\n#include "EffectPool.h"')
            
        with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
            f.write(content)
        print("Updated " + filepath)
    except Exception as e:
        print("Failed to update " + filepath + ": " + str(e))

replace_in_file(r'Source\Magic.cpp')
replace_in_file(r'Source\Player3D.cpp')

