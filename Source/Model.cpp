#include"Model.h"
#include"AttachmentModel.h"
#include"Master.h"

Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
    : position_(initPos)
    , mpAttachment(nullptr)
    , mvScale(VGet(1.0f, 1.0f, 1.0f))
    , mnChangeTextureHandle(-1)
    ,isSeparate(isSeparateAnimation)
{
    mnHandle = Master::mpResourceManager->LoadModel(filename.c_str());


    if (isSeparateAnimation)
    {
        mpSeparateAnimation = new SeparateModelAnimation(mnHandle);
        mpAnimation = nullptr;
    }
    else
    {
        mpAnimation = new ModelAnimation(mnHandle);
        mpSeparateAnimation = nullptr;
    }
}

void Model::AddAnimation(AnimationState state, std::string filename)
{
    if (mpSeparateAnimation != nullptr)
    {
        mpSeparateAnimation->AddAnimation(state, filename);
    }
}

Model::~Model()
{


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

    if (mpAttachment != nullptr)
    {
        mpAttachment->SetDeleteFlag(true);
    }

    if (mnChangeTextureHandle != -1)
    {
        DeleteGraph(mnChangeTextureHandle);
        mnChangeTextureHandle = -1;
    }

    if (mnHandle != -1) { 
        MV1DeleteModel(mnHandle); 
        mnHandle = -1;
    }
}

void Model::Update()
{
    if (mpAnimation != nullptr)
    {
        mpAnimation->Update();
    }

    if (mpSeparateAnimation != nullptr)
    {
        mpSeparateAnimation->Update();
    }

    MV1SetPosition(mnHandle, position_);

    

    MV1SetRotationXYZ(mnHandle, rotation_);
}

void Model::Draw()
{
    MV1DrawModel(mnHandle);
}

void Model::ChangeAnimation(AnimationState state)
{
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


void Model::AddAttachment(std::string filename, std::string attachFrameName, VECTOR offsetPos, VECTOR offsetRot)
{
    if (mpAttachment != nullptr)
    {
        mpAttachment->SetDeleteFlag(true);
        mpAttachment = nullptr;
    }
    int frameIndex = MV1SearchFrame(mnHandle, attachFrameName.c_str());
    if (frameIndex != -1)
    {
        mpAttachment = new AttachmentModel(filename, mnHandle, frameIndex, offsetPos, offsetRot);
    }
}

VECTOR Model::GetAttachmentPosition_None(std::string attachFrameName)
{
    int frameIndex = MV1SearchFrame(mnHandle, attachFrameName.c_str());
    if (frameIndex != -1)
    {
        return MV1GetFramePosition(mnHandle, frameIndex);
    }
    return VGet(0.0f, 30.0f, 0.0f);
}




VECTOR Model::GetAttachmentPosition()
{
    if (mpAttachment != nullptr)
    {
        VECTOR vec = VGet(0.0f, -50.0f, 0.0f);

        MATRIX matrix = MV1GetFrameLocalWorldMatrix(mpAttachment->GetHandle(), 0);

        vec = VTransform(vec, matrix);

        return vec;
    }

    return VGet(0.0f, 0.0f, 0.0f);
}




void Model::SetScale(VECTOR scale)
{
    MV1SetScale(mnHandle, scale);
}

void Model::SetTexture(std::string filename, int index)
{
    if (mnChangeTextureHandle != -1)
    {
        DeleteGraph(mnChangeTextureHandle);
    }

    mnChangeTextureHandle = Master::mpResourceManager->LoadGraphics(filename);

    MV1SetTextureGraphHandle(mnHandle, index, mnChangeTextureHandle, FALSE);
}

