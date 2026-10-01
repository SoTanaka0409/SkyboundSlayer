#include"Object2D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"GameScene.h"
#include"Scene.h"




/// @brief Object2Dの初期化（コンストラクタ）
Object2D::Object2D(VECTOR initPos)
	:m_Position(initPos)
	, m_Rotation(VGet(0.0f, 0.0f, 0.0f))
	, m_DeleteFlag(false)
	, m_Tag(Tag2D::None2D)
	, m_DrawFlag(true)
{
	// 現在のシーンのobjectManagerに自信（this)を追加する
	Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

/// @brief デストラクタ
Object2D::~Object2D()
{

}
/// @brief 描画

/// @brief Object2Dの描画処理
void Object2D::Draw()
{

}

/// @brief 更新

/// @brief Object2Dの状態更新処理
void Object2D::Update()
{

}
