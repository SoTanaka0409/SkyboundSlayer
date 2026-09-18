import codecs

# ============================================================
# 安全な修正スクリプト - 最小限の変更のみ行う
# ============================================================

# --- SettingsScene.cpp ---
with codecs.open('Source/SettingsScene.cpp', 'r', 'utf-8-sig') as f:
    s = f.read()

# 変更1: %テキストをバーの右外に（バー終端=920+420=1340, +14=1354, パネル右1450→OK）
s = s.replace(
    'DrawFormatString(barRect.x + barRect.w + 30, barRect.y - 2',
    'DrawFormatString(barRect.x + barRect.w + 14, barRect.y - 2'
)

# 変更2: DEBUG行の説明文をボタン右端(720+150=870)から少し右に、短い英語で（日本語は文字化けするので）
s = s.replace(
    'DrawFormatString(920, 564, GetColor(150, 164, 180), "Shows collider and debug controls in game");',
    'DrawFormatString(900, 568, GetColor(150, 164, 180), "ON/OFF: debug display & controls");'
)

# 変更3: DEBUGラベルのy位置をトグル(y=552)に揃える
s = s.replace(
    'DrawFormatString(594, 560, GetColor(222, 236, 248), "DEBUG");',
    'DrawFormatString(594, 564, GetColor(222, 236, 248), "DEBUG");'
)

with codecs.open('Source/SettingsScene.cpp', 'w', 'utf-8-sig') as f:
    f.write(s)
print('SettingsScene.cpp DONE')

# --- PauseMenu.cpp ---
with codecs.open('Source/PauseMenu.cpp', 'r', 'utf-8-sig') as f:
    p = f.read()

# PauseMenuの主な問題: BGM/SEのラベル(x=594)とトグル(x=594)が同じX座標で被っている
# Settings はラベルx=594, トグルx=720 で右にずれている
# PauseはSettingsと同じパネル幅(520-1450)なのでSettingsと同じ座標を使う

# ToggleをSettings相当の位置に移動
p = p.replace(
    'bgm_toggle_rect_ = { 594, 276, 172, 48 };',
    'bgm_toggle_rect_ = { 720, 312, 150, 48 };'
)
p = p.replace(
    'bgm_bar_rect_ = { 800, 290, 520, 24 };',
    'bgm_bar_rect_ = { 920, 320, 380, 24 };'
)
p = p.replace(
    'se_toggle_rect_ = { 594, 396, 172, 48 };',
    'se_toggle_rect_ = { 720, 432, 150, 48 };'
)
p = p.replace(
    'se_bar_rect_ = { 800, 410, 520, 24 };',
    'se_bar_rect_ = { 920, 440, 380, 24 };'
)
p = p.replace(
    'debug_toggle_rect_ = { 720, 550, 172, 48 };',
    'debug_toggle_rect_ = { 720, 552, 150, 48 };'
)

# %テキストをバー右外へ (+30→+14)
p = p.replace(
    'DrawFormatString(barRect.x + barRect.w + 30, barRect.y - 2',
    'DrawFormatString(barRect.x + barRect.w + 14, barRect.y - 2'
)

# DEBUG説明文を追加（DrawToggle行の後）
p = p.replace(
    '\tDrawToggle(debug_toggle_rect_, Master::debug_->Getdebug(), "MODE");\n}',
    '\tDrawToggle(debug_toggle_rect_, Master::debug_->Getdebug(), "MODE");\n\n\tSetFontSize(18);\n\tDrawFormatString(900, 568, GetColor(150, 164, 180), "ON/OFF: debug display & controls");\n\tSetFontSize(24);\n}'
)

with codecs.open('Source/PauseMenu.cpp', 'w', 'utf-8-sig') as f:
    f.write(p)
print('PauseMenu.cpp DONE')
