import os
import re

source_dir = r'c:\Users\Admin\Desktop\progtram\LevelMonster\Source'

regex_cast = re.compile(r'(?:auto|[\w:]+\s*\*)\s+(\w+)\s*=\s*dynamic_cast<')

def analyze_and_patch(filepath):
    try:
        with open(filepath, 'r', encoding='shift_jis') as f:
            lines = f.readlines()
    except Exception as e:
        print(f"Error reading {filepath}: {e}")
        return

    new_lines = []
    loop_levels = set()
    brace_level = 0
    
    i = 0
    modified = False
    
    while i < len(lines):
        line = lines[i]
        
        # very basic brace matching
        if '{' in line:
            brace_level += line.count('{')
        if '}' in line:
            brace_level -= line.count('}')
            # remove loops deeper than current
            loop_levels = {l for l in loop_levels if l <= brace_level}
            
        if 'for' in line or 'while' in line:
            if '{' in line:
                loop_levels.add(brace_level)
            else:
                # might be next line
                loop_levels.add(brace_level + 1)
                
        new_lines.append(line)
        
        match = regex_cast.search(line)
        if match and not line.strip().startswith('//'):
            var_name = match.group(1)
            
            # Check if next few lines have a null check
            has_check = False
            for j in range(1, 4):
                if i + j < len(lines):
                    next_line = lines[i+j]
                    if 'nullptr' in next_line or 'NULL' in next_line or f'!{var_name}' in next_line or f'if ({var_name})' in next_line or f'if({var_name})' in next_line:
                        has_check = True
                        break
                        
            if not has_check:
                # determine if we are in a loop
                indent = line[:len(line) - len(line.lstrip())]
                action = 'continue;' if loop_levels else 'return;'
                
                check_line = f'{indent}if ({var_name} == nullptr) {action}\n'
                new_lines.append(check_line)
                modified = True
                print(f"File: {os.path.basename(filepath)} | Line: {i+1}")
                print(f"  {line.strip()}")
                print(f"  + {check_line.strip()}")
                
        i += 1
        
    if modified:
        with open(filepath, 'w', encoding='shift_jis') as f:
            f.writelines(new_lines)

for filename in os.listdir(source_dir):
    if filename.endswith('.cpp'):
        analyze_and_patch(os.path.join(source_dir, filename))