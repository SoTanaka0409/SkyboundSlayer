import os, shutil

src_sword_dir = 'Resource/model/武器'
dst_sword_dir = 'Resource/model/props/sword'

if os.path.exists(src_sword_dir):
    os.makedirs(dst_sword_dir, exist_ok=True)
    src_sword_file = os.path.join(src_sword_dir, '01_剣モデル.mv1')
    dst_sword_file = os.path.join(dst_sword_dir, '01_sword.mv1')
    if os.path.exists(src_sword_file):
        os.rename(src_sword_file, dst_sword_file)
        print("Moved sword file")
    
    for item in os.listdir(src_sword_dir):
        os.rename(os.path.join(src_sword_dir, item), os.path.join(dst_sword_dir, item))
    os.rmdir(src_sword_dir)
    print("Removed old sword dir")

src_portal_dir = 'Resource/model/小物/ポータル'
dst_portal_dir = 'Resource/model/props/portal'

if os.path.exists(src_portal_dir):
    os.makedirs(dst_portal_dir, exist_ok=True)
    src_portal_file = os.path.join(src_portal_dir, '01_ポータルモデル.mv1')
    dst_portal_file = os.path.join(dst_portal_dir, '01_portal.mv1')
    
    if os.path.exists(src_portal_file):
        os.rename(src_portal_file, dst_portal_file)
        print("Moved portal file")
        
    for item in os.listdir(src_portal_dir):
        os.rename(os.path.join(src_portal_dir, item), os.path.join(dst_portal_dir, item))
    os.rmdir(src_portal_dir)
    print("Removed old portal dir")

src_eff = 'Resource/effect/slide_attack/02_スライド攻撃エフェクト再生用.efk'
dst_eff = 'Resource/effect/slide_attack/02_slide_attack.efk'
if os.path.exists(src_eff):
    os.rename(src_eff, dst_eff)
    print("Renamed slide attack efk")
