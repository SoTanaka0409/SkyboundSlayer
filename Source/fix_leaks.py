import codecs
import re

# 1. Main.cpp leaks
with codecs.open('Source/Main.cpp', 'r', 'utf-8-sig') as f:
    main_cpp = f.read()

cleanup_add = '''
	delete Master::debug_;
	delete Master::draw_hp_;
	delete Master::enemy_manager_;
	delete Master::item_manager_;
	delete Master::inf_class_manager_;
	delete Master::buff_manager_;
	delete Master::chat_;
	delete Master::save_;
'''
main_cpp = main_cpp.replace('delete Master::resource_manager_;', 'delete Master::resource_manager_;\n' + cleanup_add)

with codecs.open('Source/Main.cpp', 'w', 'utf-8-sig') as f:
    f.write(main_cpp)

# 2. Player3D.cpp leaks
with codecs.open('Source/Player3D.cpp', 'r', 'utf-8-sig') as f:
    p3d_cpp = f.read()

# I need to use regex because the encoding could be CP932 or utf-8-sig, but I already ensured it was UTF-8 when running the previous script? Wait. Player3D.cpp is cp932!
