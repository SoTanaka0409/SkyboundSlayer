import os
import glob
import re

replacements = {
    "mbHitSizeflag": "is_hit_size_flag_",
    "mbNewEnemyflag": "is_new_enemy_flag_",
    "mfNormalSpeed": "normal_speed_",
    "mfMaxSpeed": "max_speed_",
    "WalkTimer": "walk_timer_",
    "WalkCount": "walk_count_",
    "hitPos": "hit_pos_",
    "GoPosition": "go_position_",
    "oldPosition": "old_position_",
    "VinitPos": "init_position_",
    "NoPosition": "no_position_",
    "moveVec": "move_vec_",
    "UpMoveVector": "up_move_vector_",
    "leftMoveVector": "left_move_vector_",
    "mbInvisible": "is_invisible_",
    "mnAlgorithm": "algorithm_id_",
    "AlgHit": "alg_hit_",
    "AttackInterval": "attack_interval_",
    "AttackCount": "attack_count_",
    "Animation": "is_animation_",
    "mfTargetAngle": "target_angle_",
    "mfAngle": "angle_",
    "mfSize": "size_",
    "HitSerch": "hit_search_",
    "HitAttackSerch": "hit_attack_search_",
    "HitStopSerch": "hit_stop_search_",
    "isDead": "is_dead_",
    "isHitSearch": "is_hit_search_flag_",
    "isHitAttackSearch": "is_hit_attack_search_flag_",
    "isHitSerchStop": "is_hit_search_stop_flag_",
    "isHitAttack": "is_hit_attack_flag_",
    "mnChance": "chance_",
    "mbItem": "has_item_",
    "AttackHitJudgmentflag": "is_attack_hit_judgment_flag_",
    "HitJudgmentflag_Player": "is_hit_judgment_flag_player_",
    "mfHaveMoney": "have_money_",
    # Functions
    "GetHitSizeFlag": "IsHitSizeFlag",
    "SetHitSizeFlag": "SetHitSizeFlag",
    "GetNewEnemyFlag": "IsNewEnemyFlag",
    "GetEnemycoll": "GetEnemyCollider",
    "DeathEnemy": "DeathEnemy",
    "GiveRewards": "GiveRewards",
    "CollPositionUpdate": "UpdateColliderPosition",
    "GetGoPlayer": "GetGoPosition",
    "SetGoPlayer": "SetGoPosition",
    "SetVPosition": "SetInitPosition",
    "GetVPosition": "GetInitPosition",
    "SetHitjudgment": "SetHitJudgmentFlagPlayer",
    "GetHitjudgment": "IsHitJudgmentFlagPlayer",
    "GetisDead": "IsDead",
    "GetIsHitSearch": "IsHitSearchFlag"
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
    print("Done refactoring Enemy class variables.")
