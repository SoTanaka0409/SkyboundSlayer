import codecs, re, glob, os

path_map = {
    # Swords / Weapons
    "Resource/model/剣/01_剣モデル.mv1": "Resource/model/props/sword/01_sword.mv1",
    "Resource/3Dモデル/小物/剣/01_剣モデル.mv1": "Resource/model/props/sword/01_sword.mv1",
    "Resource/model/小物/ポータル/01_ポータルモデル.mv1": "Resource/model/props/portal/01_portal.mv1",
    "Resource/3Dモデル/小物/ポータル/01_ポータルモデル.mv1": "Resource/model/props/portal/01_portal.mv1",
    
    # Missing effect names that might have been misspelled or missed
    "Resource/effect/slide_attack/02_スライド攻撃エフェクト使用.efk": "Resource/effect/slide_attack/02_slide_attack.efk",
    "Resource/effect/magic/02_魔法陣エフェクト使用.efk": "Resource/effect/magic/02_magic.efk",
    "Resource/effect_magic/02_魔法陣エフェクト用.efk": "Resource/effect_magic/02_magic_for.efk",
}

for root, dirs, files in os.walk('Resource'):
    # Rename portal and sword folders on disk
    if "小物" in root:
        new_root = root.replace("小物", "props")
        if not os.path.exists(new_root): os.makedirs(new_root)
    if "剣" in root:
        new_root = root.replace("剣", "sword").replace("小物", "props")
        if not os.path.exists(new_root): os.makedirs(new_root)
    if "ポータル" in root:
        new_root = root.replace("ポータル", "portal").replace("小物", "props")
        if not os.path.exists(new_root): os.makedirs(new_root)

# Explicit disk renames
def rename_on_disk(old_r, new_r):
    try:
        if os.path.exists(old_r):
            os.makedirs(os.path.dirname(new_r), exist_ok=True)
            os.rename(old_r, new_r)
    except: pass

rename_on_disk('Resource/model/小物/ポータル/01_ポータルモデル.mv1', 'Resource/model/props/portal/01_portal.mv1')
rename_on_disk('Resource/model/小物/剣/01_剣モデル.mv1', 'Resource/model/props/sword/01_sword.mv1')
rename_on_disk('Resource/model/剣/01_剣モデル.mv1', 'Resource/model/props/sword/01_sword.mv1')

rename_on_disk('Resource/effect/slide_attack/02_スライド攻撃エフェクト使用.efk', 'Resource/effect/slide_attack/02_slide_attack.efk')
rename_on_disk('Resource/effect/magic/02_魔法陣エフェクト使用.efk', 'Resource/effect/magic/02_magic.efk')
rename_on_disk('Resource/effect_magic/02_魔法陣エフェクト用.efk', 'Resource/effect_magic/02_magic_for.efk')

for cpp in glob.glob('Source/*.cpp') + glob.glob('Source/*.h'):
    # Detect encoding
    with open(cpp, 'rb') as f: raw = f.read()
    if raw.startswith(b'\xef\xbb\xbf'): enc = 'utf-8-sig'
    else: enc = 'cp932'
    
    try:
        with codecs.open(cpp, 'r', enc) as f: text = f.read()
        
        orig = text
        for jp, en in path_map.items():
            text = text.replace(jp, en)
            
        if text != orig:
            with codecs.open(cpp, 'w', enc) as f: f.write(text)
            print(f"Updated {cpp}")
    except Exception as e:
        pass
