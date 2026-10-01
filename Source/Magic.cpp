#include "Magic.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Effect.h"
#include "EffectPool.h"

/// @brief Magicクラスのコンストラクタ
/// @param filename テクスチャのファイルパス
/// @param initPos 初期生成座標
/// @param r 魔法弾の判定サイズ（半径）
/// @param damage 攻撃力・ダメージ量
/// @param speed 移動速度
/// @param movevec 移動方向ベクトル
/// @param count 削除カウント初期値
/// @param time 削除（寿命）までの制限フレーム時間
/// @details 描画用テクスチャの読み込みおよび球体当たり判定（SphereCollider）の生成を行う
Magic::Magic(std::string filename, VECTOR initPos, float r, float damage, float speed, VECTOR movevec, int count, int time)
	: Object3D(initPos)
	, m_Speed(speed)
	, mfAttack_chara(damage)
	, m_MagicSize(r)
	, m_MoveVec(movevec)
	, DeleteCount(count)
	, DeleteTime(time)
	, Filename(filename)
	, m_Attack(0)
{
	m_GraphHandle = LoadGraph(filename.c_str());
	m_HitCollider = new SphereCollider(this, m_Position, m_MagicSize);
}

/// @brief Magicクラスのデストラクタ
/// @details ロードしたテクスチャグラフィックハンドルの解放処理を行う
Magic::~Magic()
{
	DeleteGraph(m_GraphHandle);
}

/// @brief 魔法弾の3Dビルボード描画処理を行う
/// @details 加算合成（DX_BLENDMODE_ADD）を適用して発光感を演出して描画する
void Magic::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawBillboard3D(m_Position, 0.5f, 0.5f, m_MagicSize, 0.0f, m_GraphHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// @brief 毎フレームの更新処理を行う
/// @details 生存タイマーの加算、移動計算を行い、寿命超過時に死亡（着弾・消滅）処理を呼び出す
void Magic::Update()
{
	DeleteCount++;
	Move();
	if (DeleteCount > DeleteTime)
	{
		Death();
	}
}

/// @brief 魔法弾の移動処理を行う
/// @details 移動方向ベクトル（m_MoveVec）と速度（m_Speed）に基づき座標を加算更新する
void Magic::Move()
{
	m_Position = VAdd(m_Position, VScale(m_MoveVec, m_Speed));
	if (m_HitCollider != nullptr)
	{
		m_HitCollider->m_Position = m_Position;
	}
}

/// @brief 魔法弾の着弾・消滅処理を行う
/// @details 消滅時エフェクトの再生、オブジェクト自身の破棄フラグおよびコライダー破棄フラグを有効化する
void Magic::Death()
{
	EffectPool::GetInstance()->Play(m_Position, Filename, GetColorU8(255, 0, 0, 0), m_MagicSize, 0.1f);
	SetDeleteFlag(true);
	m_HitCollider->SetDeleteFlag(true);
}

/// @brief 他のコライダーと接触した瞬間のイベント処理
/// @param collider 自身のコライダー
/// @param check 接触した相手のコライダー
void Magic::OnEnter(Collider* collider, Collider* check)
{
}

/// @brief 他のコライダーと接触し続けている間のイベント処理
/// @param collider 自身のコライダー
/// @param check 接触した相手のコライダー
void Magic::OnTrigger(Collider* collider, Collider* check)
{
}

/// @brief 他のコライダーから離れた瞬間のイベント処理
/// @param collider 自身のコライダー
/// @param check 離れた相手のコライダー
void Magic::OnExit(Collider* collider, Collider* check)
{
}