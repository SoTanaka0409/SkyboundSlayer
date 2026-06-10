#include "EffectU.h"
#include <cmath>

/*====================================================
  ユーティリティ関数
====================================================*/
// 度→ラジアン
static float DegToRad(float deg) { return deg * (DX_PI_F / 180.0f); }

// 0..1 のランダム
static float Rand01() { return (float)(GetRand(10000) / 10000.0f); }

// a..b の範囲ランダム
static float RandRange(float a, float b) { return a + Rand01() * (b - a); }

// a..b の範囲ランダム整数（両端含む）
static int RandIntInclusive(int a, int b)
{
    if (b <= a) return a;
    return a + GetRand(b - a);
}

/*====================================================
  読み込み（テクスチャロードなど）
====================================================*/
bool EffectU::Load()
{
    if (mTex != -1) return true;

    mTex = LoadGraph("Resource/mahoujin.png");
    if (mTex == -1) return false;

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetUseLighting(FALSE);

    return true;
}

/*====================================================
  ホールド開始（魔法陣表示開始）
====================================================*/
void EffectU::StartHold(const VECTOR& playerPos)
{
    if (mTex == -1) return;

    // 中心位置設定
    mCenter = playerPos;
    mCenter.y += mYOffset;

    mState = State::Holding;
    mPieces.clear();

    // 回転初期化
    mHoldRotY = 0.0f;

    // PreShatter用タイマー初期化
    mPreShatterTimer = 0;
}

/*====================================================
  離して破壊開始
====================================================*/
void EffectU::ReleaseAndShatter()
{
    if (mTex == -1) return;
    if (mState != State::Holding) return;

    // すぐ破壊せず、原型を少し残してから砕く
    mState = State::PreShatter;
    mPreShatterTimer = 0;
}

/*====================================================
  毎フレーム更新
====================================================*/
void EffectU::UpdateFollow(const VECTOR& /*playerPos*/)
{
    if (mTex == -1) return;
    if (mState == State::Idle) return;

    // -------------------------------------------------
    // Holding：回転のみ
    // -------------------------------------------------
    if (mState == State::Holding)
    {
        mHoldRotY += mHoldRotSpd;
        return;
    }

    // -------------------------------------------------
    // PreShatter：原型のまま少し残してから砕く
    // -------------------------------------------------
    if (mState == State::PreShatter)
    {
        if (mPreShatterKeepRotating)
        {
            mHoldRotY += mHoldRotSpd;
        }

        mPreShatterTimer++;
        if (mPreShatterTimer >= mPreShatterFrames)
        {
            BuildPieces(mCenter, mHoldRotY); // 破片生成
            mState = State::Shatter;
        }
        return;
    }

    // -------------------------------------------------
    // Shatter：破片更新
    // -------------------------------------------------
    if (mState == State::Shatter)
    {
        if (mPieces.empty())
        {
            mState = State::Idle;
            return;
        }

        int aliveCount = 0;

        for (auto& p : mPieces)
        {
            if (!p.alive) continue;

            p.age++;

            // 遅延中：その場で少しだけ揺れる
            if (p.age < p.startDelay)
            {
                if (mIdleJitter > 0.0f)
                {
                    p.pos.x += (Rand01() * 2.0f - 1.0f) * mIdleJitter * 0.02f;
                    p.pos.z += (Rand01() * 2.0f - 1.0f) * mIdleJitter * 0.02f;
                }
                ++aliveCount;
                continue;
            }

            // 上方向加速
            p.vel.y += p.upAccel;

            // 減衰（X/Z/Y別）
            p.vel.x *= mDampXZ;
            p.vel.z *= mDampXZ;
            p.vel.y *= mDampY;

            // 位置更新
            p.pos = VAdd(p.pos, p.vel);

            // 回転更新
            p.rotY += p.rotSpd;

            // フェードアウト
            p.alpha -= mFadeSpd;
            if (p.alpha <= 0.0f)
            {
                p.alpha = 0.0f;
                p.alive = false;
                continue;
            }

            ++aliveCount;
        }

        if (aliveCount == 0)
        {
            mPieces.clear();
            mState = State::Idle;
        }
    }
}

/*====================================================
  描画処理
====================================================*/
void EffectU::Draw() const
{
    if (mTex == -1) return;

    SetDrawBright(mBright, mBright, mBright);

    // Holding / PreShatter は原型を描画
    if (mState == State::Holding || mState == State::PreShatter)
    {
        DrawHoldingBoard();
    }
    // Shatter は破片描画
    else if (mState == State::Shatter)
    {
        for (const auto& p : mPieces)
        {
            if (!p.alive) continue;
            DrawPiece(p);
        }
    }

    SetDrawBright(255, 255, 255);
}

