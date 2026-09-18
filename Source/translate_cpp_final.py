import os
import glob

# Same translations dict from translate_remaining.py
translations = {
    '未使用候補': 'unused',
    '素材一式': 'materials',
    '小物': 'props',
    'ポータル': 'portal',
    '焚き火': 'campfire',
    'コイン': 'coin',
    '魔法陣エフェクト': 'magic_effect',
    'スライド攻撃エフェクト': 'slide_attack_effect',
    'ジャンプ攻撃エフェクト': 'jump_attack_effect',
    '再生用': '_playback',
    '編集用': '_edit',
    'エフェクト': 'effect',
    '旧版': '_old',
    'モデル': '_model',
    'テクスチャ': '_texture',
    '当たり判定': '_collider',
    '通常': 'normal',
    'ショップ店員': 'shop_clerk',
    '敵モンスター': 'enemy_monster',
    'プレイヤー攻撃アニメーション': 'player_attack_anim',
    '大剣斬撃アニメーション': 'greatsword_slash_anim',
    'チャージアニメーション': 'charge_anim',
    '女性アクションポーズ': 'female_action_pose',
    '銃アクションアニメーション': 'gun_action_anim',
    '上昇ジャンプアニメーション': 'rising_jump_anim',
    'ジャンプアニメーション': 'jump_anim',
    '着地アニメーション': 'landing_anim',
    '左攻撃アニメーション': 'left_attack_anim',
    '走りスライドアニメーション': 'run_slide_anim',
    '斬撃アニメーション': 'slash_anim',
    'スライドアニメーション': 'slide_anim',
    'プレイヤー': 'player',
    '騎士元': 'knight_source',
    '騎士': 'knight',
    '画像': 'image',
    '森背景': 'forest_bg',
    'ショップ': 'shop',
    'インベントリ': 'inventory',
    'ロード画面': 'load_screen',
    '空ドーム': 'sky_dome',
    '空': 'sky',
    '木元': 'tree_source',
    '城壁元': 'castle_wall_source',
    'ポータル元': 'portal_source',
    '焚き火元': 'campfire_source',
    '城壁': 'castle_wall',
    'ステージ': 'stage',
    '木': 'tree',
    '岩': 'rock',
    '剣': 'sword',
    '武器': 'weapon',
    
    # Specific ones that might have slightly different names:
    '発生用': '_for', # In effect_magic/02_魔法陣エフェクト発生用.efk -> 02_magic_effect_for.efk?
}

# The actual file on disk is "Resource/effect_magic/02_magic_effect_for.efk" or similar?
# Let's check what '02_魔法陣エフェクト発生用.efk' became.
# '魔法陣エフェクト' -> 'magic_effect'
# '発生用' -> I didn't map '発生用' in translate_remaining.py!
# Wait, did translate_remaining.py leave it as '02_magic_effect発生用.efk' on disk?

# Let's do a more direct translation using regex for any Resource/ path!
import re

for cpp in glob.glob('Source/*.cpp') + glob.glob('Source/*.h'):
    with open(cpp, 'rb') as f: raw = f.read()
    
    # If it's UTF-8 BOM, we should decode as UTF-8
    enc = 'utf-8-sig' if raw.startswith(b'\xef\xbb\xbf') else 'utf-8' # try utf-8
    
    try:
        text = raw.decode(enc)
    except:
        text = raw.decode('cp932')
        enc = 'cp932'
    
    orig = text
    
    def repl(m):
        path = m.group(0)
        # Apply the translations
        for jp, en in translations.items():
            path = path.replace(jp, en)
        # Fix '発生用'
        path = path.replace('発生用', '_for')
        return path
        
    text = re.sub(r'Resource/[^\"]+', repl, text)
    
    if text != orig:
        # We need to write back in the ORIGINAL encoding so we don't mess up non-path Japanese comments
        if enc == 'utf-8-sig':
            with open(cpp, 'w', encoding='utf-8-sig') as f: f.write(text)
        elif enc == 'cp932':
            with open(cpp, 'w', encoding='cp932') as f: f.write(text)
        else:
            with open(cpp, 'w', encoding='utf-8') as f: f.write(text)
        print(f"Updated {cpp}")

