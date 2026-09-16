import codecs

with codecs.open('Source/Master.h', 'r', 'utf-8-sig') as f:
    text = f.read()

text = text.replace('static bool is_pause_on_;', 'static int hit_stop_timer_;\n\tstatic bool is_pause_on_;')

with codecs.open('Source/Master.h', 'w', 'utf-8-sig') as f:
    f.write(text)

with codecs.open('Source/Main.cpp', 'r', 'utf-8-sig') as f:
    text = f.read()

text = text.replace('bool Master::is_pause_on_ = false;', 'int Master::hit_stop_timer_ = 0;\nbool Master::is_pause_on_ = false;')

with codecs.open('Source/Main.cpp', 'w', 'utf-8-sig') as f:
    f.write(text)
