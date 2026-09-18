import os, glob

# Rename the folder on disk
old_dir = "Resource/model/ステージ/城壁"
new_dir = "Resource/model/stage/castle_wall"
if os.path.exists(old_dir.encode('cp932')):
    os.rename(old_dir.encode('cp932'), new_dir.encode('cp932'))
    print("Renamed folder")

old_file = os.path.join(new_dir, "01_城壁モデル.mv1").encode('cp932')
new_file = os.path.join(new_dir, "01_castle_wall.mv1").encode('cp932')
if os.path.exists(old_file):
    os.rename(old_file, new_file)
    print("Renamed mv1")

# Also rename remaining ステージ to stage just in case
old_stage = "Resource/model/ステージ"
new_stage = "Resource/model/stage"
if os.path.exists(old_stage.encode('cp932')):
    os.rename(old_stage.encode('cp932'), new_stage.encode('cp932'))

# Update CSV
with open('Resource/データ/CSV/01_stage_layout.csv'.encode('cp932'), 'rb') as f:
    data = f.read()

jp_path = b'Resource/3D\x83\x82\x83f\x83\x8b/\x83X\x83e\x81[\x83W/\x8f\xe9\x95\xc7/01_\x8f\xe9\x95\xc7\x83\x82\x83f\x83\x8b.mv1'
en_path = b'Resource/model/stage/castle_wall/01_castle_wall.mv1'
if jp_path in data:
    data = data.replace(jp_path, en_path)
    with open('Resource/データ/CSV/01_stage_layout.csv'.encode('cp932'), 'wb') as f:
        f.write(data)
    print("Updated CSV")

# Update any cpp files just in case it's in cpp
import glob
for cpp in glob.glob('Source/*.cpp') + glob.glob('Source/*.h'):
    with open(cpp, 'rb') as f:
        cpp_data = f.read()
    if jp_path in cpp_data:
        cpp_data = cpp_data.replace(jp_path, en_path)
        with open(cpp, 'wb') as f:
            f.write(cpp_data)
        print(f"Updated {cpp}")

