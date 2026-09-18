import os
import glob
import codecs

def rename_paths():
    # Mapping of Japanese to English for parts of the path
    # We will do a string replacement on paths found in code, then rename the files on disk accordingly.
    
    path_map = {
        # Top level
        "Resource/3Dモデル": "Resource/model",
        "Resource/エフェクト": "Resource/effect",
        "Resource/データ": "Resource/data",
        "Resource/画像": "Resource/image",
        "Resource/音源": "Resource/sound",
        "Resource/エフェクト_魔法陣": "Resource/effect_magic",
        
        # Sub levels - model
        "/キャラクターとアニメーション": "/character",
        "/ステージ/通常ステージ": "/stage/normal",
        "/小物/岩": "/props/rock",
        "/小物/木": "/props/tree",
        "/背景/空": "/bg/sky",
        "/背景/浮遊島": "/bg/island",
        
        # Sub levels - effect
        "/ジャンプ攻撃": "/jump_attack",
        "/スライド攻撃": "/slide_attack",
        "/魔法陣": "/magic",
        
        # Sub levels - data
        # CSV is already CSV
        
        # Sub levels - image
        "/UI": "/UI",
        "/エリア表示": "/area",
        "/ショップ": "/shop",
        "/ルール画面": "/rule",
        "/戦闘": "/battle",
        "/背景": "/bg",
        
        # Files - model/character
        "/01_人型キャラクターモデル.mv1": "/01_human.mv1",
        "/02_エスタークモデル.mv1": "/02_estark.mv1",
        "/03_ボスモデル.mv1": "/03_boss.mv1",
        "/04_ショップスモデル.mv1": "/04_shop.mv1",
        "/11_待機アニメーション.mv1": "/11_idle.mv1",
        "/12_走りアニメーション.mv1": "/12_run.mv1",
        "/13_死亡アニメーション.mv1": "/13_die.mv1",
        "/14_魔法攻撃アニメーション.mv1": "/14_magic_attack.mv1",
        "/15_攻撃アニメーション１.mv1": "/15_attack_1.mv1",
        "/16_通常攻撃アニメーション.mv1": "/16_normal_attack.mv1",
        "/17_ジャンプ攻撃アニメーション.mv1": "/17_jump_attack.mv1",
        "/18_回避アニメーション.mv1": "/18_evade.mv1",
        
        # Files - model/stage
        "/01_通常ステージモデル.mv1": "/01_normal_stage.mv1",
        "/02_通常ステージ当たり判定モデル.mv1": "/02_normal_collider.mv1",
        
        # Files - model/props
        "/01_岩モデル.x": "/01_rock.x",
        "/01_木モデル.x": "/01_tree.x",
        "/02_テクスチャ.jpg": "/02_texture.jpg",
        
        # Files - model/bg
        "/01_空モデル.mv1": "/01_sky.mv1",
        "/01_浮遊島モデル.mv1": "/01_island.mv1",
        
        # Files - effect
        "/01_ジャンプ攻撃エフェクト.efk": "/01_jump_attack.efk",
        "/02_スライド攻撃エフェクト使用.efk": "/02_slide_attack.efk",
        "/02_魔法陣エフェクト使用.efk": "/02_magic.efk",
        "/02_魔法陣エフェクト用.efk": "/02_magic_for.efk",
        
        # Files - data
        "/01_ステージ配置データ.csv": "/01_stage_layout.csv",
        
        # Files - image/UI
        "/01_会話ウィンドウ画像.png": "/01_dialog.png",
        "/02_セーブ画面画像.png": "/02_save.png",
        
        # Files - image/area
        "/01_ボスエリア表示画像.png": "/01_boss_area.png",
        "/02_通常エリア表示画像.png": "/02_normal_area.png",
        
        # Files - image/shop
        "/01_最大HP強化アイコン.png": "/01_hp_icon.png",
        "/02_攻撃力強化アイコン.png": "/02_attack_icon.png",
        "/03_移動速度強化アイコン.png": "/03_speed_icon.png",
        "/04_回避距離強化アイコン.png": "/04_evade_dist_icon.png",
        "/05_回避無敵時間強化アイコン.png": "/05_evade_inv_icon.png",
        
        # Files - image/rule
        "/01_ルール画面背景１.png": "/01_rule_bg_1.png",
        "/02_ルール画面背景２.png": "/02_rule_bg_2.png",
        
        # Files - image/battle
        "/01_ダメージ表示画像.png": "/01_damage.png",
        
        # Files - sound/BGM
        "/01_タイトル画面BGM.mp3": "/01_title_bgm.mp3",
        "/02_ゲーム中BGM.mp3": "/02_game_bgm.mp3",
        "/03_リザルト画面BGM.mp3": "/03_result_bgm.mp3",
        
        # Files - sound/SE
        "/01_火魔法効果音.mp3": "/01_fire_magic.mp3",
        "/02_通常攻撃効果音.mp3": "/02_normal_attack.mp3",
        "/03_スライド攻撃効果音.mp3": "/03_slide_attack.mp3",
        "/04_スライド移動効果音.mp3": "/04_slide_move.mp3",
        "/05_回復効果音.mp3": "/05_heal.mp3",
        "/06_ジャンプ効果音.mp3": "/06_jump.mp3",
        "/07_ワープ効果音.mp3": "/07_warp.mp3",
        "/08_能力上昇効果音.mp3": "/08_powerup.mp3",
        "/09_購入効果音.mp3": "/09_buy.mp3",
        "/10_決定効果音.mp3": "/10_decide.mp3",
        "/11_メニュー効果音.mp3": "/11_menu.mp3",
    }

    # Helper to translate a path
    def translate_path(p):
        for j, e in path_map.items():
            p = p.replace(j, e)
        return p

    # 1. Gather all files in Resource
    all_files = []
    for root, dirs, files in os.walk('Resource'):
        for f in files:
            all_files.append(os.path.join(root, f).replace('\\\\', '/').replace('\\', '/'))

    # 2. Rename files on disk
    # To avoid issues with parents being renamed before children, we sort by depth descending
    all_files.sort(key=lambda x: x.count('/'), reverse=True)
    
    renamed_dirs = set()
    for old_path in all_files:
        new_path = translate_path(old_path)
        if old_path != new_path:
            # Create directories if they don't exist
            new_dir = os.path.dirname(new_path)
            if not os.path.exists(new_dir):
                os.makedirs(new_dir)
            
            # Rename the file
            try:
                os.rename(old_path, new_path)
            except Exception as e:
                print(f"Failed to rename file {old_path} -> {new_path}: {e}")

    # 3. Rename any remaining empty directories (bottom-up)
    all_dirs = []
    for root, dirs, files in os.walk('Resource'):
        all_dirs.append(root.replace('\\\\', '/').replace('\\', '/'))
    all_dirs.sort(key=lambda x: x.count('/'), reverse=True)
    
    for old_dir in all_dirs:
        new_dir = translate_path(old_dir)
        if old_dir != new_dir:
            try:
                if os.path.exists(old_dir) and not os.listdir(old_dir):
                    os.rmdir(old_dir) # Remove empty old dir
            except:
                pass

    # 4. Update source code files
    cpp_files = glob.glob('Source/*.cpp') + glob.glob('Source/*.h')
    for cpp in cpp_files:
        try:
            with codecs.open(cpp, 'r', 'utf-8-sig') as f:
                text = f.read()
            
            new_text = translate_path(text)
            
            if text != new_text:
                with codecs.open(cpp, 'w', 'utf-8-sig') as f:
                    f.write(new_text)
                print(f"Updated {cpp}")
        except Exception as e:
            print(f"Error processing {cpp}: {e}")

    print("Renaming completed successfully.")

if __name__ == "__main__":
    rename_paths()
