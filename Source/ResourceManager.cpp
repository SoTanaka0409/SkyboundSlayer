#include "DxLib.h"
#include "ResourceManager.h"

// 入力: なし / 出力: なし
// 副作用: なし（インスタンス生成時の初期化処理）
ResourceManager::ResourceManager()
{
}

// 入力: なし / 出力: なし
// 副作用: キャッシュした全リソース（3Dモデル・単一画像・分割画像）のDxLibハンドルを完全破棄し、メモリリークを防ぐ
ResourceManager::~ResourceManager()
{
    for (int i = 0; i < (int)resourceMapList.size(); i++)
    {
        MV1DeleteModel(resourceMapList.at(i).second);
    }
    resourceMapList.clear();

    for (int i = 0; i < (int)graphicResourceMapList.size(); i++)
    {
        DeleteGraph(graphicResourceMapList.at(i).second);
    }
    graphicResourceMapList.clear();

    for (int i = 0; i < (int)divGraphicResourceMapList.size(); i++)
    {
        DivGraphData* data = divGraphicResourceMapList.at(i);
        if (data != nullptr)
        {
            // 動的確保された分割ハンドルの配列要素をすべて解放し、構造体自体も破棄する
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

// 入力: pathName (ファイルパス) / 出力: モデルハンドル(-1で失敗)
// 副作用: I/O負荷軽減のためのキャッシュ機構。ロード済みモデルは状態を独立させるため複製して返す
int ResourceManager::LoadModel(std::string pathName)
{
    for (int i = 0; i < resourceMapList.size(); i++)
    {
        if (resourceMapList.at(i).first == pathName)
        {
            // 非同期読み込み中のハンドルを複製(MV1DuplicateModel)するとDxLib内部でアクセス違反が起きるため、新規ロードで安全に回避する
            if (CheckHandleASyncLoad(resourceMapList.at(i).second) == TRUE)
            {
                return MV1LoadModel(pathName.c_str());
            }

            return MV1DuplicateModel(resourceMapList.at(i).second);
        }
    }

    int handle = MV1LoadModel(pathName.c_str());
    if (handle == -1)
    {
        return -1;
    }

    // 以降の呼び出しを高速化するため元ハンドルをキャッシュしつつ、呼び出し元には状態操作用に複製を渡す
    resourceMapList.push_back(std::pair<std::string, int>(pathName, handle));
    return MV1DuplicateModel(handle);
}

// 入力: pathName (ファイルパス) / 出力: 画像ハンドル(-1で失敗)
// 副作用: 同一画像の重複ロードによるVRAM圧迫を防ぐため、キャッシュ済みであれば既存ハンドルを使い回す
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

// 入力: pathName(パス), allNum(総数), numX(横分割数), numY(縦分割数) / 出力: 分割画像データ構造体のポインタ
// 副作用: 画像サイズから1コマの解像度を自動計算し、分割ロードした配列データをキャッシュに登録する
DivGraphData* ResourceManager::LoadDivGraphics(std::string pathName, int allNum, int numX, int numY)
{
    for (int i = 0; i < divGraphicResourceMapList.size(); i++)
    {
        if (divGraphicResourceMapList.at(i)->filePath == pathName)
        {
            return divGraphicResourceMapList.at(i);
        }
    }

    // 全体の解像度から1コマあたりの正確なピクセル幅・高さを算出するため、ダミーとして一度全体をロードする
    int handle = LoadGraph(pathName.c_str());
    if (handle == -1)
    {
        return nullptr;
    }

    DivGraphData* data = new DivGraphData(
        pathName,
        numX, numY,
        allNum
    );

    int sizeX, sizeY;
    GetGraphSize(handle, &sizeX, &sizeY);

    // 取得した動的サイズ情報を元にDxLibの分割ロードAPIを叩き、ハンドル配列を構造体に格納する
    handle = LoadDivGraph(pathName.c_str(), allNum, numX, numY, sizeX / numX, sizeY / numY, data->divHandleList);
    if (handle == -1)
    {
        return nullptr;
    }

    divGraphicResourceMapList.push_back(data);

    return data;
}