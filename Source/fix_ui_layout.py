import codecs

# --- SettingsScene.cpp ---
with codecs.open('Source/SettingsScene.cpp', 'r', 'utf-8-sig') as f:
    s = f.read()

# 1. %テキストをバーの右外(+14px)に出す。パネル右端1450, バー終端=920+420=1340, 1340+14=1354 → 枠内OK
OLD_PCT = 'DrawFormatString(barRect.x + barRect.w - 80, barRect.y - 2, enabled ? GetColor(245, 246, 248) : GetColor(130, 136, 146), "%3d%%", percent);'
NEW_PCT = 'DrawFormatString(barRect.x + barRect.w + 14, barRect.y - 2, enabled ? GetColor(245, 246, 248) : GetColor(130, 136, 146), "%3d%%", percent);'
s = s.replace(OLD_PCT, NEW_PCT)

# 2. DEBUGの説明文を日本語・小フォント・ボタン右端(720+150=870)から余白20px=890に
OLD_DESC = 'DrawFormatString(880, 565, GetColor(150, 164, 180), "Shows collider and debug controls in game");'
NEW_DESC = (
    'SetFontSize(18);\n'
    '\tDrawFormatString(890, 568, GetColor(150, 164, 180), "コライダー表示・デバッグ操作を有効にする");\n'
    '\tSetFontSize(24);'
)
s = s.replace(OLD_DESC, NEW_DESC)

with codecs.open('Source/SettingsScene.cpp', 'w', 'utf-8-sig') as f:
    f.write(s)
print('SettingsScene.cpp DONE')

# --- PauseMenu.cpp ---
with codecs.open('Source/PauseMenu.cpp', 'r', 'utf-8-sig') as f:
    p = f.read()

# 1. %テキストをバーの右外に。パネル1450, バー終端=800+520=1320, +14=1334 → 枠内OK
OLD_PCT2 = 'DrawFormatString(barRect.x + barRect.w - 80, barRect.y - 2, enabled ? GetColor(245, 246, 248) : GetColor(130, 136, 146), "%3d%%", percent);'
NEW_PCT2 = 'DrawFormatString(barRect.x + barRect.w + 14, barRect.y - 2, enabled ? GetColor(245, 246, 248) : GetColor(130, 136, 146), "%3d%%", percent);'
p = p.replace(OLD_PCT2, NEW_PCT2)

# 2. PauseのDEBUG行: debug_toggle x=594, w=172 → 右端766。説明は786から
# DrawSettingRowsでDebugラベルとトグルを描いた後、説明文を追加
# まず既存のDrawToggle行の後に説明文挿入
OLD_PAUSE_DEBUG = (
    '\tSetFontSize(34);\n'
    '\tDrawFormatString(594, 565, GetColor(222, 236, 248), "DEBUG");\n'
    '\tDrawToggle(debug_toggle_rect_, Master::debug_->GetdEbug(), "MODE");'
)
# Getdebug (note: casing)
OLD_PAUSE_DEBUG_ACTUAL = (
    '\tSetFontSize(34);\n'
    '\tDrawFormatString(594, 565, GetColor(222, 236, 248), "DEBUG");\n'
    '\tDrawToggle(debug_toggle_rect_, Master::debug_->Getdebug(), "MODE");'
)
NEW_PAUSE_DEBUG = (
    '\tSetFontSize(34);\n'
    '\tDrawFormatString(594, 565, GetColor(222, 236, 248), "DEBUG");\n'
    '\tDrawToggle(debug_toggle_rect_, Master::debug_->Getdebug(), "MODE");\n'
    '\n'
    '\tSetFontSize(18);\n'
    '\tDrawFormatString(780, 568, GetColor(150, 164, 180), "コライダー表示・デバッグ操作を有効にする");\n'
    '\tSetFontSize(24);'
)

if OLD_PAUSE_DEBUG_ACTUAL in p:
    p = p.replace(OLD_PAUSE_DEBUG_ACTUAL, NEW_PAUSE_DEBUG)
    print('PauseMenu DEBUG desc added')
else:
    print('WARNING: PauseMenu debug block not found, searching...')
    import re
    m = re.search(r'SetFontSize\(34\).*?DrawToggle\(debug_toggle_rect_.*?\);', p, re.DOTALL)
    if m:
        print('Found:', repr(m.group()))

with codecs.open('Source/PauseMenu.cpp', 'w', 'utf-8-sig') as f:
    f.write(p)
print('PauseMenu.cpp DONE')
