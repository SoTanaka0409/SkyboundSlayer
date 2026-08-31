import sys

# Update vcxproj
with open('DxLesson.vcxproj', 'r', encoding='utf-8') as f:
    proj = f.read()

if 'Source\\Fade.cpp' not in proj:
    proj = proj.replace(
        '<ClCompile Include="Source\\SceneManager.cpp" />',
        '<ClCompile Include="Source\\SceneManager.cpp" />\n    <ClCompile Include="Source\\Fade.cpp" />'
    )
    proj = proj.replace(
        '<ClInclude Include="Source\\SceneManager.h" />',
        '<ClInclude Include="Source\\SceneManager.h" />\n    <ClInclude Include="Source\\Fade.h" />'
    )
    with open('DxLesson.vcxproj', 'w', encoding='utf-8') as f:
        f.write(proj)

# Update filters
with open('DxLesson.vcxproj.filters', 'r', encoding='utf-8') as f:
    filters = f.read()

if 'Source\\Fade.cpp' not in filters:
    filters = filters.replace(
        '<ClCompile Include="Source\\SceneManager.cpp">',
        '<ClCompile Include="Source\\SceneManager.cpp">\n      <Filter>ソース ファイル</Filter>\n    </ClCompile>\n    <ClCompile Include="Source\\Fade.cpp">'
    )
    filters = filters.replace(
        '<ClInclude Include="Source\\SceneManager.h">',
        '<ClInclude Include="Source\\SceneManager.h">\n      <Filter>ヘッダー ファイル</Filter>\n    </ClInclude>\n    <ClInclude Include="Source\\Fade.h">'
    )
    with open('DxLesson.vcxproj.filters', 'w', encoding='utf-8') as f:
        f.write(filters)
