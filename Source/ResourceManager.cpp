#include "DxLib.h"
#include "ResourceManager.h"


/*
 * 目的（ResourceManagerのコンストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
ResourceManager::ResourceManager()
{

}


/*
 * 目的（ResourceManagerのデストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] リソースハンドルの削除
 */
ResourceManager::~ResourceManager()
{
    // 3D繝｢繝・Ν縺ｮ繧ｪ繝ｪ繧ｸ繝翫Ν繝上Φ繝峨Ν繧貞炎髯､縺吶ｋ
    for (int i = 0; i < (int)resourceMapList.size(); i++)
    {
        MV1DeleteModel(resourceMapList.at(i).second);
    }
    resourceMapList.clear();

    // 逕ｻ蜒上ワ繝ｳ繝峨Ν繧貞炎髯､縺吶ｋ
    for (int i = 0; i < (int)graphicResourceMapList.size(); i++)
    {
        DeleteGraph(graphicResourceMapList.at(i).second);
    }
    graphicResourceMapList.clear();

    // 蛻・牡逕ｻ蜒上ワ繝ｳ繝峨Ν繧貞炎髯､縺吶ｋ
    for (int i = 0; i < (int)divGraphicResourceMapList.size(); i++)
    {
        DivGraphData* data = divGraphicResourceMapList.at(i);
        if (data != nullptr)
        {
            // 蛻・牡縺輔ｌ縺溷推繝上Φ繝峨Ν繧貞炎髯､
            for (int j = 0; j < data->allNum; j++)
            {
                DeleteGraph(data->divHandleList[j]);
            }
            delete[] data->divHandleList;
            delete data;
        }
    }
    divGraphicResourceMapList.clear();
}

// 繝｢繝・Ν繝ｪ繧ｽ繝ｼ繧ｹ逕滓・

/*
 * 目的（3Dモデルデータを読み込みキャッシュするため）
 * [入力] std::string pathName
 * [出力] int: モデルハンドル
 * [副作用] DxLibのモデル読み込み処理
 */
int ResourceManager::LoadModel(std::string pathName)
{
    // 譌｢縺ｫ隱ｭ縺ｿ霎ｼ縺ｾ繧後◆繝｢繝・Ν縺九←縺・°縺ｮ遒ｺ隱・
    for (int i = 0; i < resourceMapList.size(); i++)
    {
        if (resourceMapList.at(i).first == pathName)
        {
            // 髱槫酔譛溯ｪｭ縺ｿ霎ｼ縺ｿ荳ｭ縺ｯ隍・｣ｽ・・V1DuplicateModel・峨☆繧九→
            // DxLib蜀・Κ縺ｧ繧｢繧ｯ繧ｻ繧ｹ驕募渚縺瑚ｵｷ縺阪ｋ蝣ｴ蜷医′縺ゅｋ縺ｮ縺ｧ縲∵眠縺溘↓隱ｭ縺ｿ霎ｼ繧
            if (CheckHandleASyncLoad(resourceMapList.at(i).second) == TRUE)
            {
                return MV1LoadModel(pathName.c_str());
            }
            
            // 隱ｭ縺ｿ霎ｼ縺ｿ螳御ｺ・＠縺ｦ縺・ｋ縺ｪ繧峨Δ繝・Ν繝上Φ繝峨Ν繧定､・｣ｽ縺励※霑斐☆
            return MV1DuplicateModel(resourceMapList.at(i).second);
        }
    }

    // 隱ｭ縺ｿ霎ｼ縺ｾ繧後※縺・↑縺・ｴ蜷医・譁ｰ縺溘↓隱ｭ縺ｿ霎ｼ繧
    int handle = MV1LoadModel(pathName.c_str());
    if (handle == -1)
    {
        return -1;
    }

    // vector 縺ｫ霑ｽ蜉
    resourceMapList.push_back(std::pair<std::string, int>(pathName, handle));
    return MV1DuplicateModel(handle);   // 繧ｪ繝ｪ繧ｸ繝翫Ν縺ｮ繝上Φ繝峨Ν縺ｯ谿九＠縺ｦ縺翫″縺溘＞縺ｮ縺ｧ隍・｣ｽ縺励※霑斐＠縺ｦ縺翫￥
}

// 繧ｰ繝ｩ繝輔ぅ繝・け繝ｪ繧ｽ繝ｼ繧ｹ逕滓・

/*
 * 目的（2D画像データを読み込みキャッシュするため）
 * [入力] std::string pathName
 * [出力] int: 画像ハンドル
 * [副作用] DxLibの画像読み込み処理
 */
int ResourceManager::LoadGraphics(std::string pathName)
{
    for (int i = 0; i < graphicResourceMapList.size(); i++)
    {
        if (graphicResourceMapList.at(i).first == pathName)
        {
            return graphicResourceMapList.at(i).second;
        }
    }

    int handle = LoadGraph(pathName.c_str());
    if (handle == -1)
    {
        return -1;
    }

    graphicResourceMapList.push_back(std::pair<std::string, int>(pathName, handle));
    return handle;
}

// 蛻・牡縺輔ｌ縺溘げ繝ｩ繝輔ぅ繝・け繝ｪ繧ｽ繝ｼ繧ｹ逕滓・
DivGraphData* 
/*
 * 目的（分割画像データを読み込みキャッシュするため）
 * [入力] std::string pathName, int allNum, int numX, int numY
 * [出力] DivGraphData*
 * [副作用] DxLibの分割画像読み込み処理
 */
ResourceManager::LoadDivGraphics(std::string pathName, int allNum, int numX, int numY)
{
    for (int i = 0; i < divGraphicResourceMapList.size(); i++)
    {
        if (divGraphicResourceMapList.at(i)->filePath == pathName)
        {
            return divGraphicResourceMapList.at(i);
        }
    }

    // 荳譌ｦ繝・け繧ｹ繝√Ε繧定ｪｭ縺ｿ霎ｼ縺ｿ
    int handle = LoadGraph(pathName.c_str());
    if (handle == -1)
    {
        return nullptr;
    }

    // 閾ｪ菴懊＠縺溘け繝ｩ繧ｹ縺ｫ諠・ｱ繧呈ｼ邏・
    DivGraphData *data = new DivGraphData(
        pathName,
        numX, numY,
        allNum
    );

    // 荳譌ｦ隱ｭ縺ｿ霎ｼ繧薙□繝・け繧ｹ繝√Ε縺ｮ逕ｻ蜒上し繧､繧ｺ繧貞叙蠕・
    int sizeX, sizeY;
    GetGraphSize(handle, &sizeX, &sizeY);

    // 繝・け繧ｹ繝√Ε蛻・牡隱ｭ縺ｿ霎ｼ縺ｿ
    std::vector<int> test;
    handle = LoadDivGraph(pathName.c_str(), allNum, numX, numY, sizeX / numX, sizeY / numY, data->divHandleList);
    if (handle == -1)
    {
        return nullptr;
    }

    // 繝・・繧ｿ繧剃ｿ晏ｭ・
    divGraphicResourceMapList.push_back(data);

    return data;
}