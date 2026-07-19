import os
import glob
import re

cpp_comment = """/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
"""

h_comment = "// [入力]  [出力]  [副作用] \n"

def add_comments_to_cpp(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8-sig') as f:
            content = f.read()
    except:
        try:
            with open(filepath, 'r', encoding='cp932') as f:
                content = f.read()
        except Exception as e:
            print(f"Error reading {filepath}: {e}")
            return

    original = content
    # Regex to match function definitions in .cpp files (e.g. Type ClassName::FuncName(args) { )
    # Negative lookbehind to avoid adding if there's already a comment
    pattern = r'(?<!\*/\n)(?<!\*/\r\n)(?<!\*/\n\n)(?<!\*/\r\n\r\n)(^[ \t]*[\w\*\&\:]+\s+\w+::\w+\s*\(.*?\)\s*\n?{)'
    
    content = re.sub(pattern, cpp_comment + r'\1', content, flags=re.MULTILINE)
    
    if content != original:
        with open(filepath, 'w', encoding='cp932', errors='replace') as f:
            f.write(content)
        print(f"Added block comments to {filepath}")

def add_comments_to_h(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8-sig') as f:
            content = f.read()
    except:
        try:
            with open(filepath, 'r', encoding='cp932') as f:
                content = f.read()
        except Exception as e:
            print(f"Error reading {filepath}: {e}")
            return

    original = content
    # Process line by line to avoid look-behind issues
    lines = content.splitlines(True)
    new_lines = []
    
    # Match function declarations
    pattern = re.compile(r'^[ \t]*(?:virtual\s+)?(?:[\w\*\&\:]+)\s+\w+\s*\(.*?\)(?:\s*override)?\s*;')
    
    for i, line in enumerate(lines):
        if pattern.search(line):
            # Check if previous line exists and has the comment
            if i == 0 or "// [入力]" not in lines[i-1]:
                new_lines.append(h_comment)
        new_lines.append(line)
        
    content = "".join(new_lines)

    if content != original:
        with open(filepath, 'w', encoding='cp932', errors='replace') as f:
            f.write(content)
        print(f"Added inline comments to {filepath}")


if __name__ == "__main__":
    enemy_cpps = [
        "Source/Enemy.cpp",
        "Source/Enemy3D.cpp",
        "Source/Enemy3D_AT.cpp",
        "Source/EnemyBoss_1.cpp",
        "Source/EnemyManager.cpp",
        "Source/EnemyMonster.cpp"
    ]
    enemy_hs = [
        "Source/Enemy.h",
        "Source/Enemy3D.h",
        "Source/Enemy3D_AT.h",
        "Source/EnemyBoss_1.h",
        "Source/EnemyManager.h",
        "Source/EnemyMonster.h"
    ]
    
    for f in enemy_cpps:
        if os.path.exists(f):
            add_comments_to_cpp(f)
            
    for f in enemy_hs:
        if os.path.exists(f):
            add_comments_to_h(f)
