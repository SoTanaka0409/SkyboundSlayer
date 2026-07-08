# -*- coding: utf-8 -*-
import os

def replace_in_file(filepath, replacements):
    with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
        content = f.read()
    for old, new in replacements:
        content = content.replace(old, new)
    with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
        f.write(content)

# 1. TitleScene.h
with open(r'Source\TitleScene.h', 'r', encoding='shift_jis', errors='ignore') as f:
    title_h = f.read()
    
# Clean up garbage variables
lines = title_h.split('\n')
new_lines = []
for line in lines:
    if any(x in line for x in ['mnC', 'mnS', 'mnE', 'bool C;', 'bool S;', 'bool E;', 'bool T;', 'C_Enter', 'S_Enter', 'E_Enter', 'mnPause;', 'int size =']):
        continue
    if 'int Color1;' in line:
        new_lines.append('\tint mnColorFade;//カラー変更')
        continue
    if 'int ColorCount;' in line:
        new_lines.append('\tint mnColorCount;')
        continue
    if 'bool Colorflag;' in line:
        new_lines.append('\tbool mbColorFlag;')
        continue
    new_lines.append(line)
with open(r'Source\TitleScene.h', 'w', encoding='shift_jis', errors='replace') as f:
    f.write('\n'.join(new_lines))

# 2. TitleScene.cpp
replace_in_file(r'Source\TitleScene.cpp', [
    ('Color1', 'mnColorFade'),
    ('Colorflag', 'mbColorFlag'),
    (': mnPause(0), C(false), S(false), E(false), T(false)\n, C_Enter(false), S_Enter(false), E_Enter(false)\n, mnColorFade(1), mbColorFlag(false)', 
     ': mnColorFade(1), mbColorFlag(false)'),
])


# 3. ResultScene.h
with open(r'Source\ResultScene.h', 'r', encoding='shift_jis', errors='ignore') as f:
    res_h = f.read()
lines = res_h.split('\n')
new_lines = []
for line in lines:
    if any(x in line for x in ['mnC', 'mnS', 'mnE', 'bool C;', 'bool S;', 'bool E;', 'C_Enter', 'S_Enter', 'E_Enter', 'mnPause;', 'int size =', 'std::string filename1', 'mnHandle1']):
        continue
    if 'int AllTimer;' in line:
        new_lines.append('\tint mnAllTimer;')
        continue
    if 'int Color1;' in line:
        new_lines.append('\tint mnColorFade;//カラー変更')
        continue
    if 'int ColorCount;' in line:
        new_lines.append('\tint mnColorCount;')
        continue
    if 'bool Colorflag;' in line:
        new_lines.append('\tbool mbColorFlag;')
        continue
    new_lines.append(line)
with open(r'Source\ResultScene.h', 'w', encoding='shift_jis', errors='replace') as f:
    f.write('\n'.join(new_lines))


# 4. ResultScene.cpp
replace_in_file(r'Source\ResultScene.cpp', [
    ('AllTimer', 'mnAllTimer'),
    ('Color1', 'mnColorFade'),
    ('Colorflag', 'mbColorFlag'),
])

# 5. ResultWin.h
replace_in_file(r'Source\ResultWin.h', [
    ('AllTimer', 'mnAllTimer'),
    ('char Name;', 'char mNameStr;')
])

# 6. ResultWin.cpp
replace_in_file(r'Source\ResultWin.cpp', [
    ('AllTimer', 'mnAllTimer'),
    ('Name(0)', 'mNameStr(0)')
])

print("Cleanup complete.")
