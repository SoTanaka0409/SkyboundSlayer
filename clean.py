import os
import re

def clean_dead_code(directory):
    code_patterns = [
        r'^\s*//\s*.*[;{}]$',
        r'^\s*//\s*(if|for|while|switch|return|continue|break)\b.*',
        r'^\s*//\s*[a-zA-Z0-9_]+\s*\(.*\)\s*;?',
        r'^\s*//\s*[a-zA-Z0-9_]+\s*=\s*.*',
        r'^\s*//\s*auto\s+.*',
    ]
    regexes = [re.compile(p) for p in code_patterns]
    
    count = 0
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.cpp') or file.endswith('.h'):
                filepath = os.path.join(root, file)
                try:
                    with open(filepath, 'r', encoding='shift_jis') as f:
                        lines = f.readlines()
                except UnicodeDecodeError:
                    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                        lines = f.readlines()
                
                new_lines = []
                for line in lines:
                    is_dead_code = any(r.match(line) for r in regexes)
                    if not is_dead_code:
                        new_lines.append(line)
                    else:
                        count += 1
                
                with open(filepath, 'w', encoding='shift_jis', errors='ignore') as f:
                    f.writelines(new_lines)
    print(f'Removed {count} lines of dead code.')

clean_dead_code('c:/Users/student/Desktop/LevelMonster/Source')
