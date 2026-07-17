#include "Master.h"
#include "DxLib.h"
#include "SeparateModelAnimation.h"

/*
 * 目的（SeparateModelAnimationのSeparateModelAnimation処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
SeparateModelAnimation::SeparateModelAnimation(int modelHandle)
    : model_handle_(modelHandle)
    , animation_time_(0.0f)
    , animation_index_(-1)
    , old_animation_time_(0.0f)
    , old_animation_index_(-1)
    , anim_blend_rate_(1.0f)
    , state_(AnimationState::ANIMATION_MAX)    // 譛蛻昴・譛螟ｧ蛟､縺ｨ縺励※縺翫￥
    , loop_(true)
    , loop_finish_state_(AnimationState::ANIMATION_MAX)
    , loop_finish_(false)
    , animation_info_list_()
    , animation_count_(0.5f)
{
}

// 繝・せ繝医Λ繧ｯ繧ｿ
SeparateModelAnimation::~SeparateModelAnimation()
{
    // 霑ｽ蜉隱ｭ縺ｿ霎ｼ縺ｿ縺励◆繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ蜑企勁
    if (!animation_info_list_.empty())
    {
        for (auto itr = animation_info_list_.begin(); itr != animation_info_list_.end(); )
        {
            auto temp = *itr;
            itr = animation_info_list_.erase(itr);
            if (temp->animation_handle_ != -1) {
                MV1DeleteModel(temp->animation_handle_);
                temp->animation_handle_ = -1;
            }

            delete temp;
            temp = nullptr;
        }
    }
}

// 譖ｴ譁ｰ

/*
 * 目的（SeparateModelAnimationのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SeparateModelAnimation::Update()
{
    // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝悶Ξ繝ｳ繝臥紫繧帝ｲ繧√ｋ
    if (anim_blend_rate_ < 1.0f)
    {
        anim_blend_rate_ += 0.1f;    // += 0.1f 縺ｯ繝悶Ξ繝ｳ繝蛾溷ｺｦ縲り・逕ｱ縺ｫ螟峨∴縺ｦ繧Ｐk
        if (anim_blend_rate_ > 1.0f)
        {
            anim_blend_rate_ = 1.0f;
        }
    }


    // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譖ｴ譁ｰ
    float fAnimTotalTime = 0.0f;
    if (animation_index_ != -1)
    {
        // 邱丞・逕滓凾髢薙・蜿門ｾ・
        fAnimTotalTime = MV1GetAttachAnimTotalTime(model_handle_, animation_index_);

        // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ繧帝ｲ繧√ｋ
        animation_time_ += animation_count_;

        // 繝ｫ繝ｼ繝励＆縺帙ｋ
        if (animation_time_ > fAnimTotalTime)
        {
            // 繝ｫ繝ｼ繝励＠縺ｪ縺・ｨｭ螳壹〒縺ゅｌ縺ｰ
            if (!loop_)
            {
                // 谺｡縺ｮ繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺瑚ｨｭ螳壹＆繧後※縺・↑縺・ｴ蜷・
                if (loop_finish_state_ == ANIMATION_MAX)
                {
                    // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｯ縺薙ｌ莉･荳企ｲ繧√★縲∝・逅・ｒ荳ｭ譁ｭ縺輔○繧・
                    loop_finish_ = true;
                    return;
                }

                // 繝ｫ繝ｼ繝礼ｵゆｺ・凾縺ｮ繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｸ螟画峩
                ChangeAnimation(loop_finish_state_);
                // 繝悶Ξ繝ｳ繝峨・縺励↑縺・
                SetAnimationBlend(false);
                // 螟画峩縺輔ｌ縺溘・縺ｧ謾ｹ繧√※邱丞・逕滓凾髢薙ｒ縺ｨ縺｣縺ｦ縺翫￥
                fAnimTotalTime = MV1GetAttachAnimTotalTime(model_handle_, animation_index_);
            }

            animation_time_ = 0.0f;
        }

        // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ繧貞渚譏
        MV1SetAttachAnimTime(model_handle_, animation_index_, animation_time_);

        // 繝悶Ξ繝ｳ繝臥紫繧定ｨｭ螳・
        MV1SetAttachAnimBlendRate(model_handle_, animation_index_, anim_blend_rate_);
    }

    // ・代▽蜑阪・繝｢繝ｼ繧ｷ繝ｧ繝ｳ繧呈峩譁ｰ
    if (old_animation_index_ != -1)
    {
        // 邱丞・逕滓凾髢薙・蜿門ｾ・
        fAnimTotalTime = MV1GetAttachAnimTotalTime(model_handle_, old_animation_index_);

        // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ繧帝ｲ繧√ｋ
        old_animation_time_ += animation_count_;

        // 繝ｫ繝ｼ繝励＆縺帙ｋ
        if (old_animation_time_ > fAnimTotalTime)
        {
            old_animation_time_ = 0.0f;
        }

        // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ繧貞渚譏
        MV1SetAttachAnimTime(model_handle_, old_animation_index_, old_animation_time_);

        // 繝悶Ξ繝ｳ繝臥紫繧定ｨｭ螳・
        MV1SetAttachAnimBlendRate(model_handle_, old_animation_index_, 1.0f - anim_blend_rate_);
    }
}

// 繝｢繝ｼ繧ｷ繝ｧ繝ｳ蛻・ｊ譖ｿ縺・

/*
 * 目的（SeparateModelAnimationのChangeAnimation処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SeparateModelAnimation::ChangeAnimation(AnimationState state, int index)
{
    // 蛻・ｊ譖ｿ縺医ｈ縺・→縺励※縺・ｋ繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺後☆縺ｧ縺ｫ險ｭ螳壹＆繧後※縺・ｋ蝣ｴ蜷・
    if (state_ == state)
    {
        return;     // 菴輔ｂ縺励↑縺・
    }

    // 蛻・ｊ譖ｿ縺亥・縺ｮ逡ｪ蜿ｷ繧剃ｿ晄戟
    state_ = state;

    // 繝ｫ繝ｼ繝玲ュ蝣ｱ縺ｮ蛻晄悄蛹・
    loop_ = true;  // 險ｭ螳壹′迚ｹ縺ｫ縺ｪ縺・ｴ蜷医・繝ｫ繝ｼ繝励＆縺帙ｋ
    loop_finish_state_ = AnimationState::ANIMATION_MAX;  // 繝ｫ繝ｼ繝礼ｵゆｺ・凾縺ｮ繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｯ迚ｹ縺ｫ縺ｪ縺・
    loop_finish_ = false;

    // ・代▽蜑阪・繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺梧怏蜉ｹ迥ｶ諷九〒縺ゅｌ縺ｰ
    if (old_animation_index_ != -1)
    {
        // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝・ち繝・メ・亥叙繧雁､悶☆・・
        MV1DetachAnim(model_handle_, old_animation_index_);
        old_animation_index_ = -1;
    }

    // 迴ｾ蝨ｨ縺ｮ繝｢繝ｼ繧ｷ繝ｧ繝ｳ迥ｶ諷九ｒ菫晄戟縺吶ｋ
    old_animation_index_ = animation_index_;
    old_animation_time_ = animation_time_;

    // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繧｢繧ｿ繝・メ (NameSearch 繧・TRUE 縺ｫ縺励※繝懊・繝ｳ蜷阪〒繝槭ャ繝√Φ繧ｰ縺輔○繧・
    animation_index_ = MV1AttachAnim(model_handle_, index, GetAnimationHandle(state), TRUE);

    // 蜀咲函譎る俣縺ｮ蛻晄悄蛹・
    animation_time_ = 0.0f;

    // 繝悶Ξ繝ｳ繝臥憾諷九ｒ蛻晄悄蛹・
    // 繝悶Ξ繝ｳ繝臥紫縺ｯ縲∝商縺・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺梧怏蜉ｹ縺ｧ縺ｪ縺・ｴ蜷医・1.0f・医ヶ繝ｬ繝ｳ繝峨＠縺ｪ縺・憾諷具ｼ峨↓縺励※縺翫￥
    anim_blend_rate_ = (old_animation_index_ == -1 ? 1.0f : 0.0f);
}

// 繝｢繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝悶Ξ繝ｳ繝芽ｨｭ螳・

/*
 * 目的（SeparateModelAnimationのSetAnimationBlend処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SeparateModelAnimation::SetAnimationBlend(bool isBlend)
{
    if (isBlend)    // 繝悶Ξ繝ｳ繝峨☆繧句ｴ蜷・
    {
        // 繝悶Ξ繝ｳ繝臥紫縺ｯ縲∝商縺・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺梧怏蜉ｹ縺ｧ縺ｪ縺・ｴ蜷医・1.0f・医ヶ繝ｬ繝ｳ繝峨＠縺ｪ縺・憾諷具ｼ峨↓縺励※縺翫￥
        anim_blend_rate_ = (old_animation_index_ == -1 ? 1.0f : 0.0f);
    }
    else    // 繝悶Ξ繝ｳ繝峨＠縺ｪ縺・ｴ蜷・
    {
        // 繝悶Ξ繝ｳ繝峨＠縺ｪ縺・憾諷九↓縺吶ｋ
        anim_blend_rate_ = 1.0f;

        // 繝悶Ξ繝ｳ繝峨☆繧句ｿ・ｦ√′縺ｪ縺・・縺ｧ縲∝商縺・Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｯ繝・ち繝・メ縺励※縺翫￥
        if (old_animation_index_ != -1)
        {
            MV1DetachAnim(model_handle_, old_animation_index_);
            old_animation_index_ = -1;
        }
    }
}

// 繝｢繝ｼ繧ｷ繝ｧ繝ｳ霑ｽ蜉

/*
 * 目的（SeparateModelAnimationのAddAnimation処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void SeparateModelAnimation::AddAnimation(AnimationState state, std::string filename)
{
    // 繝｢繝ｼ繧ｷ繝ｧ繝ｳ繝｢繝・Ν隱ｭ縺ｿ霎ｼ縺ｿ
    int handle = Master::resource_manager_->LoadModel(filename.c_str());

    if (handle == -1)
    {
        return;
    }

    // AnimationState 縺ｨ隱ｭ縺ｿ霎ｼ繧薙□繝上Φ繝峨Ν縺ｮ邏舌▼縺・
    AnimationInfo* pInfo = new AnimationInfo();
    pInfo->state_ = state;
    pInfo->animation_handle_ = handle;
    animation_info_list_.push_back(pInfo);

    // NEUTRAL繝｢繝ｼ繧ｷ繝ｧ繝ｳ・亥ｾ・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ・峨′霑ｽ蜉縺輔ｌ縺溘ｉ繝｢繝ｼ繧ｷ繝ｧ繝ｳ螟画峩蜃ｦ逅・ｒ縺励※縺翫￥
    if (state == AnimationState::ANIMATION_NEUTRAL)
    {
        // 蛻晄悄迥ｶ諷九・蠕・ｩ溘Δ繝ｼ繧ｷ繝ｧ繝ｳ縺ｫ縺励※縺翫￥
        ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);
    }
}

// 蟇ｾ蠢懊＠縺溘Δ繝ｼ繧ｷ繝ｧ繝ｳ繝上Φ繝峨Ν縺ｮ蜿門ｾ・

/*
 * 目的（SeparateModelAnimationのGetAnimationHandle処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
int SeparateModelAnimation::GetAnimationHandle(AnimationState state)
{
    // 縺昴ｂ縺昴ｂ遨ｺ縺｣縺ｽ縺ｮ蝣ｴ蜷医・謗｢縺輔↑縺・
    if (animation_info_list_.empty())
    {
        return -1;
    }

    for (auto itr = animation_info_list_.begin(); itr != animation_info_list_.end(); itr++)
    {
        auto temp = *itr;

        // 蟇ｾ蠢懊☆繧九Δ繝ｼ繧ｷ繝ｧ繝ｳ繝上Φ繝峨Ν縺後≠繧後・縺昴ｌ繧定ｿ斐☆
        if (temp->state_ == state)
        {
            return temp->animation_handle_;
        }
    }

    // 隕九▽縺九ｉ縺ｪ縺九▲縺溘ｉ-1繧定ｿ斐☆
    return -1;
}
