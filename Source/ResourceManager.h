#pragma once
#pragma once
#include <vector>
#include <string>


/// @brief テクスチャの分割読み込み用データ
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

/// @brief リソース管理クラス
class ResourceManager
{
public:
    /// @brief コンストラクタ
	ResourceManager();
    /// @brief コンストラクタ
    /// @brief デストラクタ
	~ResourceManager();

	int LoadModel(std::string pathName);	// モデルリソース生成
	int LoadGraphics(std::string pathName);	// グラフィックリソース生成
	DivGraphData* LoadDivGraphics(std::string pathName, int allNum, int numX, int numY);	// 分割されたグラフィックリソース生成

    /// @brief 現在読み込んでいるリソースの総数を取得する
	int GetTotalResource() { return static_cast<int>(resourceMapList.size() + graphicResourceMapList.size() + divGraphicResourceMapList.size()); }

private:
	std::vector<std::pair<std::string, int>> resourceMapList;			// モデルリソースリスト
	std::vector<std::pair<std::string, int>> graphicResourceMapList;	// テクスチャリスト
	std::vector<DivGraphData*> divGraphicResourceMapList;				// 分割テクスチャリスト
};
