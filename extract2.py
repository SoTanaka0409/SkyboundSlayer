import glob, re
paths = set()
for f in glob.glob('Source/*.[ch]*'):
    with open(f, 'rb') as fp:
        content = fp.read().decode('cp932', errors='ignore')
    for match in re.findall(r'\"(Resource/[^\"]+)\"', content):
        if any(ord(c) > 127 for c in match):
            paths.add(match)
for p in sorted(paths):
    print(p.encode('unicode_escape').decode('utf-8'))
