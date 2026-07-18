#include <fstream>
#include <sstream>
#include"Scene3D.h"
#include"Config.h"
#include"Enemy3D_AT.h"
#include"Player3D.h"
#include"InputManager.h"
#include"Enemy3D.h"
#include"ObjectManager.h"
#include"Stage.h"
#include"Tree.h"
#include"StatShop.h"
#include"Wall.h"
#include"EnemyManager.h"
#include"SkyBox.h"
#include"StageCollider.h"
#include"Rock.h"
#include"StageObject.h"

// 入力：なし
// 出力：なし
// 副作用：ロードタイマーやフラグなど、シーン初期化に必要なベースステータスのデフォルト値設定
Scene3D::Scene3D()
	: load_timer_(500)
	, load_count_(0)
	, is_load_flag_(true)
{
}

// 入力：なし
// 出力：なし
// 副作用：Scene3Dインスタンスの破棄
Scene3D::~Scene3D()
{
}

// 入力：なし
// 出力：なし
// 副作用：ゲーム本編の環境（ライト・フォグ）、アクター、ステージ、およびCSV定義オブジェクトの一括セットアップ処理
void Scene3D::Initialize()
{
	SceneGame::Initialize();
	SetupEnvironment();
	CreateInitialActors();
	CreateStage();
	LoadStageObjectsFromCsv();
	CreateSkyBox();
}

// 入力：なし
// 出力：なし
// 副作用：DxLibの3D描画環境におけるフォグ（霧）と各種グローバルライティングの適用
void Scene3D::SetupEnvironment()
{
	// 演出仕様：空間の広がりと奥行き（空気遠近法）を表現するためのフォグ設定と、3Dモデル全体を均一に照らして視認性を確保する環境光・指向性ライトのベースセットアップ
	SetFogEnable(TRUE);
	SetFogColor(20, 30, 50);
	SetFogStartEnd(500.0f, 10000.0f);

	SetGlobalAmbientLight(GetColorF(0.15f, 0.15f, 0.2f, 1.0f));

	SetLightDifColor(GetColorF(0.6f, 0.6f, 0.8f, 1.0f));
	SetLightSpcColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));
	SetLightDirection(VNorm(VGet(-0.5f, -0.8f, 0.3f)));
}

// 入力：なし
// 出力：なし
// 副作用：自機（Player3D）、ショップ、キャンプファイヤーなど、ステージに依存せず必ず存在するコアアクター群の動的生成
void Scene3D::CreateInitialActors()
{
	new Player3D("Resource/Model/T.mv1", VGet(-1200, 20.0f, -1000), 30.0f, 12.0f, 150.0f, true);
	new StatShop("Resource/Model/shop.mv1", VGet(-1500, 100, 1500));
	new StageObject(VGet(-1250, 20.0f, -1050), "Resource/3D/low-poly-campfire/source/campfire.mv1", VGet(10.0f, 10.0f, 10.0f));
}

// 入力：なし
// 出力：なし
// 副作用：メインステージおよび遠景用（浮遊島等）の巨大な地形スタティックメッシュの生成
void Scene3D::CreateStage()
{
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1", VGet(3.0f, 0.3f, 3.0f));
}

// 入力：なし
// 出力：なし
// 副作用：外部CSVファイルのパース、および定義内容に基づく各種ステージオブジェクト（障害物、壁、小物など）の座標計算とインスタンス大量生成
void Scene3D::LoadStageObjectsFromCsv()
{
	// アーキテクチャ設計：ステージの構成要素をC++コード内にハードコードせず、外部CSVから動的に読み込むデータ駆動設計。これにより、ビルド（コンパイル）を挟まずにレベルデザインのイテレーション（調整・再配置）を高速に回すことが可能になる
	std::ifstream file("Resource/CSV/stage_objects.csv");
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line);
		while (std::getline(file, line))
		{
			if (line.empty()) continue;
			std::stringstream ss(line);
			std::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr, texture, colSizeStr, isRelativeStr;

			std::getline(ss, type, ',');
			std::getline(ss, model, ',');
			std::getline(ss, xStr, ',');
			std::getline(ss, yStr, ',');
			std::getline(ss, zStr, ',');
			std::getline(ss, sxStr, ',');
			std::getline(ss, syStr, ',');
			std::getline(ss, szStr, ',');
			std::getline(ss, texture, ',');
			std::getline(ss, colSizeStr, ',');
			std::getline(ss, isRelativeStr, ',');

			float x = std::stof(xStr);
			float y = std::stof(yStr);
			float z = std::stof(zStr);
			float sx = std::stof(sxStr);
			float sy = std::stof(syStr);
			float sz = std::stof(szStr);
			int isRelative = 0;
			if (!isRelativeStr.empty()) isRelative = std::stoi(isRelativeStr);

			VECTOR pos = VGet(x, y, z);

			// レベルデザイン：絶対座標だけでなく「ステージ中心からの相対座標」での配置をサポートし、ステージ全体の座標がシフトした際にもアセット群が破綻なく追従できる設計
			if (isRelative == 1)
			{
				pos = VAdd(Config::GetStageCenter(), pos);
			}
			VECTOR scale = VGet(sx, sy, sz);

			if (type == "StageObject")
			{
				float colSize = 0.0f;
				if (!colSizeStr.empty()) colSize = std::stof(colSizeStr);
				new StageObject(pos, model, scale, "", colSize);
			}
			else if (type == "Stage")
			{
				new Stage(pos, model, model, scale, texture);
			}
		}
	}
}

