import codecs
import re

# Update EnemyBoss_1.h
with codecs.open('Source/EnemyBoss_1.h', 'r', 'utf-8-sig') as f:
    header = f.read()

header_addition = '''
    // === ボスの攻撃タイプ ===
    enum class BossAttackType {
        kCombo = 0,
        kMagic = 1,
        kJump = 2
    };

    // === パラメータ定数 ===
    static constexpr float kJumpAttackDamage = 10.0f;
    static constexpr int kJumpChargeFrames = 30;
    static constexpr float kJumpInitialVelocity = 80.0f;
    static constexpr float kMagicScale = 75.0f;
    static constexpr float kMagicDamage = 5.0f;
    static constexpr float kMagicSpeed = 30.0f;
    static constexpr int kMagicLifetime = 150;
    static constexpr int kAttackChanceThreshold = 30;

    static constexpr float kJumpAscendSpeed = 15.0f;
    static constexpr float kJumpDescendSpeed = -25.0f;
'''

header = re.sub(r'static constexpr float kJumpAscendSpeed = 15\.0f;.*?\n.*?static constexpr float kJumpDescendSpeed = -25\.0f;.*?\n', header_addition, header, flags=re.DOTALL)
header = header.replace('int attack_type_;', 'BossAttackType attack_type_;')

with codecs.open('Source/EnemyBoss_1.h', 'w', 'utf-8-sig') as f:
    f.write(header)

# Update EnemyBoss_1.cpp
with codecs.open('Source/EnemyBoss_1.cpp', 'r', 'utf-8-sig') as f:
    cpp = f.read()

cpp = cpp.replace('attack_type_ = 0;', 'attack_type_ = BossAttackType::kCombo;')
cpp = cpp.replace('attack_type_ = GetRand(2);', 'attack_type_ = static_cast<BossAttackType>(GetRand(2));')
cpp = cpp.replace('if (attack_type_ == 0)', 'if (attack_type_ == BossAttackType::kCombo)')
cpp = cpp.replace('else if (attack_type_ == 1)', 'else if (attack_type_ == BossAttackType::kMagic)')
cpp = cpp.replace('else if (attack_type_ == 2)', 'else if (attack_type_ == BossAttackType::kJump)')
cpp = cpp.replace('attack_type_ == 0 && attack1_combo_count_ > 0', 'attack_type_ == BossAttackType::kCombo && attack1_combo_count_ > 0')
cpp = cpp.replace('attack_type_ == 2', 'attack_type_ == BossAttackType::kJump')

cpp = cpp.replace('chance_ = 30;', 'chance_ = kAttackChanceThreshold;')

cpp = cpp.replace('75.0f, 5, 30.0f', 'kMagicScale, kMagicDamage, kMagicSpeed')
cpp = cpp.replace(', 0, 150);', ', 0, kMagicLifetime);')

cpp = cpp.replace('pPlayer->Damage(10.0f);', 'pPlayer->Damage(kJumpAttackDamage);')
cpp = cpp.replace('if (jump_charge_timer_ <= 30)', 'if (jump_charge_timer_ <= kJumpChargeFrames)')
cpp = cpp.replace('if (jump_charge_timer_ == 30)', 'if (jump_charge_timer_ == kJumpChargeFrames)')
cpp = cpp.replace('jump_velocity_ = 80.0f;', 'jump_velocity_ = kJumpInitialVelocity;')

with codecs.open('Source/EnemyBoss_1.cpp', 'w', 'utf-8-sig') as f:
    f.write(cpp)
