import codecs

# ============================================================
# PauseMenu.cpp: 全面的レイアウト修正
# ============================================================
with open('Source/PauseMenu.cpp', 'rb') as f:
    raw = f.read()

# UTF-8 no-BOM -> decode as UTF-8
text = raw.decode('utf-8', errors='replace')

# ----------------------------------------------------------
# 1. トグルボタンとラベルのX座標が同じ(x=594)で重なっている問題を修正
#    Settings と同様にトグルをラベルの右に移動
#    BGM toggle: 594,276 -> 740,308   SE toggle: 594,396 -> 740,428
#    DEBUG toggle: 594,550 -> 740,552
#    バー: x=800,w=520 -> x=930,w=370（右端1300、%テキスト1315、パネル右1450でOK）
# ----------------------------------------------------------
text = text.replace(
    'bgm_toggle_rect_ = { 594, 276, 172, 48 };',
    'bgm_toggle_rect_ = { 740, 308, 150, 48 };'
)
text = text.replace(
    'bgm_bar_rect_ = { 800, 290, 520, 24 };',
    'bgm_bar_rect_ = { 930, 316, 370, 24 };'
)
text = text.replace(
    'se_toggle_rect_ = { 594, 396, 172, 48 };',
    'se_toggle_rect_ = { 740, 428, 150, 48 };'
)
text = text.replace(
    'se_bar_rect_ = { 800, 410, 520, 24 };',
    'se_bar_rect_ = { 930, 436, 370, 24 };'
)
text = text.replace(
    'debug_toggle_rect_ = { 594, 550, 172, 48 };',
    'debug_toggle_rect_ = { 740, 552, 150, 48 };'
)

# ----------------------------------------------------------
# 2. 文字化けした日本語説明文をASCII変数でダミー置換後、正しいUTF-8で書き直す
#    → UTF-8 BOM付きで保存し直す
# ----------------------------------------------------------
# 古い文字化けまたはUTF-8バイト列の説明文行を削除してASCIIに置き換え
import re
# 780のDrawFormatString行（説明文）を除去して正しいものに差し替え
text = re.sub(
    r'\tDrawFormatString\(780, 568, GetColor\(150, 164, 180\).*?\);',
    '\tDrawFormatString(900, 568, GetColor(150, 164, 180), "ON/OFF: \x83R\x83\x89\x83C\x83_\x81[\x95\\<\x8e\xa6\x81E\x83f\x83o\x83b\x83O\x91\x80\x8dF");',
    text,
    flags=re.DOTALL
)

# 実際にはCP932で正しく書かないといけないのでここでCP932でencodeする
# でも 'コライダー表示・デバッグ操作' をCP932で埋め込む

# 上のreplaceはASCIIバックスラッシュ表記を含むので一旦文字列として扱い
# 最終的にCP932でファイル出力する
# シンプルに: 正しい説明文をASCIIのみで
text = re.sub(
    r'\tDrawFormatString\(900, 568, GetColor\(150, 164, 180\).*?\);',
    '\tDrawFormatString(900, 568, GetColor(150, 164, 180), "DEBUG: ON/OFF");',
    text,
    flags=re.DOTALL
)

# ----------------------------------------------------------
# 3. Settingsの説明文もパネル内に収まるよう短縮 & 位置調整
# ----------------------------------------------------------
with open('Source/SettingsScene.cpp', 'rb') as f:
    rs = f.read()
ts = rs.decode('utf-8', errors='replace')

ts = re.sub(
    r'DrawFormatString\(880, 565, GetColor\(150, 164, 180\).*?\);',
    'DrawFormatString(900, 568, GetColor(150, 164, 180), "DEBUG: ON/OFF");',
    ts,
    flags=re.DOTALL
)

# CP932で保存
with open('Source/PauseMenu.cpp', 'wb') as f:
    f.write(text.encode('cp932', errors='replace'))

with open('Source/SettingsScene.cpp', 'wb') as f:
    f.write(ts.encode('cp932', errors='replace'))

print("Done (ASCII fallback for description)")
