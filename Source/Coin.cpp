#include "Coin.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "HaveMoneyClass.h"
#include <math.h>

/// @brief Coinの初期化（コンストラクタ）
Coin::Coin(std::string filename, VECTOR pos, int value)
    : Object3D(pos)
    , value_(value)
    , is_sucking_(false)
    , collected_(false)
    , age_(0)
{
    SetTag(Object3D::Tag3D_Obj);
    // 地面より上に出現するようにYオフセットを追加
    position_.y += kSpawnOffsetY;
    model_ = new Model(filename, position_, false);
    model_->SetScale(VGet(kScale, kScale, kScale));
}

Coin::~Coin()
{
    if (model_) {
        delete model_;
        model_ = nullptr;
    }
}

/// @brief Coinの描画処理
void Coin::Draw()
{
    if (model_) {
        model_->Draw();
    }
}

/// @brief Coinの状態更新処理
void Coin::Update()
{
    if (collected_) return;

    age_++;
    
    // 視認性を高めるためにコインを回転させる
    rotation_.y += 0.1f;
    if (model_) {
        model_->SetRotation(rotation_);
    }

    UpdatePopPhysics();
    UpdateSuckToPlayer();
}

/// @brief CoinのUpdatePopPhysics処理
void Coin::UpdatePopPhysics()
{
    // 出現時の物理挙動
    if (age_ < kPopDuration) {
        position_.y += kPopSpeedY;
        if (model_) model_->SetPosition(position_);
    }
}

/// @brief CoinのUpdateSuckToPlayer処理
void Coin::UpdateSuckToPlayer()
{
    if (age_ < kPopDuration) return; // Don't suck yet

    auto player = Master::player_;
    if (!player) return;

    VECTOR pPos = player->GetPosition();
    // Aim for player's center, not feet
    pPos.y += 50.0f;
    VECTOR myPos = GetPosition();
    
    float dx = pPos.x - myPos.x;
    float dy = pPos.y - myPos.y;
    float dz = pPos.z - myPos.z;
    float dist = sqrt(dx*dx + dy*dy + dz*dz);

    if (dist < kSuckRadius) {
        is_sucking_ = true;
    }

    if (is_sucking_) {
        if (dist > 0.0f) {
            myPos.x += (dx / dist) * kSuckSpeed;
            myPos.y += (dy / dist) * kSuckSpeed;
            myPos.z += (dz / dist) * kSuckSpeed;
            SetPosition(myPos);
            if (model_) {
                model_->SetPosition(myPos);
            }
        }
    }

    if (dist < kCollectRadius && !collected_) {
        player->have_money_->AddMoney(value_);
        collected_ = true;
        SetDeleteFlag(true);
    }
}
