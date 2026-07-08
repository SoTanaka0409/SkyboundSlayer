# -*- coding: utf-8 -*-
with open(r'c:\Users\student\Desktop\programing\LevelMonster\DxLesson.vcxproj.filters', 'r', encoding='utf-8') as f:
    content = f.read()

no_filter = 0
wrong_filter = 0
for line in content.split('\n'):
    if '<ClInclude' in line:
        # Check next line
        pass
    if '<ClCompile' in line:
        pass

import xml.etree.ElementTree as ET
tree = ET.parse(r'c:\Users\student\Desktop\programing\LevelMonster\DxLesson.vcxproj.filters')
root = tree.getroot()
ns = {'msbuild': 'http://schemas.microsoft.com/developer/msbuild/2003'}

for itemgroup in root.findall('msbuild:ItemGroup', ns):
    for clcompile in itemgroup.findall('msbuild:ClCompile', ns):
        filter_node = clcompile.find('msbuild:Filter', ns)
        if filter_node is None:
            print(f"ClCompile without filter: {clcompile.get('Include')}")
        else:
            if 'ヘッダー' in filter_node.text:
                print(f"ClCompile with header filter: {clcompile.get('Include')} ({filter_node.text})")
    for clinclude in itemgroup.findall('msbuild:ClInclude', ns):
        filter_node = clinclude.find('msbuild:Filter', ns)
        if filter_node is None:
            print(f"ClInclude without filter: {clinclude.get('Include')}")
        else:
            if 'ソース' in filter_node.text:
                print(f"ClInclude with source filter: {clinclude.get('Include')} ({filter_node.text})")
