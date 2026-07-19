import os
import glob
import re

replacements = {
    "mpPlayer": "player_",
    "mpSceneManager": "scene_manager_",
    "mpSoundManager": "sound_manager_",
    "mpScoreManager": "score_manager_",
    "mpCamera": "camera_",
    "mpResourceManager": "resource_manager_",
    "mpDebug": "debug_",
    "mpDrawHp": "draw_hp_",
    "mpEnemyManager": "enemy_manager_",
    "mpItemManager": "item_manager_",
    "mpInfClassManager": "inf_class_manager_",
    "mpBuffManager": "buff_manager_",
    "mpChat": "chat_",
    "mpSave": "save_",
    "PauseOn": "is_pause_on_",
    "StatShopClassOn": "is_stat_shop_on_",
    "NearShopOn": "is_near_shop_on_",
    "SafePointOn": "is_safe_point_on_",
    "mbSave": "is_save_",
    "CutscenePlaying": "is_cutscene_playing_",
    "GameClearCount": "game_clear_count_"
}

def refactor_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8-sig') as f:
            content = f.read()
            original = content
    except UnicodeDecodeError:
        try:
            with open(filepath, 'r', encoding='cp932') as f:
                content = f.read()
                original = content
        except Exception as e:
            print(f"Error reading {filepath}: {e}")
            return

    for old, new in replacements.items():
        content = re.sub(rf'\b{old}\b', new, content)

    if content != original:
        with open(filepath, 'w', encoding='utf-8-sig') as f:
            f.write(content)
        print(f"Refactored {filepath}")

if __name__ == "__main__":
    files = glob.glob("Source/**/*.h", recursive=True) + glob.glob("Source/**/*.cpp", recursive=True)
    for f in files:
        refactor_file(f)
    print("Done refactoring Master class variables.")