// 入力：なし
// 出力：なし
// 副作用：全方位の遠景を覆う天球（スカイボックス）モデルの生成とテクスチャ適用
void Scene3D::CreateSkyBox()
{
	SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x", VGet(0, 0, -5000));
	float scale = 13.0f;
	pSkyBox->SetScale(VGet(scale, scale, scale));
	pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");
}

// 入力：なし
// 出力：なし
// 副作用：オブジェクト群のフレーム進行、セーブデータの更新、ゲームクリア判定の監視、および結果に応じたリザルトシーンへの遷移トリガー
void Scene3D::Update()
{
	SceneGame::Update();
	Master::save_->Update();
	Player3D* player = Master::player_;

	// レベルデザイン：ボスの撃破などによりフェーズが「クリア」に移行したことを検知し、最終的なプレイヤーステータス（HP・攻撃・速度）をスコア算定用にキャプチャしてリザルトシーンへバトンタッチする
	if (game_manager_->GetCurrentPhase() == GameManager::Phase::kClear)
	{
		Master::game_clear_count_ = 2;
	}

	if (Master::game_clear_count_ == 2)
	{
		if (player != nullptr && Master::score_manager_ != nullptr)
		{
			Master::score_manager_->SetFinalStats(player->GetAllStatusState(Object3D::Status_Hp), player->GetAllStatusState(Object3D::Status_Attack), player->GetAllStatusState(Object3D::Status_Speed));
		}
		Master::scene_manager_->SetNextScene(SceneManager::kSceneResultWin);
	}

	// バグ回避：物理演算等の衝突解決漏れでプレイヤーがステージ境界外へ飛び出してしまった場合、即座に前フレームの安全な座標へ強制的に巻き戻して落下死やスタックを防止する
	if (player->GetStageOutFlag() == false)
	{
		player->GetPosition() = player->GetOldPosition();
	}
}

// 入力：なし
// 出力：なし
// 副作用：本編の3D空間描画、セーブUI等のレイヤー描画、およびデバッグ用グリッドのレンダリング
void Scene3D::Draw()
{
	SceneGame::Draw();
	Master::save_->Draw();
	DrawDebugGrid();
}

// 入力：なし
// 出力：なし
// 副作用：原点を基準としたXZ平面上のワイヤーフレームメッシュ（距離500px刻み）のレンダリング
void Scene3D::DrawDebugGrid()
{
	// 視覚的フィードバック：開発中にアクターの座標スケール感や、移動の絶対的な速度感を視覚的に把握しやすくするためのデバッグ専用フロアグリッド
	const int count = 51;
	const float distance = 500.0f;
	for (int i = 0; i < count; i++)
	{
		float base = (count / 2 - i) * -distance;

		DrawLine3D(
			VGet(-distance * (count / 2), 0.0f, base),
			VGet(distance * (count / 2), 0.0f, base),
			GetColor(255, 255, 255)
		);

		DrawLine3D(
			VGet(base, 0.0f, -distance * (count / 2)),
			VGet(base, 0.0f, distance * (count / 2)),
			GetColor(255, 255, 255)
		);
	}
}

// 入力：なし
// 出力：なし
// 副作用：BGMの強制停止、基底クラスの破棄ロジック呼び出し、およびシーン専用の環境エフェクト（フォグ）の無効化
void Scene3D::Finalize()
{
	Master::sound_manager_->StopBGM();
	SceneGame::Finalize();

	// メモリ管理：次のシーン（リザルト画面やタイトルなど）に予期せぬフォグ設定が引き継がれて画面が白濁するバグを防ぐためのステート初期化
	SetFogEnable(FALSE);
}