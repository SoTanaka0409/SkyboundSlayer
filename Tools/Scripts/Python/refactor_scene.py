import os
import glob
import re

replacements = {
    # TitleScene
    "mCameraAngle": "camera_angle_",
    "mnColorFade": "color_fade_",
    "mbColorFlag": "color_flag_",
    # Scene
    "mpObjectManager": "object_manager_",
    "mpColliderManager": "collider_manager_",
    # Scene3D
    "mpTexture": "texture_",
    "mpTexture2": "texture2_",
    "warningRadius": "warning_radius_",
    "maxRadius": "max_radius_",
    "LoadTimer": "load_timer_",
    "LoadCount": "load_count_",
    "Loadflag": "is_load_flag_",
    "pos": "pos_",
    "size": "size_",
    # Fade
    "mState": "state_",
    "mAlpha": "alpha_",
    "mFadeSpeed": "fade_speed_",
}

# we need to be careful with "pos" and "size" because they are too generic.
# They are only in Scene3D.cpp and Scene3D.h, so we only apply them there.

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

    filename = os.path.basename(filepath)
    is_scene3d = (filename in ["Scene3D.h", "Scene3D.cpp"])

    for old, new in replacements.items():
        if old in ["pos", "size"] and not is_scene3d:
            continue
        # Use word boundaries
        content = re.sub(rf'\b{old}\b', new, content)

    if content != original:
        with open(filepath, 'w', encoding='utf-8-sig') as f:
            f.write(content)
        print(f"Refactored {filepath}")

if __name__ == "__main__":
    files = glob.glob("Source/**/*.h", recursive=True) + glob.glob("Source/**/*.cpp", recursive=True)
    for f in files:
        refactor_file(f)
    print("Done refactoring Scene class variables.")
