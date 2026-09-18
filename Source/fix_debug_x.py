import codecs

# SettingsScene.cpp
with codecs.open('Source/SettingsScene.cpp', 'r', 'utf-8-sig') as f:
    s = f.read()

# debug_toggle を右にずらす（DEBUG文字幅~110px + 余白 → x=820）
s = s.replace(
    ', debug_toggle_rect_{ 720, 552, 150, 48 }',
    ', debug_toggle_rect_{ 820, 552, 150, 48 }'
)
# 説明文もトグル右端(820+150=970)の右へ
s = s.replace(
    'DrawFormatString(900, 568, GetColor(150, 164, 180), "ON/OFF: debug display & controls");',
    'DrawFormatString(990, 568, GetColor(150, 164, 180), "ON/OFF: debug display & controls");'
)

with codecs.open('Source/SettingsScene.cpp', 'w', 'utf-8-sig') as f:
    f.write(s)
print('SettingsScene.cpp DONE')

# PauseMenu.cpp
with codecs.open('Source/PauseMenu.cpp', 'r', 'utf-8-sig') as f:
    p = f.read()

p = p.replace(
    'debug_toggle_rect_ = { 720, 552, 150, 48 };',
    'debug_toggle_rect_ = { 820, 552, 150, 48 };'
)
p = p.replace(
    'DrawFormatString(900, 568, GetColor(150, 164, 180), "ON/OFF: debug display & controls");',
    'DrawFormatString(990, 568, GetColor(150, 164, 180), "ON/OFF: debug display & controls");'
)

with codecs.open('Source/PauseMenu.cpp', 'w', 'utf-8-sig') as f:
    f.write(p)
print('PauseMenu.cpp DONE')
