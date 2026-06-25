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
    mvPosition.y += 30.0f;
    mpModel = new Model(filename, mvPosition, false);
    mpModel->SetScale(VGet(150.0f, 150.0f, 150.0f)); // Make it larger to be visible
}

Coin::~Coin()
{
    if (mpModel) {
        delete mpModel;
        mpModel = nullptr;
    }
}

void Coin::Draw()
{
    if (mpModel) {
        mpModel->Draw();
    }
}

void Coin::Update()
{
    if (mCollected) return;

    mAge++;
    
    // Rotate the coin for visibility
    mvRotation.y += 0.1f;
    if (mpModel) {
        mpModel->SetRotation(mvRotation);
    }

    // Initial pop physics
    if (mAge < 20) {
        mvPosition.y += 2.0f;
        if (mpModel) mpModel->SetPosition(mvPosition);
        return; // Don't suck yet
    }

    auto mpPlayer = Master::mpPlayer;
    if (!mpPlayer) return;

    Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
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
            if (mpModel) {
                mpModel->SetPosition(myPos);
            }
        }
    }

    if (dist < 80.0f && !mCollected) {
        player->mpHaveMoney->AddMoney(mValue);
        mCollected = true;
        SetDeleteFlag(true);
    }
}
