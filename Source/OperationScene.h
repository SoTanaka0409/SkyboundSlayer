#pragma once
#include"Dxlib.h"
#include"Master.h"
#include"InputManager.h"
#include"Scene.h"
#include"Texture.h"


class OperationScene:public Scene
{
public:
	OperationScene();

	~OperationScene();

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

private:
	Texture* mpTexture;
	Texture* mpTexture2;
	Texture* mpTexture3;
	Scene* mpScene;
	///////選択///////////
	int mnC;//クリア方法が選ばれたイルかどうか
	int mnS;//操作方法が選ばれているかどうか
	int mnE;//エンドが選ばれているかどうか

	bool C;//クリア方法が選ばれたイルかどうか
	bool S;//操作方法が選ばれているかどうか
	bool E;//エンドが選ばれているかどうか

	bool C_Enter; //(enter)
	bool S_Enter; //(enter)
	bool E_Enter; //(enter)

	
	/// //////////////////////////////
	int mnPause;//ポーズ画面かどうか
	int size = GetFontSize();

	int Color1;//カラー変更
	int Colorflag;

	int color = GetColor(255,255,255);
	bool FirstPage;
	bool NextPage;

};

