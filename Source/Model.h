#pragma once
#include "DxLib.h"
#include <string>
#include "ModelUtility.h"
#include "ModelAnimation.h"
#include "SeparateModelAnimation.h"

// 前方宣言
class AttachmentModel;


class Model
{
public:

    // ☁Eew☁E
    // コンストラクタ
    // note: 刁E��アニメーションを使ぁE��どぁE��の設定を追加、E
    Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);
    ~Model();   // チE��トラクタ

    void Update();  // 更新
    void Draw();    // 描画

    // アニメーション刁E��替ぁE
    void ChangeAnimation(AnimationState state);
    // ループ設宁E
    void SetLoop(bool loop);
    void SetLoopFinishState(AnimationState state);
    // アニメーションのブレンド設宁E
    void SetAnimationBlend(bool isBlend);
    // 現在再生されてぁE��アニメーションの取征E
    AnimationState GetNowState();
    // アニメーションのループが終亁E��てぁE��かどぁE�� 
    bool IsAnimationLoopFinish();


    // アタチE��モチE��関連 //
    // アタチE��メントを追加
    void AddAttachment(std::string filename, std::string attachFrameName, VECTOR offsetPos = VGet(0.0f, 0.0f, 0.0f), VECTOR offsetRot = VGet(0.0f, 0.0f, 0.0f));
    
   
    // アタチE��モチE��の座標取征E
    VECTOR GetAttachmentPosition();
    VECTOR GetAttachmentPosition_None(std::string attachFrameName);


    VECTOR GetPosition() { return position_; } // 座標取征E
    void SetPosition(VECTOR pos) { position_ = pos; }  // 座標設宁E

    VECTOR GetRotation() { return rotation_; } // 回転取征E
    void SetRotation(VECTOR rot) { rotation_ = rot; }  // 回転設宁E

    void SetScale(VECTOR scale);
    void SetTexture(std::string filename, int index = 0);

    bool GetIsSeparate() { return isSeparate; }

    // ☁Eew☁E
    // アニメーションチE�Eタの追加
    // note: SeparateModelAnimation クラスへの橋渡し関数
    void AddAnimation(AnimationState state, std::string filename);

    // 刁E��読み込みバ�EジョンのモチE��アニメーションクラスのポインタ
    SeparateModelAnimation* mpSeparateAnimation;
    ModelAnimation* mpAnimation;    // モチE��アニメーションクラスのポインタ
private:
    int mnHandle;   // 読み込んだモチE��のハンドル
    VECTOR position_;  // 座樁E
    VECTOR rotation_;  // 回転
    VECTOR mvScale;
    int mnChangeTextureHandle;

    bool isSeparate;

    // ☁Eew☁E
   

    AttachmentModel* mpAttachment;  // アタチE��モチE���E�褁E��持たせたぁE��合�E std::vector めE�E列で管琁E��ると良ぁE��E
};