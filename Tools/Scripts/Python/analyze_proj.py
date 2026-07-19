import xml.etree.ElementTree as ET

tree = ET.parse(r'c:\Users\student\Desktop\programing\LevelMonster\DxLesson.vcxproj')
root = tree.getroot()
ns = {'msbuild': 'http://schemas.microsoft.com/developer/msbuild/2003'}

cpp_files = []
h_files = []

for itemgroup in root.findall('msbuild:ItemGroup', ns):
    for clcompile in itemgroup.findall('msbuild:ClCompile', ns):
        inc = clcompile.get('Include')
        if inc and inc.endswith('.cpp'):
            cpp_files.append(inc)
    for clinclude in itemgroup.findall('msbuild:ClInclude', ns):
        inc = clinclude.get('Include')
        if inc and inc.endswith('.h'):
            h_files.append(inc)

print(f"Found {len(cpp_files)} cpp files and {len(h_files)} h files")

with open('fix_filters.py', 'w', encoding='utf-8') as f:
    f.write('cpp_files = ' + str(cpp_files) + '\n')
    f.write('h_files = ' + str(h_files) + '\n')

