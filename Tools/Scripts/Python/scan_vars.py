import os
import glob
import re

def scan_headers():
    files = glob.glob("Source/*.h")
    bad_vars = set()
    
    # Matches mFoo, mnFoo, mbFoo, mpFoo, mfFoo
    pattern = re.compile(r'\b(m|mn|mb|mp|mf|is|has)([A-Z][A-Za-z0-9]*)\b')
    
    for f in files:
        with open(f, 'r', encoding='utf-8-sig', errors='ignore') as file:
            content = file.read()
            for line in content.splitlines():
                # naive check if it looks like a declaration
                if ';' in line and not line.strip().startswith('//'):
                    matches = pattern.findall(line)
                    for m in matches:
                        # skip if it's a known non-variable like IsHoverStart (wait, functions usually have parenthesis, but let's just collect all and filter visually)
                        if '(' not in line:
                            prefix, rest = m
                            bad_vars.add(prefix + rest)
                            
    for v in sorted(list(bad_vars)):
        print(v)

if __name__ == "__main__":
    scan_headers()
