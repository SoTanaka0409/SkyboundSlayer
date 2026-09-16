import glob, re, codecs
paths = set()
for f in glob.glob('Source/*.[ch]*'):
    try:
        content = codecs.open(f, 'r', 'cp932').read()
        for match in re.findall(r'\"(Resource/[^\"]+)\"', content):
            paths.add(match)
    except: pass
with codecs.open('paths.txt', 'w', 'utf-8') as f:
    for p in sorted(paths):
        f.write(p + '\n')
