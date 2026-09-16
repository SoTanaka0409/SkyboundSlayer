import codecs
with open('Source/DrawHp.cpp', 'rb') as f:
    raw = f.read()

# Strip any BOMs or garbage at the very beginning
if raw.startswith(b'\xef\xbb\xbf'):
    raw = raw[3:]

text = raw.decode('utf-8', errors='ignore')
if text.startswith(''):
    text = text[1:]
if text.startswith('?'):
    text = text[1:]
if not text.startswith('#include'):
    text = '#include' + text.split('#include', 1)[1]

with codecs.open('Source/DrawHp.cpp', 'w', 'utf-8-sig') as f:
    f.write(text)
