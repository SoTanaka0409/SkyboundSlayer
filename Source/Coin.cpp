#include "Coin.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "HaveMoneyClass.h"
#include <math.h>

Coin::Coin(std::string filename, VECTOR pos, int value)
    : Object3D(pos)
    , mValue(value)
    , mIsSucking(false)
    , mCollected(false)
    , mAge(0)
{
    SetTag(Object3D::Tag3D_Obj);
    // Add Y offset so it spawns above ground
    position_.y += 30.0f;
    model_ = new Model(filename, position_, false);
    model_->SetScale(VGet(150.0f, 150.0f, 150.0f)); // Make it larger to be visible
}

Coin::~Coin()
{
    if (model_) {
        delete model_;
        model_ = nullptr;
    }
}

void Coin::Draw()
{
    if (model_) {
        model_->Draw();
    }
}

void Coin::Update()
{
    if (mCollected) return;

    mAge++;
    
    // Rotate the coin for visibility
    rotation_.y += 0.1f;
    if (model_) {
        model_->SetRotation(rotation_);
    }

    // Initial pop physics
    if (mAge < 20) {
        position_.y += 2.0f;
        if (model_) model_->SetPosition(position_);
        return; // Don't suck yet
    }

    auto mpPlayer = Master::mpPlayer;
    if (!mpPlayer) return;

    Player3D* player = Master::mpPlayer;
    if (!player) return;

    VECTOR pPos = player->GetPosition();
    // Aim for player's center, not feet
    pPos.y += 50.0f;
    VECTOR myPos = GetPosition();
    
    float dx = pPos.x - myPos.x;
    float dy = pPos.y - myPos.y;
    float dz = pPos.z - myPos.z;
    float dist = sqrt(dx*dx + dy*dy + dz*dz);

    if (dist < 600.0f) {
        mIsSucking = true;
    }

    if (mIsSucking) {
        float speed = 30.0f; // Faster suck
        if (dist > 0.0f) {
            myPos.x += (dx / dist) * speed;
            myPos.y += (dy / dist) * speed;
            myPos.z += (dz / dist) * speed;
            SetPosition(myPos);
            if (model_) {
                model_->SetPosition(myPos);
            }
        }
    }

    if (dist < 80.0f && !mCollected) {
        player->mpHaveMoney->AddMoney(mValue);
        mCollected = true;
        SetDeleteFlag(true);
    }
}