/*====================================================
  破片生成
====================================================*/
void EffectU::BuildPieces(const VECTOR& center, float baseRotY)
{
    mPieces.clear();
    mPieces.reserve(mDiv * mDiv);

    const float cell = mBoardSize / (float)mDiv;
    const float half = mBoardSize * 0.5f;
    const float du = 1.0f / (float)mDiv;
    const float dv = 1.0f / (float)mDiv;

    const float bc = cosf(baseRotY);
    const float bs = sinf(baseRotY);

    for (int iy = 0; iy < mDiv; ++iy)
    {
        for (int ix = 0; ix < mDiv; ++ix)
        {
            Piece p;
            p.alive = true;
            p.alpha = 255.0f;
            p.age = 0;

            // 遅延時間
            p.startDelay = RandIntInclusive(mStartDelayMin, mStartDelayMax);

            // 上方向加速度ランダム
            p.upAccel = mUpAccelBase * RandRange(0.7f, 1.35f);

            const float localX = -half + (ix + 0.5f) * cell;
            const float localZ = -half + (iy + 0.5f) * cell;

            // 回転反映した初期位置
            const float rx = localX * bc - localZ * bs;
            const float rz = localX * bs + localZ * bc;
            p.pos = VAdd(center, VGet(rx, 0.0f, rz));

            p.halfSize = cell * 0.5f;

            // 破片自身の回転
            p.rotY = baseRotY + DegToRad((float)GetRand(360));
            p.rotSpd = DegToRad((float)(GetRand(60) - 30)) * 0.04f;

            // 初速
            const float nx = localX / (half + 0.0001f);
            const float nz = localZ / (half + 0.0001f);
            const float upStart = RandRange(mUpStartMin, mUpStartMax);
            p.vel = VGet(
                nx * mOutPower + (Rand01() * 2.0f - 1.0f) * mRandJitter,
                upStart,
                nz * mOutPower + (Rand01() * 2.0f - 1.0f) * mRandJitter
            );

            // UV座標
            p.u0 = ix * du;        p.v0 = iy * dv;
            p.u1 = (ix + 1) * du;  p.v1 = (iy + 1) * dv;

            mPieces.push_back(p);
        }
    }
}

/*====================================================
  原型（魔法陣）描画
====================================================*/
void EffectU::DrawHoldingBoard() const
{
    const float half = mBoardSize * 0.5f;
    const float c = cosf(mHoldRotY);
    const float s = sinf(mHoldRotY);

    const VECTOR local[4] =
    {
        VGet(-half, 0.0f, -half),
        VGet(half, 0.0f, -half),
        VGet(half, 0.0f,  half),
        VGet(-half, 0.0f,  half),
    };

    VECTOR world[4];
    for (int i = 0; i < 4; ++i)
    {
        const float x = local[i].x;
        const float z = local[i].z;
        const float rx = x * c - z * s;
        const float rz = x * s + z * c;
        world[i] = VAdd(mCenter, VGet(rx, 0.0f, rz));
    }

    VERTEX3D v4[4]{};
    const DxLib::COLOR_U8 dif = GetColorU8(255, 255, 255, 255);
    const DxLib::COLOR_U8 spc = GetColorU8(0, 0, 0, 0);
    const VECTOR norm = VGet(0.0f, 1.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        v4[i].pos = world[i];
        v4[i].norm = norm;
        v4[i].dif = dif;
        v4[i].spc = spc;
    }

    v4[0].u = 0.0f; v4[0].v = 0.0f;
    v4[1].u = 1.0f; v4[1].v = 0.0f;
    v4[2].u = 1.0f; v4[2].v = 1.0f;
    v4[3].u = 0.0f; v4[3].v = 1.0f;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    SetUseBackCulling(FALSE);

    VERTEX3D tri1[3] = { v4[0], v4[1], v4[2] };
    VERTEX3D tri2[3] = { v4[0], v4[2], v4[3] };
    DrawPolygon3D(tri1, 3, mTex, TRUE);
    DrawPolygon3D(tri2, 3, mTex, TRUE);

    SetUseBackCulling(TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/*====================================================
  破片描画
====================================================*/
void EffectU::DrawPiece(const Piece& p) const
{
    const float c = cosf(p.rotY);
    const float s = sinf(p.rotY);
    const float hs = p.halfSize;

    const VECTOR local[4] =
    {
        VGet(-hs, 0.0f, -hs),
        VGet(hs, 0.0f, -hs),
        VGet(hs, 0.0f,  hs),
        VGet(-hs, 0.0f,  hs),
    };

    VECTOR world[4];
    for (int i = 0; i < 4; ++i)
    {
        const float x = local[i].x;
        const float z = local[i].z;
        const float rx = x * c - z * s;
        const float rz = x * s + z * c;
        world[i] = VAdd(p.pos, VGet(rx, 0.0f, rz));
    }

    VERTEX3D v4[4]{};
    const DxLib::COLOR_U8 dif = GetColorU8(255, 255, 255, 255);
    const DxLib::COLOR_U8 spc = GetColorU8(0, 0, 0, 0);
    const VECTOR norm = VGet(0.0f, 1.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        v4[i].pos = world[i];
        v4[i].norm = norm;
        v4[i].dif = dif;
        v4[i].spc = spc;
    }

    v4[0].u = p.u0; v4[0].v = p.v0;
    v4[1].u = p.u1; v4[1].v = p.v0;
    v4[2].u = p.u1; v4[2].v = p.v1;
    v4[3].u = p.u0; v4[3].v = p.v1;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)p.alpha);
    SetUseBackCulling(FALSE);

    VERTEX3D tri1[3] = { v4[0], v4[1], v4[2] };
    VERTEX3D tri2[3] = { v4[0], v4[2], v4[3] };
    DrawPolygon3D(tri1, 3, mTex, TRUE);
    DrawPolygon3D(tri2, 3, mTex, TRUE);

    SetUseBackCulling(TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
