#include "SkyBox.h"
#include "Model.h"

// コンストラクタ
SkyBox::SkyBox(std::string filename,VECTOR pos)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))
{
	// スカイボックスモデルの生成
	// note: スカイボックスの座標は、基本的には原点。
	//       ものによっては、座標を常にプレイヤーの座標にすることで、空が見切れないようにすることもある。
	model_ = new Model( filename, pos);
}

// デストラクタ
SkyBox::~SkyBox()
{
	// モデルクラスの破棄
	if (model_ != nullptr)
	{
		delete model_;
	}
}

// 更新処理
void SkyBox::Update()
{
	if (model_ != nullptr)
	{
		// スカイボックスがステージを突き抜けたり影を落とすのを防ぐため、常にカメラ位置に追従させる
		model_->SetPosition(GetCameraPosition());
		model_->Update();
	}
}

// 描画処理
void SkyBox::Draw()
{
	if (model_ != nullptr)
	{
		// Zバッファの書き込みをオフにする（奥の背景として描画し、ステージを隠さないようにする）
		SetWriteZBuffer3D(FALSE);
		
		// 一時的にライトの影響をOFFにして描画する
		// note: 影響すると影が出来て暗くなってしまう場合があるため。
		//       逆に影を付けたい場合はこの処理は外してよい。
		SetUseLighting(FALSE);
		
		model_->Draw();
		
		SetUseLighting(TRUE);
		SetWriteZBuffer3D(TRUE);
	}
}

// 拡大値（スケール値）の設定（Modelクラスへの橋渡し）
void SkyBox::SetScale(VECTOR scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

// モデルのテクスチャ変更（Modelクラスへの橋渡し）
void SkyBox::SetModelTexture(std::string filename, int index)
{
	if (model_ != nullptr)
	{
		model_->SetTexture(filename, index);
	}
}