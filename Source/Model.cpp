#include"Model.h"
#include"AttachmentModel.h"
#include"Master.h"


/*
 * 目的（ModelのModel処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
    : position_(initPos)
    , attachment_(nullptr)
    , mvScale(VGet(1.0f, 1.0f, 1.0f))
    , change_texture_handle_(-1)
    ,is_separate_(isSeparateAnimation)
{
    handle_ = Master::resource_manager_->LoadModel(filename.c_str());


    if (isSeparateAnimation)
    {
        separate_animation_ = new SeparateModelAnimation(handle_);
        animation_ = nullptr;
    }
    else
    {
        animation_ = new ModelAnimation(handle_);
        separate_animation_ = nullptr;
    }
}


/*
 * 目的（ModelのAddAnimation処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::AddAnimation(AnimationState state, std::string filename)
{
    if (separate_animation_ != nullptr)
    {
        separate_animation_->AddAnimation(state, filename);
    }
}

Model::~Model()
{


    if (separate_animation_ != nullptr)
    {
        delete separate_animation_;
        separate_animation_ = nullptr;
    }
    if (animation_ != nullptr)
    {
        delete animation_;
        animation_ = nullptr;
    }

    if (attachment_ != nullptr)
    {
        attachment_->SetDeleteFlag(true);
    }

    if (change_texture_handle_ != -1)
    {
        DeleteGraph(change_texture_handle_);
        change_texture_handle_ = -1;
    }

    if (handle_ != -1) { 
        MV1DeleteModel(handle_); 
        handle_ = -1;
    }
}


/*
 * 目的（ModelのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::Update()
{
    if (animation_ != nullptr)
    {
        animation_->Update();
    }

    if (separate_animation_ != nullptr)
    {
        separate_animation_->Update();
    }

    MV1SetPosition(handle_, position_);

    

    MV1SetRotationXYZ(handle_, rotation_);
}


/*
 * 目的（ModelのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::Draw()
{
    MV1DrawModel(handle_);
}


/*
 * 目的（ModelのChangeAnimation処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::ChangeAnimation(AnimationState state)
{
    if (animation_ != nullptr)
    {
        animation_->ChangeAnimation(state);
    }
    if (separate_animation_ != nullptr)
    {
        separate_animation_->ChangeAnimation(state);
    }
}


/*
 * 目的（ModelのSetLoop処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::SetLoop(bool loop)
{
    if (animation_ != nullptr)
    {
        animation_->SetLoop(loop);
    }
    if (separate_animation_ != nullptr)
    {
        separate_animation_->SetLoop(loop);
    }
}


/*
 * 目的（ModelのSetLoopFinishState処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::SetLoopFinishState(AnimationState state)
{
    if (animation_ != nullptr)
    {
        animation_->SetLoopFinishState(state);
    }
    if (separate_animation_ != nullptr)
    {
        separate_animation_->SetLoopFinishState(state);
    }
}


/*
 * 目的（ModelのSetAnimationBlend処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::SetAnimationBlend(bool isBlend)
{
    if (animation_ != nullptr)
    {
        animation_->SetAnimationBlend(isBlend);
    }
    if (separate_animation_ != nullptr)
    {
        separate_animation_->SetAnimationBlend(isBlend);
    }
}


/*
 * 目的（ModelのGetNowState処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
AnimationState Model::GetNowState()
{
    AnimationState ret = AnimationState::ANIMATION_MAX;

    if (animation_ != nullptr)
    {
        ret = animation_->GetNowState();
    }
    if (separate_animation_ != nullptr)
    {
        ret = separate_animation_->GetNowState();
    }

    return ret;
}


/*
 * 目的（ModelのIsAnimationLoopFinish処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
bool Model::IsAnimationLoopFinish()
{

    bool ret = false;

    if (animation_ != nullptr)
    {
        ret = animation_->IsLoopFinish();
    }
    if (separate_animation_ != nullptr)
    {
        ret = separate_animation_->IsLoopFinish();
    }

    return ret;
}



/*
 * 目的（ModelのAddAttachment処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::AddAttachment(std::string filename, std::string attachFrameName, VECTOR offsetPos, VECTOR offsetRot)
{
    if (attachment_ != nullptr)
    {
        attachment_->SetDeleteFlag(true);
        attachment_ = nullptr;
    }
    int frameIndex = MV1SearchFrame(handle_, attachFrameName.c_str());
    if (frameIndex != -1)
    {
        attachment_ = new AttachmentModel(filename, handle_, frameIndex, offsetPos, offsetRot);
    }
}


/*
 * 目的（ModelのGetAttachmentPosition_None処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
VECTOR Model::GetAttachmentPosition_None(std::string attachFrameName)
{
    int frameIndex = MV1SearchFrame(handle_, attachFrameName.c_str());
    if (frameIndex != -1)
    {
        return MV1GetFramePosition(handle_, frameIndex);
    }
    return VGet(0.0f, 30.0f, 0.0f);
}





/*
 * 目的（ModelのGetAttachmentPosition処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
VECTOR Model::GetAttachmentPosition()
{
    if (attachment_ != nullptr)
    {
        VECTOR vec = VGet(0.0f, -50.0f, 0.0f);

        MATRIX matrix = MV1GetFrameLocalWorldMatrix(attachment_->GetHandle(), 0);

        vec = VTransform(vec, matrix);

        return vec;
    }

    return VGet(0.0f, 0.0f, 0.0f);
}





/*
 * 目的（ModelのSetScale処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::SetScale(VECTOR scale)
{
    MV1SetScale(handle_, scale);
}


/*
 * 目的（ModelのSetTexture処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Model::SetTexture(std::string filename, int index)
{
    if (change_texture_handle_ != -1)
    {
        DeleteGraph(change_texture_handle_);
    }

    change_texture_handle_ = Master::resource_manager_->LoadGraphics(filename);

    MV1SetTextureGraphHandle(handle_, index, change_texture_handle_, FALSE);
}

