# -*- coding: utf-8 -*-
import os

proj = 'DxLesson.vcxproj'
filt = 'DxLesson.vcxproj.filters'

# Add to vcxproj
with open(proj, 'r', encoding='utf-8') as f:
    proj_content = f.read()

if 'EffectPool.cpp' not in proj_content:
    proj_content = proj_content.replace('<ClCompile Include="Source\\Effect.cpp" />', '<ClCompile Include="Source\\Effect.cpp" />\n    <ClCompile Include="Source\\EffectPool.cpp" />')
    proj_content = proj_content.replace('<ClInclude Include="Source\\Effect.h" />', '<ClInclude Include="Source\\Effect.h" />\n    <ClInclude Include="Source\\EffectPool.h" />')
    with open(proj, 'w', encoding='utf-8') as f:
        f.write(proj_content)

# Add to filters
with open(filt, 'r', encoding='utf-8') as f:
    filt_content = f.read()

if 'EffectPool.cpp' not in filt_content:
    filt_cpp = '''    <ClCompile Include="Source\\EffectPool.cpp">
      <Filter>ソース ファイル</Filter>
    </ClCompile>'''
    filt_h = '''    <ClInclude Include="Source\\EffectPool.h">
      <Filter>ヘッダー ファイル</Filter>
    </ClInclude>'''
    
    filt_content = filt_content.replace('<ClCompile Include="Source\\Effect.cpp">', filt_cpp + '\n    <ClCompile Include="Source\\Effect.cpp">')
    filt_content = filt_content.replace('<ClInclude Include="Source\\Effect.h">', filt_h + '\n    <ClInclude Include="Source\\Effect.h">')
    
    with open(filt, 'w', encoding='utf-8') as f:
        f.write(filt_content)

print("Project files updated.")
