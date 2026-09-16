import codecs
import re

with codecs.open('Source/Magic_Ene.h', 'r', 'cp932', errors='ignore') as f:
    header = f.read()

addition = '''
private:
    // === パラメータ定数 ===
    static constexpr float kMagicScaleMax = 1.8f;
    static constexpr float kMagicScaleMid = 1.3f;
    static constexpr float kMagicScaleMin = 0.7f;
'''

header = header.replace('};', addition + '\n};')

with codecs.open('Source/Magic_Ene.h', 'w', 'cp932') as f:
    f.write(header)

with codecs.open('Source/Magic_Ene.cpp', 'r', 'cp932', errors='ignore') as f:
    cpp = f.read()

cpp = cpp.replace('1.8f', 'kMagicScaleMax')
cpp = cpp.replace('1.3f', 'kMagicScaleMid')
cpp = cpp.replace('0.7f', 'kMagicScaleMin')
cpp = cpp.replace('GetColor(255, 30, 80)', 'GetColor(255, 30, 80) /* kColorRed */')

with codecs.open('Source/Magic_Ene.cpp', 'w', 'cp932') as f:
    f.write(cpp)
