#include"Model.h"
#include"AttachmentModel.h"
#include"Master.h"

// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
    : position_(initPos)
    , mpAttachment(nullptr)
    , mvScale(VGet(1.0f, 1.0f, 1.0f))
    , mnChangeTextureHandle(-1)
    ,isSeparate(isSeparateAnimation)
{
    // 繝｢繝・Ν縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
    mnHandle = Master::mpResourceManager->LoadModel(filename.c_str());

    //繧ｵ繧､繧ｺ險ｭ螳・

    // 笘・ew笘・
    // 譚｡莉ｶ蛻・ｲ舌ｒ霑ｽ蜉
    if (isSeparateAnimation)
    {
        // 蛻・牡繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ逕滓・
        mpSeparateAnimation = new SeparateModelAnimation(mnHandle);
        mpAnimation = nullptr;
    }
    else
    {
        // 騾壼ｸｸ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ逕滓・
        mpAnimation = new ModelAnimation(mnHandle);
        mpSeparateAnimation = nullptr;
    }
}

// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ霑ｽ蜉
void Model::AddAnimation(AnimationState state, std::string filename)
{
    if (mpSeparateAnimation != nullptr)
    {
        mpSeparateAnimation->AddAnimation(state, filename);
    }
}

// 繝・せ繝医Λ繧ｯ繧ｿ
Model::~Model()
{
    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ遐ｴ譽・
    if (mpAnimation != nullptr)
    {
        delete mpAnimation;
    }

    // 笘・ew笘・
    if (mpSeparateAnimation != nullptr)
    {
        delete mpSeparateAnimation;
        mpSeparateAnimation = nullptr;
    }
    if (mpAnimation != nullptr)
    {
        delete mpAnimation;
        mpAnimation = nullptr;
    }

    // 繧｢繧ｿ繝・メ繝｢繝・Ν繧ｯ繝ｩ繧ｹ縺ｮ遐ｴ譽・
    if (mpAttachment != nullptr)
    {
        mpAttachment->SetDeleteFlag(true);
    }

    // 繝・け繧ｹ繝√Ε繧貞・繧頑崛縺医※縺・ｋ蝣ｴ蜷医・縺昴・繝・け繧ｹ繝√Ε縺ｮ遐ｴ譽・
    if (mnChangeTextureHandle != -1)
    {
        DeleteGraph(mnChangeTextureHandle);
        mnChangeTextureHandle = -1;
    }

    // 隱ｭ縺ｿ霎ｼ繧薙□繝｢繝・Ν縺ｮ蜑企勁
    // note: 隱ｭ縺ｿ霎ｼ繧薙□繝｢繝・Ν縺ｯ蜍晄焔縺ｫ遐ｴ譽・＠縺ｦ縺上ｌ縺ｪ縺・・縺ｧ縲∝ｿ・ｦ√↑縺上↑縺｣縺溘ｉ謇句虚縺ｧ遐ｴ譽・☆繧・
    if (mnHandle != -1) { 
        MV1DeleteModel(mnHandle); 
        mnHandle = -1;
    }
}

// 譖ｴ譁ｰ
void Model::Update()
{
    if (mpAnimation != nullptr)
    {
        // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譖ｴ譁ｰ
        mpAnimation->Update();
    }

    // 笘・ew笘・
    // 蛻・牡繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譖ｴ譁ｰ
    if (mpSeparateAnimation != nullptr)
    {
        mpSeparateAnimation->Update();
    }

    // 蠎ｧ讓呵ｨｭ螳・
    MV1SetPosition(mnHandle, position_);

    

    // 蝗櫁ｻ｢險ｭ螳・
    MV1SetRotationXYZ(mnHandle, rotation_);
}

// 謠冗判
void Model::Draw()
{
    // 繝｢繝・Ν縺ｮ謠冗判
    MV1DrawModel(mnHandle);
}

// 繧｢繝九Γ繧ｷ繝ｧ繝ｳ蛻・ｊ譖ｿ縺・
void Model::ChangeAnimation(AnimationState state)
{
    // 笘・ew笘・
    // 騾壼ｸｸ or 蛻・牡縺ｮ縺ｩ縺｡繧峨°繧剃ｽｿ縺｣縺ｦ縺・ｋ縺九〒蛻・ｲ・
    if (mpAnimation != nullptr)
    {
        mpAnimation->ChangeAnimation(state);
    }
    if (mpSeparateAnimation != nullptr)
    {
        mpSeparateAnimation->ChangeAnimation(state);
    }
}

void Model::SetLoop(bool loop)
{
    // 笘・ew笘・
    // 騾壼ｸｸ or 蛻・牡縺ｮ縺ｩ縺｡繧峨°繧剃ｽｿ縺｣縺ｦ縺・ｋ縺九〒蛻・ｲ・
    if (mpAnimation != nullptr)
    {
        mpAnimation->SetLoop(loop);
    }
    if (mpSeparateAnimation != nullptr)
    {
        mpSeparateAnimation->SetLoop(loop);
    }
}

void Model::SetLoopFinishState(AnimationState state)
{
    // 笘・ew笘・
    // 騾壼ｸｸ or 蛻・牡縺ｮ縺ｩ縺｡繧峨°繧剃ｽｿ縺｣縺ｦ縺・ｋ縺九〒蛻・ｲ・
    if (mpAnimation != nullptr)
    {
        mpAnimation->SetLoopFinishState(state);
    }
    if (mpSeparateAnimation != nullptr)
    {
        mpSeparateAnimation->SetLoopFinishState(state);
    }
}

