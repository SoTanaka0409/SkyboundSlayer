#pragma once
#include "DxLib.h"
#include <string>
#include "ModelUtility.h"
#include "ModelAnimation.h"
#include "SeparateModelAnimation.h"

class AttachmentModel;

// DxLibの3Dモデル表示をラップし、姿勢制御、武器などのボーン追従（アタッチメント）、およびアニメーション管理を統括するクラス
class Model
{
public:
    // 入力: filename(モデルパス), initPos(初期座標), isSeparateAnimation(分離アニメ管理フラグ)
    // 出力: なし / 副作用: 3DモデルのVRAMロードと、アニメーションデータが本体同梱か別ファイルかに応じた管理クラスの初期化を行う
    Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);

    // 入力: なし / 出力: なし
    // 副作用: アニメーション管理インスタンスやアタッチメントモデル、自身のモデルハンドルを破棄しメモリリークを防ぐ
    ~Model();

    // 入力: なし / 出力: なし
    // 副作用: アニメーションの再生時間を進め、アタッチメント（武器等）の座標を親モデルの指定ボーン位置に同期させる
    void Update();

    // 入力: なし / 出力: なし
    // 副作用: 更新された座標・姿勢・アニメーション情報を適用し、3Dモデルを描画バッファへ登録する
    void Draw();

    // 入力: state (再生したいアニメーション状態) / 出力: なし
    // 副作用: 待機から走りなどへアニメーションを切り替える（ブレンド有効時はフレーム間を補間して滑らかに遷移させる）
    void ChangeAnimation(AnimationState state);

    void SetLoop(bool loop);
    void SetLoopFinishState(AnimationState state);

    // 入力: isBlend (補間の有効化フラグ) / 出力: なし
    // 副作用: モーション切り替え時の急な姿勢変化（カクつき）を防ぐためのブレンド処理のON/OFFを切り替える
    void SetAnimationBlend(bool isBlend);

    AnimationState GetNowState();

    // 入力: なし / 出力: ループ終了済みか(bool) / 副作用: なし
    // 攻撃モーションの終了検知など、次のアクションへステートを遷移させるための同期トリガーとして使用する
    bool IsAnimationLoopFinish();

    // 入力: filename(アタッチするモデルのパス), attachFrameName(追従先ボーン名), offsetPos, offsetRot / 出力: なし
    // 副作用: キャラクターの右手（指定フレーム）などに連動して動く武器や装飾品モデルを動的生成し、親子関係を構築する
    void AddAttachment(std::string filename, std::string attachFrameName, VECTOR offsetPos = VGet(0.0f, 0.0f, 0.0f), VECTOR offsetRot = VGet(0.0f, 0.0f, 0.0f));

    // 入力: なし / 出力: アタッチメントの現在ワールド座標 / 副作用: なし
    // 剣の切っ先の位置を取得して攻撃判定（コライダー）を生成する際などに使用する
    VECTOR GetAttachmentPosition();
    VECTOR GetAttachmentPosition_None(std::string attachFrameName);

    VECTOR GetPosition() { return position_; }
    void SetPosition(VECTOR pos) { position_ = pos; }

    VECTOR GetRotation() { return rotation_; }
    void SetRotation(VECTOR rot) { rotation_ = rot; }

    void SetScale(VECTOR scale);

    // 入力: filename(テクスチャパス), index(マテリアル番号) / 出力: なし
    // 副作用: 被ダメージ時の点滅や、状態異常時の色変えなどを行うため、指定マテリアルのテクスチャを動的に差し替える
    void SetTexture(std::string filename, int index = 0);

    bool GetIsSeparate() { return is_separate_; }

    // 入力: state(割り当てる状態), filename(モーションファイルのパス) / 出力: なし
    // 副作用: 分離アニメーション形式の場合、指定した外部モーションファイル(.mv1)を読み込みステートに紐付ける
    void AddAnimation(AnimationState state, std::string filename);

    SeparateModelAnimation* separate_animation_; // モーションが別ファイルに分離されている場合のアニメーション管理クラス
    ModelAnimation* animation_;                  // モデル本体にモーションが同梱されている場合のアニメーション管理クラス

private:
    int handle_;                   // DxLib側でロードされた3Dモデルの実体ハンドル
    VECTOR position_;              // ワールド空間上でのモデルの中心座標
    VECTOR rotation_;              // モデルのY軸などを基準とした回転（姿勢）
    VECTOR mvScale;                // モデルの描画スケール（初期サイズ調整や演出での拡縮に使用）
    int change_texture_handle_;    // 動的差し替え用にロードされたテクスチャのハンドル（破棄管理用）

    bool is_separate_;             // アニメーションデータが別ファイルに分かれているモデルかどうかのフラグ

    AttachmentModel* attachment_;  // 武器など、特定のボーンに追従させる別モデル（現状単一だが拡張時はvectorを推奨）
};