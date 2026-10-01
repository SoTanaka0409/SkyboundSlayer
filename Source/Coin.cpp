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
    , m_Value(value)
    , m_IsSucking(false)
    , m_Collected(false)
    , m_Age(0)
{
    SetTag(Object3D::Tag3D_Obj);
    // 地面より上に出現するようにYオフセットを追加
    m_Position.y += kSpawnOffsetY;
    m_Model = new Model(filename, m_Position, false);
    m_Model->SetScale(VGet(kScale, kScale, kScale));
}

Coin::~Coin()
{
    if (m_Model) {
        delete m_Model;
        m_Model = nullptr;
    }
}

/// @brief Coinの描画処理
void Coin::Draw()
{
    if (m_Model) {
        m_Model->Draw();
    }
}

/// @brief Coinの状態更新処理
void Coin::Update()
{
    if (m_Collected) return;

    m_Age++;
    
    // 視認性を高めるためにコインを回転させる
    m_Rotation.y += 0.1f;
    if (m_Model) {
        m_Model->SetRotation(m_Rotation);
    }

    UpdatePopPhysics();
    UpdateSuckToPlayer();
}

/// @brief CoinのUpdatePopPhysics処理
void Coin::UpdatePopPhysics()
{
    // 出現時の物理挙動
    if (m_Age < kPopDuration) {
        m_Position.y += kPopSpeedY;
        if (m_Model) m_Model->SetPosition(m_Position);
    }
}

/// @brief CoinのUpdateSuckToPlayer処理
void Coin::UpdateSuckToPlayer()
{
    if (m_Age < kPopDuration) return; // Don't suck yet

    auto player = Master::m_Player;
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
        m_IsSucking = true;
    }

    if (m_IsSucking) {
        if (dist > 0.0f) {
            myPos.x += (dx / dist) * kSuckSpeed;
            myPos.y += (dy / dist) * kSuckSpeed;
            myPos.z += (dz / dist) * kSuckSpeed;
            SetPosition(myPos);
            if (m_Model) {
                m_Model->SetPosition(myPos);
            }
        }
    }

    if (dist < kCollectRadius && !m_Collected) {
        player->m_HaveMoney->AddMoney(m_Value);
        m_Collected = true;
        SetDeleteFlag(true);
    }
}
