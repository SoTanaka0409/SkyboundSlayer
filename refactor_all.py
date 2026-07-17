import os
import glob
import re

# We will supply the list of bad_vars manually to be safe
bad_vars = [
    "isLog", "isSeparate", "isSeparateAnim", "isWalk",
    "mActive", "mAge", "mAnimationInfoList", "mAttack1ComboCount", "mAttackState", "mAttackType",
    "mBgHandle1", "mBgHandle2", "mChargeTimer", "mCollected", "mDateList", "mDownBuffer", "mEffects",
    "mForwardSpeed", "mGravity", "mHasLandedHit", "mIsCutsceneMode", "mIsHolding", "mIsSucking",
    "mItemList", "mJumpStartY", "mJumpTargetDir", "mJumpTimer", "mJumpVelocity", "mMouseDownBuffer",
    "mOffset", "mOffsetPos", "mOffsetRot", "mPage", "mPlayingHandle", "mPool", "mSelectedIndex",
    "mShopState", "mSizeList", "mTargetPosition", "mUpBuffer", "mValue",
    "mbDelete", "mbElaseFlag", "mbGet", "mbGetItemflag", "mbIsFollow", "mbLoop", "mbLoopFinish",
    "mbMagic", "mbOldMouseDown", "mbUse",
    "mfAnimBlendRate", "mfAnimationCount", "mfAnimationTime", "mfHorizontalAngle", "mfMagicSize",
    "mfOldAnimationTime", "mfShakeAngle", "mfShakeAngleSpeed", "mfShakeTime", "mfShakeTimeCounter",
    "mfShakeWidth", "mfStepTime", "mfVerticalAngle",
    "mnAllNum", "mnAllTimer", "mnAnimationHandle", "mnAnimationIndex", "mnBgmHandleList", "mnCenter",
    "mnChangeTextureHandle", "mnColor1", "mnColor2", "mnColor3", "mnColor4", "mnCombo", "mnCounter",
    "mnCurrentNum", "mnFirstFont", "mnHandle", "mnHandleList", "mnHp", "mnIconAttackHandle",
    "mnIconEvasionDistHandle", "mnIconEvasionInvHandle", "mnIconMaxHpHandle", "mnIconSpeedHandle",
    "mnInterval", "mnInvisible", "mnLoopFinishState", "mnMaxHp", "mnMoney", "mnNowPlayingBgm",
    "mnNowPlayingSe", "mnOldAnimationIndex", "mnParentFrameIndex", "mnParentHandle", "mnSeHandleList",
    "mnSelect", "mnSelectMax", "mnSelectMin", "mnShakeTime", "mnShakeTimeCount", "mnSizeS", "mnSizeX",
    "mnSizeY", "mnSpeed", "mnState", "mnTagD", "mnTransFlag", "mnUpgradeAttackCount",
    "mnUpgradeEvasionSpeedCount", "mnUpgradeMaxHpCount", "mnUpgradeSpeedCount",
    "mpAnimation", "mpAttachCollider", "mpAttachment", "mpAttackCollider", "mpDate", "mpEffect",
    "mpEneList", "mpGoBossCollider", "mpHitCollider", "mpJumpAttackCoiider", "mpLandingAttackCollider",
    "mpObject", "mpParent", "mpSafeZoon", "mpSeparateAnimation", "mpSerchCollider", "mpShopIn",
    "mpStopCollider", "mpTexture0", "mpTexture1", "mpTexture3", "mpTexture4", "mpTexture5", "mpTexture6",
    "mpTexture7", "mpTexture8", "mpTexture9"
]

def to_snake_case(name):
    # Remove m, mn, mb, mp, mf prefixes
    if name.startswith('mn') or name.startswith('mb') or name.startswith('mp') or name.startswith('mf'):
        core = name[2:]
    elif name.startswith('m') and len(name) > 1 and name[1].isupper():
        core = name[1:]
    elif name.startswith('is'):
        core = name # keep 'is'
    else:
        core = name
        
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', core)
    s2 = re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()
    return s2 + '_'

replacements = {v: to_snake_case(v) for v in bad_vars}

def refactor_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8-sig') as f:
            content = f.read()
            original = content
    except:
        try:
            with open(filepath, 'r', encoding='cp932') as f:
                content = f.read()
                original = content
        except Exception as e:
            print(f"Error reading {filepath}: {e}")
            return

    for old, new in replacements.items():
        content = re.sub(rf'\b{old}\b', new, content)

    if content != original:
        with open(filepath, 'w', encoding='utf-8-sig') as f:
            f.write(content)
        print(f"Refactored {filepath}")

if __name__ == "__main__":
    files = glob.glob("Source/**/*.h", recursive=True) + glob.glob("Source/**/*.cpp", recursive=True)
    for f in files:
        refactor_file(f)
    print("Done refactoring remaining variables.")
