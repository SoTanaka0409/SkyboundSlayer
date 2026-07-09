#pragma once
#include "DxLib.h"
#include <string>
#include "ModelUtility.h"
#include "ModelAnimation.h"
#include "SeparateModelAnimation.h"

// 蜑肴婿螳｣險
class AttachmentModel;


class Model
{
public:

    // 笘・ew笘・
    // 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
    // note: 蛻・牡繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧剃ｽｿ縺・°縺ｩ縺・°縺ｮ險ｭ螳壹ｒ霑ｽ蜉縲・
    Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);
    ~Model();   // 繝・せ繝医Λ繧ｯ繧ｿ

    void Update();  // 譖ｴ譁ｰ
    void Draw();    // 謠冗判

    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蛻・ｊ譖ｿ縺・
    void ChangeAnimation(AnimationState state);
    // 繝ｫ繝ｼ繝苓ｨｭ螳・
    void SetLoop(bool loop);
    void SetLoopFinishState(AnimationState state);
    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝悶Ξ繝ｳ繝芽ｨｭ螳・
    void SetAnimationBlend(bool isBlend);
    // 迴ｾ蝨ｨ蜀咲函縺輔ｌ縺ｦ縺・ｋ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ蜿門ｾ・
    AnimationState GetNowState();
    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝ｫ繝ｼ繝励′邨ゆｺ・＠縺ｦ縺・ｋ縺九←縺・° 
    bool IsAnimationLoopFinish();


    // 繧｢繧ｿ繝・メ繝｢繝・Ν髢｢騾｣ //
    // 繧｢繧ｿ繝・メ繝｡繝ｳ繝医ｒ霑ｽ蜉
    void AddAttachment(std::string filename, std::string attachFrameName, VECTOR offsetPos = VGet(0.0f, 0.0f, 0.0f), VECTOR offsetRot = VGet(0.0f, 0.0f, 0.0f));
    
   
    // 繧｢繧ｿ繝・メ繝｢繝・Ν縺ｮ蠎ｧ讓吝叙蠕・
    VECTOR GetAttachmentPosition();
    VECTOR GetAttachmentPosition_None(std::string attachFrameName);


    VECTOR GetPosition() { return position_; } // 蠎ｧ讓吝叙蠕・
    void SetPosition(VECTOR pos) { position_ = pos; }  // 蠎ｧ讓呵ｨｭ螳・

    VECTOR GetRotation() { return rotation_; } // 蝗櫁ｻ｢蜿門ｾ・
    void SetRotation(VECTOR rot) { rotation_ = rot; }  // 蝗櫁ｻ｢險ｭ螳・

    void SetScale(VECTOR scale);
    void SetTexture(std::string filename, int index = 0);

    bool GetIsSeparate() { return isSeparate; }

    // 笘・ew笘・
    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繝・・繧ｿ縺ｮ霑ｽ蜉
    // note: SeparateModelAnimation 繧ｯ繝ｩ繧ｹ縺ｸ縺ｮ讖区ｸ｡縺鈴未謨ｰ
    void AddAnimation(AnimationState state, std::string filename);

    // 蛻・牡隱ｭ縺ｿ霎ｼ縺ｿ繝舌・繧ｸ繝ｧ繝ｳ縺ｮ繝｢繝・Ν繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ繝昴う繝ｳ繧ｿ
    SeparateModelAnimation* mpSeparateAnimation;
    ModelAnimation* mpAnimation;    // 繝｢繝・Ν繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ繝昴う繝ｳ繧ｿ
private:
    int mnHandle;   // 隱ｭ縺ｿ霎ｼ繧薙□繝｢繝・Ν縺ｮ繝上Φ繝峨Ν
    VECTOR position_;  // 蠎ｧ讓・
    VECTOR rotation_;  // 蝗櫁ｻ｢
    VECTOR mvScale;
    int mnChangeTextureHandle;

    bool isSeparate;

    // 笘・ew笘・
   

    AttachmentModel* mpAttachment;  // 繧｢繧ｿ繝・メ繝｢繝・Ν・郁､・焚謖√◆縺帙◆縺・ｴ蜷医・ std::vector 繧・・蛻励〒邂｡逅・☆繧九→濶ｯ縺・ｼ・
};