void Model::SetAnimationBlend(bool isBlend)
{
    // 笘・ew笘・
     // 騾壼ｸｸ or 蛻・牡縺ｮ縺ｩ縺｡繧峨°繧剃ｽｿ縺｣縺ｦ縺・ｋ縺九〒蛻・ｲ・
    if (mpAnimation != nullptr)
    {
        mpAnimation->SetAnimationBlend(isBlend);
    }
    if (mpSeparateAnimation != nullptr)
    {
        mpSeparateAnimation->SetAnimationBlend(isBlend);
    }
}

AnimationState Model::GetNowState()
{
    // 笘・ew笘・
    // 騾壼ｸｸ or 蛻・牡縺ｮ縺ｩ縺｡繧峨°繧剃ｽｿ縺｣縺ｦ縺・ｋ縺九〒蛻・ｲ・
    // note: ・医⊇縺ｼ縺ゅｊ縺医↑縺・′・峨ｂ縺励←縺｡繧峨ｂ辟｡縺代ｌ縺ｰ縲∫音縺ｫ險ｭ螳壹・縺ｪ縺・怙螟ｧ蛟､繧定ｿ斐☆繧医≧縺ｫ縺吶ｋ
    AnimationState ret = AnimationState::ANIMATION_MAX;

    if (mpAnimation != nullptr)
    {
        ret = mpAnimation->GetNowState();
    }
    if (mpSeparateAnimation != nullptr)
    {
        ret = mpSeparateAnimation->GetNowState();
    }

    return ret;
}

bool Model::IsAnimationLoopFinish()
{
    // 笘・ew笘・
    // 騾壼ｸｸ or 蛻・牡縺ｮ縺ｩ縺｡繧峨°繧剃ｽｿ縺｣縺ｦ縺・ｋ縺九〒蛻・ｲ・
    // note: ・医⊇縺ｼ縺ゅｊ縺医↑縺・′・峨ｂ縺励←縺｡繧峨ｂ辟｡縺代ｌ縺ｰ縲’alse 繧定ｿ斐☆繧医≧縺ｫ縺励※縺翫￥

    bool ret = false;

    if (mpAnimation != nullptr)
    {
        ret = mpAnimation->IsLoopFinish();
    }
    if (mpSeparateAnimation != nullptr)
    {
        ret = mpSeparateAnimation->IsLoopFinish();
    }

    return ret;
}


// 繧｢繧ｿ繝・メ繝｡繝ｳ繝医ｒ霑ｽ蜉
void Model::AddAttachment(std::string filename, std::string attachFrameName, VECTOR offsetPos, VECTOR offsetRot)
{
    if (mpAttachment != nullptr)
    {
        mpAttachment->SetDeleteFlag(true);
        mpAttachment = nullptr;
    }
    // 繧｢繧ｿ繝・メ蜈医・繝輔Ξ繝ｼ繝逡ｪ蜿ｷ繧貞叙蠕・
    int frameIndex = MV1SearchFrame(mnHandle, attachFrameName.c_str());
    if (frameIndex != -1)
    {
        // 繧｢繧ｿ繝・メ繝｡繝ｳ繝医Δ繝・Ν縺ｮ逕滓・
        mpAttachment = new AttachmentModel(filename, mnHandle, frameIndex, offsetPos, offsetRot);
    }
}

VECTOR Model::GetAttachmentPosition_None(std::string attachFrameName)
{
    if (this != nullptr)
    {
        int frameIndex = MV1SearchFrame(mnHandle, attachFrameName.c_str());
        VECTOR pos = VGet(0.0f, 30.0f, 0.0f);
        pos = MV1GetFramePosition(mnHandle, frameIndex);
        return pos;
    }
}




// 繧｢繧ｿ繝・メ繝｢繝・Ν縺ｮ蠎ｧ讓吝叙蠕・
VECTOR Model::GetAttachmentPosition()
{
    if (mpAttachment != nullptr)
    {
        VECTOR vec = VGet(0.0f, -50.0f, 0.0f);

        // 陦悟・縺ｮ蜿門ｾ・
        MATRIX matrix = MV1GetFrameLocalWorldMatrix(mpAttachment->GetHandle(), 0);

        // 陦悟・諠・ｱ繧偵ｂ縺ｨ縺ｫ蠎ｧ讓吝､画鋤縺吶ｋ
        vec = VTransform(vec, matrix);

        return vec;
    }

    // 繧｢繧ｿ繝・メ繝｡繝ｳ繝医′辟｡縺・ｴ蜷医・蜴溽せ繧定ｿ斐＠縺ｦ縺翫￥
    return VGet(0.0f, 0.0f, 0.0f);
}




void Model::SetScale(VECTOR scale)
{
    MV1SetScale(mnHandle, scale);
}

void Model::SetTexture(std::string filename, int index)
{
    // 繝・け繧ｹ繝√Ε繧偵☆縺ｧ縺ｫ蛻・ｊ譖ｿ縺医※縺・ｋ蝣ｴ蜷医・縺昴・繝・け繧ｹ繝√Ε縺ｮ遐ｴ譽・
    if (mnChangeTextureHandle != -1)
    {
        DeleteGraph(mnChangeTextureHandle);
    }

    // 繝・け繧ｹ繝√Ε縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
    mnChangeTextureHandle = Master::mpResourceManager->LoadGraphics(filename);

    // 隱ｭ縺ｿ霎ｼ繧薙□繝・け繧ｹ繝√Ε縺ｮ蜿肴丐
    MV1SetTextureGraphHandle(mnHandle, index, mnChangeTextureHandle, FALSE);
}
