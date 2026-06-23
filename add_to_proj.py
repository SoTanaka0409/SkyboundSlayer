import sys

# Update vcxproj
with open('DxLesson.vcxproj', 'r', encoding='utf-8') as f:
    proj = f.read()

proj = proj.replace(
    '<ClCompile Include="Source\\Effect.cpp" />',
    '<ClCompile Include="Source\\Effect.cpp" />\n    <ClCompile Include="Source\\EffekseerManager.cpp" />'
)
proj = proj.replace(
    '<ClInclude Include="Source\\Effect.h" />',
    '<ClInclude Include="Source\\Effect.h" />\n    <ClInclude Include="Source\\EffekseerManager.h" />'
)

with open('DxLesson.vcxproj', 'w', encoding='utf-8') as f:
    f.write(proj)

# Update filters
with open('DxLesson.vcxproj.filters', 'r', encoding='utf-8') as f:
    filters = f.read()

filters = filters.replace(
    '<ClCompile Include="Source\\Effect.cpp">',
    '<ClCompile Include="Source\\Effect.cpp">\n      <Filter>ソース ファイル</Filter>\n    </ClCompile>\n    <ClCompile Include="Source\\EffekseerManager.cpp">'
)
filters = filters.replace(
    '<ClInclude Include="Source\\Effect.h">',
    '<ClInclude Include="Source\\Effect.h">\n      <Filter>ヘッダー ファイル</Filter>\n    </ClInclude>\n    <ClInclude Include="Source\\EffekseerManager.h">'
)

with open('DxLesson.vcxproj.filters', 'w', encoding='utf-8') as f:
    f.write(filters)
