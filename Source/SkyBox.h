#pragma once
class Model;

#include "Object3D.h"

class SkyBox : public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	SkyBox(std::string filename,VECTOR pos);		// コンストラクタ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~SkyBox();							// デストラクタ

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update() override;	// 更新処理
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw() override;	// 描画処理

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetScale(VECTOR scale);									// 拡大値（スケール値）の設定（Modelクラスへの橋渡し）
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetModelTexture(std::string filename, int index = 0);	// モデルのテクスチャ変更（Modelクラスへの橋渡し）

private:
	Model* model_;			// モデルクラスのポインタ
};

/*
*
* ★使用方法
* 1. スカイボックスを生成したい Scene の Initialize 内で、SkyBox クラスを new する。
* 2. 必要であれば SkyBox の大きさを調整する。
*	2-a. 1の手順でnewする際に、ローカル変数にnewしたものを取っておく
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
*		例）SkyBox* pSkyBox = new SkyBox(ファイル名);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
*			pSkyBox->SetScale(5.0f);
* 3. スカイボックスモデルに使用されているテクスチャを変更したい場合は、SetModelTexture を利用する。
*	3-a. 1の手順でnewする際に、ローカル変数にnewしたものを取っておく
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
*		例）SkyBox* pSkyBox = new SkyBox(ファイル名);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
*			pSkyBox->SetModelTexture(変更したい画像へのパス);
* ※2 と 3 は併用可能です。
*
* 基本的にはこれだけですが、使用したい場合は Model クラス側の修正も必要なので注意すること。
* →「（Modelクラスへの橋渡し）」というコメントのある関数を Model クラス側に用意する必要がある。
*
*/