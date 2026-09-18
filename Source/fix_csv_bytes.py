import os, glob

path_map = {
    b"Resource/3D\x83\x82\x83f\x83\x8b/\x8f\xac\x95\xa8/\x96\xd8/01_\x96\xd8\x83\x82\x83f\x83\x8b.mv1": b"Resource/model/props/tree/01_tree.mv1",
    b"Resource/3D\x83\x82\x83f\x83\x8b/\x8f\xac\x95\xa8/\x8a\xe2/01_\x8a\xe2\x83\x82\x83f\x83\x8b.mv1": b"Resource/model/props/rock/01_rock.mv1",
    b"Resource/3D\x83\x82\x83f\x83\x8b/\x8f\xac\x95\xa8/\x8a\xe2/02_\x8a\xe2\x83e\x83N\x83X\x83`\x83\x83.jpg": b"Resource/model/props/rock/02_texture.jpg",
    b"Resource/3D\x83\x82\x83f\x83\x8b/\x83X\x83e\x81[\x83W/\x92\xca\x8f\xe7\x83X\x83e\x81[\x83W/01_\x92\xca\x8f\xe7\x83X\x83e\x81[\x83W\x83\x82\x83f\x83\x8b.mv1": b"Resource/model/stage/normal/01_normal_stage.mv1",
    b"Resource/3D\x83\x82\x83f\x83\x8b/\x83X\x83e\x81[\x83W/\x92\xca\x8f\xe7/01_\x93\x96\x82\xbd\x82\xe8\x94\xbb\x92\xe8\x83\x82\x83f\x83\x8b.mv1": b"Resource/model/stage/normal/02_normal_collider.mv1",
}

for csv_file in glob.glob('Resource/データ/CSV/*.csv'):
    with open(csv_file, 'rb') as f:
        data = f.read()
    
    orig = data
    for jp, en in path_map.items():
        data = data.replace(jp, en)
        
    if data != orig:
        with open(csv_file, 'wb') as f:
            f.write(data)
        print(f"Updated {csv_file}")
    
    # Rename the file if needed
    new_csv = csv_file
    if b'\x93G\x97p\x83f\x81[\x83^\x88\xea\x97\x97' in csv_file.encode('cp932'):
        new_csv = csv_file.replace('00_敵用データ一覧.csv', '00_enemy_data.csv')
    if b'\x83A\x83C\x83e\x83\x80\x83f\x81[\x83^' in csv_file.encode('cp932'):
        new_csv = csv_file.replace('02_アイテムデータ.csv', '02_item_data.csv')
    if b'\x83X\x83e\x81[\x83W\x94z\x92u\x83f\x81[\x83^' in csv_file.encode('cp932'):
        new_csv = csv_file.replace('01_ステージ配置データ.csv', '01_stage_layout.csv')
    if b'\x91f\x8d\xde\x97p\x93r\x88\xea\x97\x97' in csv_file.encode('cp932'):
        new_csv = csv_file.replace('00_素材用途一覧.csv', '00_material_usage.csv')
    
    if new_csv != csv_file:
        os.rename(csv_file, new_csv)
        print(f"Renamed {csv_file} to {new_csv}")

