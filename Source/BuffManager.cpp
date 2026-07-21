#include "BuffManager.h"

/// @brief BuffManagerクラスのコンストラクタ
BuffManager::BuffManager()
{
}

/// @brief BuffManagerクラスのデストラクタ
BuffManager::~BuffManager()
{
}

/// @brief 各種バフエフェクト・アイコンの描画処理を行う
void BuffManager::Draw()
{
}

/// @brief 適用中の全バフのタイマー進行や効果更新を行う
void BuffManager::Update()
{
	for (auto b : buffList)
	{
		(*b).Update();
	}
}

/// @brief 新しいバフ効果を追加・更新する
/// @param date 追加・更新するバフのデータポインタ
/// @details 同一タイプのバフが既に存在する場合はタイマーとカウントを上書き更新し、存在しない場合はリストに新規登録する
void BuffManager::AddBuff(Buff* date)
{
	for (auto b : buffList)
	{
		if (b->efDate.type == date->efDate.type)
		{
			b->efDate.type = date->efDate.type;
			b->efDate.Count = date->efDate.Count;
			date->efDate.Effect = 0;
			return;
		}
	}

	date->efDate.use_ = true;
	buffList.push_back(date);
}

/// @brief 制限時間が経過したバフをリストから削除・整理する
void BuffManager::DeleteList()
{
	for (auto b = buffList.begin(); b != buffList.end();)
	{
		if ((*b)->efDate.Timer <= (*b)->efDate.Count)
		{
			b = buffList.erase(b);
			return;
		}
		else
		{
			b++;
		}
	}
}

/// @brief 指定したステータス種別に対応する有効なバフ効果量を取得する
/// @param state 対象となるステータス種別（StatusState）
/// @return float 適用されるバフ効果量（補正値）
float BuffManager::GetBuff(Object3D::StatusState state)
{
	for (auto b : buffList)
	{
		if (b->efDate.type == state)
		{
			if ((*b).efDate.use_ == true)
			{
				return b->efDate.Effect;
			}
		}
	}

	return 0.0f;
}