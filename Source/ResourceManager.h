#pragma once
#pragma once
#include <vector>
#include <string>


// テクスチャの分割読み込み用データ
struct DivGraphData
{
	std::string filePath;	// テクスチャのパス名
	int* divHandleList;		// 分割されたテクスチャハンドルのリスト
	int divX;				// 横の分割数
	int divY;				// 縦の分割数
	int allNum;				// 分割最大数

	DivGraphData(
		std::string filePath,
		int divX,
		int divY,
		int allNum
	)
	{
		this->filePath = filePath;
		this->divHandleList = new int[allNum];
		this->divX = divX;
		this->divY = divY;
		this->allNum = allNum;
	}
};

// リソース管理クラス
class ResourceManager
{
public:
    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
	ResourceManager();
    // コンストラクタ
// [入力] なし [出力] なし [副作用] メンバ変数の初期化
    // デストラクタ
// [入力] なし [出力] なし [副作用] リソースの解放
	~ResourceManager();

	int LoadModel(std::string pathName);	// モデルリソース生成
	int LoadGraphics(std::string pathName);	// グラフィックリソース生成
	DivGraphData* LoadDivGraphics(std::string pathName, int allNum, int numX, int numY);	// 分割されたグラフィックリソース生成

    // 現在読み込んでいるリソースの総数を取得する
// [入力] なし [出力] int: リソース数 [副作用] なし
	int GetTotalResource() { return static_cast<int>(resourceMapList.size() + graphicResourceMapList.size() + divGraphicResourceMapList.size()); }

private:
	std::vector<std::pair<std::string, int>> resourceMapList;			// モデルリソースリスト
	std::vector<std::pair<std::string, int>> graphicResourceMapList;	// テクスチャリスト
	std::vector<DivGraphData*> divGraphicResourceMapList;				// 分割テクスチャリスト
};
