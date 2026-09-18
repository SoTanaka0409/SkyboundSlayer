import os, glob

path_map = {
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
        "/01_通常ステージモデル.mv1": "/01_normal_stage.mv1",
        "/02_通常ステージ当たり判定モデル.mv1": "/02_normal_collider.mv1",
        "/01_岩モデル.x": "/01_rock.x",
        "/01_岩モデル.mv1": "/01_rock.mv1",
        "/01_木モデル.x": "/01_tree.x",
        "/01_木モデル.mv1": "/01_tree.mv1",
        "/02_テクスチャ.jpg": "/02_texture.jpg",
        "/01_空モデル.mv1": "/01_sky.mv1",
        "/01_浮遊島モデル.mv1": "/01_island.mv1",
        "/01_ジャンプ攻撃エフェクト.efk": "/01_jump_attack.efk",
        "/02_スライド攻撃エフェクト使用.efk": "/02_slide_attack.efk",
        "/02_魔法陣エフェクト使用.efk": "/02_magic.efk",
        "/02_魔法陣エフェクト用.efk": "/02_magic_for.efk",
        "/01_ステージ配置データ.csv": "/01_stage_layout.csv",
        "/00_敵用データ一覧.csv": "/00_enemy_data.csv",
        "/02_アイテムデータ.csv": "/02_item_data.csv",
        "/00_素材用途一覧.csv": "/00_material_usage.csv",
        "/01_会話ウィンドウ画像.png": "/01_dialog.png",
        "/02_セーブ画面画像.png": "/02_save.png",
        "/01_ボスエリア表示画像.png": "/01_boss_area.png",
        "/02_通常エリア表示画像.png": "/02_normal_area.png",
        "/01_最大HP強化アイコン.png": "/01_hp_icon.png",
        "/02_攻撃力強化アイコン.png": "/02_attack_icon.png",
        "/03_移動速度強化アイコン.png": "/03_speed_icon.png",
        "/04_回避距離強化アイコン.png": "/04_evade_dist_icon.png",
        "/05_回避無敵時間強化アイコン.png": "/05_evade_inv_icon.png",
        "/01_ルール画面背景１.png": "/01_rule_bg_1.png",
        "/02_ルール画面背景２.png": "/02_rule_bg_2.png",
        "/01_ダメージ表示画像.png": "/01_damage.png",
        "/01_タイトル画面BGM.mp3": "/01_title_bgm.mp3",
        "/02_ゲーム中BGM.mp3": "/02_game_bgm.mp3",
        "/03_リザルト画面BGM.mp3": "/03_result_bgm.mp3",
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

for root, dirs, files in os.walk('Resource'):
    for f in files:
        for jp, en in path_map.items():
            if jp[1:] == f: # Match file name (ignoring leading slash)
                print(f"Renaming {os.path.join(root, f)} -> {os.path.join(root, en[1:])}")
                os.rename(os.path.join(root, f), os.path.join(root, en[1:]))

print("Checking Stage layout CSV")
import codecs
with codecs.open('Resource/data/CSV/01_stage_layout.csv', 'r', 'cp932') as f:
    text = f.read()

# Make sure we didn't miss rock.mv1 and tree.mv1 inside the CSV
text = text.replace('Resource/model/props/rock/01_rock.mv1', 'Resource/model/props/rock/01_rock.mv1') # Already there?
# Let's see if there are any Japanese characters left in the CSV
import re
jp_chars = re.findall(r'[^\x00-\x7F]+', text)
if jp_chars:
    print("Found Japanese chars in CSV:", set(jp_chars))
else:
    print("No Japanese chars in CSV.")
