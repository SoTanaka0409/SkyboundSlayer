#include"Wall.h"


Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	:Object3D(centerPos)
{
	// タグ設定
	SetTag(Object3D::Tag3D_Wall3D);


	// 画像の読み込み
	m_GraphHandle = LoadGraph(filename.c_str());

	// 4頂点分のデータをセット

	// 左上
	m_Vertex[0].pos = VAdd(centerPos, topLeft);
	m_Vertex[0].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
	m_Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	m_Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	m_Vertex[0].u = 0.0f;
	m_Vertex[0].v = 0.0f;
	m_Vertex[0].su = 0.0f;
	m_Vertex[0].sv = 0.0f;

	// 右上
	m_Vertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	m_Vertex[1].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
	m_Vertex[1].dif = GetColorU8(255, 255, 255, 255);
	m_Vertex[1].spc = GetColorU8(0, 0, 0, 0);
	m_Vertex[1].u = 1.0f;
	m_Vertex[1].v = 0.0f;
	m_Vertex[1].su = 1.0f;
	m_Vertex[1].sv = 0.0f;

	// 左下
	m_Vertex[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	m_Vertex[2].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
	m_Vertex[2].dif = GetColorU8(255, 255, 255, 255);
	m_Vertex[2].spc = GetColorU8(0, 0, 0, 0);
	m_Vertex[2].u = 0.0f;
	m_Vertex[2].v = 1.0f;
	m_Vertex[2].su = 0.0f;
	m_Vertex[2].sv = 1.0f;
	// 右下
	m_Vertex[3].pos = VAdd(centerPos, bottomRight);
	m_Vertex[3].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
	m_Vertex[3].dif = GetColorU8(255, 255, 255, 255);
	m_Vertex[3].spc = GetColorU8(0, 0, 0, 0);
	m_Vertex[3].u = 1.0f;
	m_Vertex[3].v = 1.0f;
	m_Vertex[3].su = 1.0f;
	m_Vertex[3].sv = 1.0f;





	// 法線の設定
	VECTOR norm = VCross(VSub(m_Vertex[0].pos, m_Vertex[1].pos), VSub(m_Vertex[0].pos, m_Vertex[2].pos));
	norm = VNorm(norm);//正規化（ベクトルの大きさを１にする
	m_Vertex[0].norm = norm;
	m_Vertex[1].norm = norm;
	m_Vertex[2].norm = norm;
	m_Vertex[3].norm = norm;

}



Wall::~Wall()
{

	// 画像の破棄
	DeleteGraph(m_GraphHandle);
}


/// @brief 更新
void Wall::Update()
{

}
/// @brief 描画
void Wall::Draw()
{
	WORD index[6];

	// 2ポリゴン分のインデックスデータを設定
	// 右辺は頂点データの配列番号
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	SetUseLighting(false);

	// 2つの三角形ポリゴンのびょうが
	DrawPolygonIndexed3D(m_Vertex, 4, index, 2, m_GraphHandle, true);

	SetUseLighting(true);
}