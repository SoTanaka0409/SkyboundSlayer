import os, glob, re

all_paths = set()
for cpp in glob.glob('Source/*.cpp') + glob.glob('Source/*.h'):
    try:
        # Try utf-8-sig first, then cp932
        for enc in ['utf-8-sig', 'utf-8', 'cp932']:
            try:
                with open(cpp, encoding=enc) as f:
                    text = f.read()
                break
            except:
                continue
        for m in re.finditer(r'"(Resource/[^"]+)"', text):
            all_paths.add(m.group(1))
    except:
        pass

for p in sorted(all_paths):
    try:
        print(p)
    except:
        print(p.encode('ascii','replace').decode())
