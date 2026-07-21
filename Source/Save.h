#pragma once
#include "DxLib.h"
#include "EquipmentManager.h"
#include "ItemManager.h"
#include "Texture.h"
#include <list>

/// @brief セーブデータの保存・読み込み・セーブスロット選択UIを管理するクラス
class Save
{
public:
	/// @brief セーブスロット識別用列挙型
	enum SaveNumber
	{
		Save_1,   ///< セーブスロット 1
		Save_2,   ///< セーブスロット 2
		Save_3,   ///< セーブスロット 3
		Save_Max, ///< セーブスロット最大数
	};

	/// @brief セーブ対象となるプレイヤーおよびインベントリデータを保持する構造体
	struct SaveDate
	{
		SaveNumber num;                                  ///< 保存先のセーブスロット番号
		float hp = 0;                                    ///< プレイヤーの現在HP
		float MaxHp = 0;                                 ///< プレイヤーの最大HP
		float attack = 0;                                ///< プレイヤーの基礎攻撃力
		float speed = 0;                                 ///< プレイヤーの基礎移動速度
		std::list<Item::ItemInformation*> I_date;         ///< 所持アイテム情報リスト
		std::list<Equipment::EquipmentDate*> E_date;      ///< 所持・装備データリスト
	};

public:
	/// @brief Saveクラスのコンストラクタ
	Save();

	/// @brief Saveクラスのデストラクタ
	~Save();

	/// @brief セーブ画面・スロット選択UIの描画処理を行う
	void Draw();

	/// @brief 入力受付およびスロット選択状態の更新処理を行う
	void Update();

	/// @brief プレイヤーデータをファイル（またはメモリ）へ保存（セーブ）する
	/// @param date 保存するデータ構造体へのポインタ
	void GameSave(SaveDate* date);

	/// @brief ファイル（またはメモリ）からプレイヤーデータを読み込む（ロードする）
	/// @param date 読み込み先のデータ構造体へのポインタ
	void GameLoad(SaveDate* date);

	/// @brief 選択されているセーブスロットの確定・クラス選択処理を行う
	void SelectClass();

private:
	Texture* texture_;                 ///< セーブ画面背景・UI枠等のテクスチャポインタ
	std::list<SaveDate*> AllSaveDate;  ///< 全セーブスロットのデータを管理するリスト

	int Select;                        ///< 現在選択されているセーブスロットインデックス
	int SelectMax;                     ///< 選択インデックスの最大値
	int SelectMin;                     ///< 選択インデックスの最小値
};