import os, shutil

# Source directories in Japanese
src_sword_dir = b'Resource/model/\x95\x90\x8a\xed' # 武器
src_sword_file = src_sword_dir + b'/01_\x8c\x95\x83\x82\x83f\x83\x8b.mv1' # 01_剣モデル.mv1

dst_sword_dir = b'Resource/model/props/sword'
dst_sword_file = b'Resource/model/props/sword/01_sword.mv1'

if os.path.exists(src_sword_dir):
    os.makedirs(dst_sword_dir, exist_ok=True)
    if os.path.exists(src_sword_file):
        os.rename(src_sword_file, dst_sword_file)
        print("Moved sword file")
    
    # move anything else
    for item in os.listdir(src_sword_dir):
        os.rename(os.path.join(src_sword_dir, item), os.path.join(dst_sword_dir, item))
    
    os.rmdir(src_sword_dir)
    print("Removed old sword dir")

# What about portal?
src_portal_dir = b'Resource/model/\x8f\xac\x95\xa8/\x83|\x81[\x83^\x83\x8b' # 小物/ポータル
src_portal_file = src_portal_dir + b'/01_\x83|\x81[\x83^\x83\x8b\x83\x82\x83f\x83\x8b.mv1'

dst_portal_dir = b'Resource/model/props/portal'
dst_portal_file = b'Resource/model/props/portal/01_portal.mv1'

if os.path.exists(src_portal_dir):
    os.makedirs(dst_portal_dir, exist_ok=True)
    if os.path.exists(src_portal_file):
        os.rename(src_portal_file, dst_portal_file)
        print("Moved portal file")
    
    for item in os.listdir(src_portal_dir):
        os.rename(os.path.join(src_portal_dir, item), os.path.join(dst_portal_dir, item))
        
    os.rmdir(src_portal_dir)
    print("Removed old portal dir")

# What about the slide attack effect?
src_eff = b'Resource/effect/slide_attack/02_\x83X\x83\x89\x83C\x83h\x8dU\x8c\x82\x83G\x83t\x83F\x83N\x83g\x8d\xc4\x90\xb6\x97p.efk'
dst_eff = b'Resource/effect/slide_attack/02_slide_attack.efk'
if os.path.exists(src_eff):
    os.rename(src_eff, dst_eff)
    print("Renamed slide attack efk")
