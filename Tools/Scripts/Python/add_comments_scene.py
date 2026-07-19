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
    lines = content.splitlines(True)
    new_lines = []
    
    # Exclude constructor/destructor and operators if possible, but the regex already looks for return types, wait.
    # Actually, the previous regex was: r'^[ \t]*(?:virtual\s+)?(?:[\w\*\&\:]+)\s+\w+\s*\(.*?\)(?:\s*override)?\s*;'
    # This matches functions with return types. Constructors don't have return types.
    pattern = re.compile(r'^[ \t]*(?:virtual\s+)?(?:[\w\*\&\:]+)\s+\w+\s*\(.*?\)(?:\s*override)?\s*;')
    
    for i, line in enumerate(lines):
        if pattern.search(line):
            if i == 0 or "// [入力]" not in lines[i-1]:
                new_lines.append(h_comment)
        new_lines.append(line)
        
    content = "".join(new_lines)

    if content != original:
        with open(filepath, 'w', encoding='cp932', errors='replace') as f:
            f.write(content)
        print(f"Added inline comments to {filepath}")


if __name__ == "__main__":
    scene_cpps = [
        "Source/Scene.cpp",
        "Source/TitleScene.cpp",
        "Source/Scene3D.cpp",
        "Source/SceneGame.cpp",
        "Source/SceneManager.cpp",
        "Source/GameManager.cpp",
        "Source/Fade.cpp"
    ]
    scene_hs = [
        "Source/Scene.h",
        "Source/TitleScene.h",
        "Source/Scene3D.h",
        "Source/SceneGame.h",
        "Source/SceneManager.h",
        "Source/GameManager.h",
        "Source/Fade.h"
    ]
    
    for f in scene_cpps:
        if os.path.exists(f):
            add_comments_to_cpp(f)
            
    for f in scene_hs:
        if os.path.exists(f):
            add_comments_to_h(f)
