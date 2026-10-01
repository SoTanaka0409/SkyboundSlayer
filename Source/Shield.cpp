#include"Shield.h"
#include"Model.h"
#include"Player3D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"GameScene.h"
#include"SceneManager.h"
#include"Scene.h"




/// @brief Shieldの初期化（コンストラクタ）
Shield::Shield(std::string filename, VECTOR initPos,int hp)
	:Object3D(initPos)
	,NewShield(true)
	,m_Hp(hp)
	,m_SizeS(200.0f)
{
	SetTag(Object3D::Tag_3D_Shield);
	m_Model=new Model(filename, initPos, 1.0f);
	auto m_Player = Master::m_Player;
	auto pPlayer = Master::m_Player;

	m_Position = pPlayer->GetPosition();
}

Shield::~Shield()
{
	if (m_Model != nullptr)
	{
		delete m_Model;
	}
}


/// @brief Shieldの状態更新処理
void Shield::Update()
{
	auto m_Player = Master::m_Player;
	auto pPlayer = Master::m_Player;
	m_Position = pPlayer->GetPosition();
}


/// @brief Shieldの描画処理
void Shield::Draw()
{
	DrawCapsule3D(m_Position, VAdd(m_Position, VGet(0.0f, 80.0f, 0.0f)),
		m_SizeS,
		8,
		GetColor(0, 255, 255),
		GetColor(0, 255, 255),
		false
	);

}
