import os, re
import glob

def check_files():
    print("Checking Scene3D files...")
    text = open('Source/Scene3D.cpp', encoding='utf-8', errors='ignore').read()
    paths = re.findall(r'"(Resource/[^"]+)"', text)
    missing = [p for p in paths if not os.path.exists(p)]
    if missing:
        print("Missing files in Scene3D.cpp:")
        for p in missing:
            print(p)
    else:
        print("All files exist.")

check_files()
