import os
import glob
import re

replacements = {
    # ObjectManager
    "mCached3DLists": "cached_3d_lists_",
    "mCacheDirty": "cache_dirty_",
    "mObject3DList": "object_3d_list_",
    "mObject2DList": "object_2d_list_",
    # Stage
    "mnCollisionHandle": "collision_handle_",
    # StageBox, Wall
    "mVertex": "vertex_",
    "mnGraphHandle": "graph_handle_",
    # StageObject
    "mfScale": "scale_",
    "mfHitRadius": "hit_radius_",
    "mbHasFollowedTerrain": "has_followed_terrain_",
    "mYOffset": "y_offset_",
    # Tree, Rock
    "mpCapsuleCollider": "capsule_collider_",
    "mnHitFlag": "is_hit_flag_",
    "mfHigh": "high_",
    # ColliderManager
    "mColliderList": "collider_list_",
}

# specific careful replacements
# mnModelHandle -> model_handle_ (used in Stage, Tree)
# Instance -> instance_ (in ColliderManager)

def refactor_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8-sig') as f:
            content = f.read()
            original = content
    except:
        try:
            with open(filepath, 'r', encoding='cp932') as f:
                content = f.read()
                original = content
        except Exception as e:
            print(f"Error reading {filepath}: {e}")
            return

    for old, new in replacements.items():
        content = re.sub(rf'\b{old}\b', new, content)

    # carefully replace mnModelHandle
    content = re.sub(r'\bmnModelHandle\b', 'model_handle_', content)

    # ColliderManager Instance
    # In ColliderManager.h / .cpp, "Instance" -> "instance_"
    if "ColliderManager" in filepath:
        content = re.sub(r'\bInstance\b', 'instance_', content)

    if content != original:
        with open(filepath, 'w', encoding='utf-8-sig') as f:
            f.write(content)
        print(f"Refactored {filepath}")

if __name__ == "__main__":
    files = glob.glob("Source/**/*.h", recursive=True) + glob.glob("Source/**/*.cpp", recursive=True)
    for f in files:
        refactor_file(f)
    print("Done refactoring Object class variables.")
