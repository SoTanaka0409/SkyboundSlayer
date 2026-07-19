import os

def remove_from_file(filepath, strings_to_remove):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        
        new_lines = []
        for line in lines:
            if not any(s in line for s in strings_to_remove):
                new_lines.append(line)
                
        with open(filepath, 'w', encoding='utf-8') as f:
            f.writelines(new_lines)
        print("Updated " + filepath)
    except Exception as e:
        print("Failed to update " + filepath + ": " + str(e))

proj = 'DxLesson.vcxproj'
filt = 'DxLesson.vcxproj.filters'

remove_from_file(proj, ['Source\\Bush.cpp', 'Source\\Bush.h'])
remove_from_file(filt, ['Source\\Bush.cpp', 'Source\\Bush.h'])

