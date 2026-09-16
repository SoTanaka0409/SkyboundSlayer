import codecs
import re

with codecs.open('Source/Player3D.h', 'r', 'cp932', errors='ignore') as f:
    header = f.read()

# Add constants to Player3D.h
addition = '''
    // === パラメータ定数 ===
    static constexpr float kAnimSpeedIdle = 0.5f;
    static constexpr float kAnimSpeedAttack = 0.9f;
    static constexpr float kAnimSpeedAttackSub = 0.35f;
    static constexpr float kAnimSpeedSlide = 1.2f;
    static constexpr float kAnimSpeedAttackSlide = 1.2f;
    static constexpr float kAnimSpeedDefault = 1.0f;
'''

header = header.replace('private:\r\n', addition + '\r\nprivate:\r\n')
header = header.replace('private:\n', addition + '\nprivate:\n')

with codecs.open('Source/Player3D.h', 'w', 'cp932') as f:
    f.write(header)

with codecs.open('Source/Player3D.cpp', 'r', 'cp932', errors='ignore') as f:
    cpp = f.read()

cpp = cpp.replace('SetAnimationCount(0.5f)', 'SetAnimationCount(kAnimSpeedIdle)')
cpp = cpp.replace('SetAnimationCount(0.9f)', 'SetAnimationCount(kAnimSpeedAttack)')
cpp = cpp.replace('SetAnimationCount(0.35f)', 'SetAnimationCount(kAnimSpeedAttackSub)')
cpp = cpp.replace('SetAnimationCount(1.2f)', 'SetAnimationCount(kAnimSpeedSlide)')
cpp = cpp.replace('SetAnimationCount(1.0f)', 'SetAnimationCount(kAnimSpeedDefault)')

with codecs.open('Source/Player3D.cpp', 'w', 'cp932') as f:
    f.write(cpp)
