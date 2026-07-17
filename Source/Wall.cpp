#include"Wall.h"


Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	:Object3D(centerPos)
{
	//タグ設定
	SetTag(Object3D::Tag3D_Wall3D);


	//画像の読み込み
	graph_handle_ = LoadGraph(filename.c_str());

	//4頂点分のデータをセット

	//左上
	vertex_[0].pos = VAdd(centerPos, topLeft);
	vertex_[0].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
	vertex_[0].dif = GetColorU8(255, 255, 255, 255);
	vertex_[0].spc = GetColorU8(0, 0, 0, 0);
	vertex_[0].u = 0.0f;
	vertex_[0].v = 0.0f;
	vertex_[0].su = 0.0f;
	vertex_[0].sv = 0.0f;

	//右上
	vertex_[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	vertex_[1].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
	vertex_[1].dif = GetColorU8(255, 255, 255, 255);
	vertex_[1].spc = GetColorU8(0, 0, 0, 0);
	vertex_[1].u = 1.0f;
	vertex_[1].v = 0.0f;
	vertex_[1].su = 1.0f;
	vertex_[1].sv = 0.0f;

	//左下
	vertex_[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	vertex_[2].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
	vertex_[2].dif = GetColorU8(255, 255, 255, 255);
	vertex_[2].spc = GetColorU8(0, 0, 0, 0);
	vertex_[2].u = 0.0f;
	vertex_[2].v = 1.0f;
	vertex_[2].su = 0.0f;
	vertex_[2].sv = 1.0f;
	//右下
	vertex_[3].pos = VAdd(centerPos, bottomRight);
	vertex_[3].norm = VGet(1.0f, 0.0f, 0.0f);//後で計算する
	vertex_[3].dif = GetColorU8(255, 255, 255, 255);
	vertex_[3].spc = GetColorU8(0, 0, 0, 0);
	vertex_[3].u = 1.0f;
	vertex_[3].v = 1.0f;
	vertex_[3].su = 1.0f;
	vertex_[3].sv = 1.0f;





	//法線の設定
	VECTOR norm = VCross(VSub(vertex_[0].pos, vertex_[1].pos), VSub(vertex_[0].pos, vertex_[2].pos));
	norm = VNorm(norm);//正規化（ベクトルの大きさを１にする
	vertex_[0].norm = norm;
	vertex_[1].norm = norm;
	vertex_[2].norm = norm;
	vertex_[3].norm = norm;

}



Wall::~Wall()
{

	//画像の破棄
	DeleteGraph(graph_handle_);
}


//更新
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void Wall::Update()
{

}
//描画
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void Wall::Draw()
{
	WORD index[6];

	//2ポリゴン分のインデックスデータを設定
	//右辺は頂点データの配列番号
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	SetUseLighting(false);

	//2つの三角形ポリゴンのびょうが　
	DrawPolygonIndexed3D(vertex_, 4, index, 2, graph_handle_, true);

	SetUseLighting(true);
}