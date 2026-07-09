#pragma once
#include"Dxlib.h"
#include"Scene.h"
#include"InputManager.h"
#include"Texture.h"

class Rule :public Scene
{
public:
	Rule();

	~Rule();

	void Initialize();

	void Draw();

	void Update();

	void Finalize();

private:
	Texture* mpTexture;
	Scene* mpScene;
	Texture* mpTexture2;
	Texture* mpTexture3;
	int mnC;//クリア方法が選ばれたイルかどうか
	int mnS;//操作方法が選ばれているかどうか
	int mnE;//エンドが選ばれているかどうか

	bool C;//クリア方法が選ばれたイルかどうか
	bool S;//操作方法が選ばれているかどうか
	bool E;//エンドが選ばれているかどうか

	bool C_Enter;//クリア方法が選ばれたイルかどうか(enter)
	bool S_Enter;//操作方法が選ばれているかどうか(enter)
	bool E_Enter;//エンドが選ばれているかどうか(enter)

	int mnPause;//ポーズ画面かどうか
	int size = GetFontSize();

	std::string filename1;
	int mnHandle1;

	int Color1;//色の変更
	int Colorflag;

};