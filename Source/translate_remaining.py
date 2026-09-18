import os
import shutil

# We will traverse bottom-up so renaming a folder doesn't break paths to its children
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
}

def translate_name(name):
    new_name = name
    for jp, en in translations.items():
        new_name = new_name.replace(jp, en)
    return new_name

def rename_all_japanese():
    for root, dirs, files in os.walk('Resource', topdown=False):
        for f in files:
            new_f = translate_name(f)
            if new_f != f:
                src = os.path.join(root, f)
                dst = os.path.join(root, new_f)
                if not os.path.exists(dst):
                    os.rename(src, dst)
                else:
                    # if already exists, just remove the old one if they are identical? Or skip.
                    pass

        for d in dirs:
            new_d = translate_name(d)
            if new_d != d:
                src = os.path.join(root, d)
                dst = os.path.join(root, new_d)
                if not os.path.exists(dst):
                    os.rename(src, dst)
                else:
                    # Need to merge dirs if dest already exists
                    for sub_root, sub_dirs, sub_files in os.walk(src, topdown=False):
                        rel = os.path.relpath(sub_root, src)
                        dst_root = os.path.normpath(os.path.join(dst, rel))
                        os.makedirs(dst_root, exist_ok=True)
                        for sf in sub_files:
                            s_src = os.path.join(sub_root, sf)
                            s_dst = os.path.join(dst_root, sf)
                            if not os.path.exists(s_dst):
                                shutil.move(s_src, s_dst)
                    shutil.rmtree(src)

if __name__ == '__main__':
    rename_all_japanese()
