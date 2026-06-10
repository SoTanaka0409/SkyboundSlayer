#pragma once
#include"Scene3D.h"
#include"Dxlib.h"
#include"Scene.h"
#include"Texture.h"
#include"ObjectManager.h"
#include"haikei.h"

class ResultScene :public Scene
{
public:
	/*enum ColorList
	{
		none = 0,
		Color1,
		Color2,
		Color3,
		Rast,

	};*/
public:
	ResultScene();

	~ResultScene();

	void Initialize();

	void Draw();

	void Update();

	void Finalize();

private:
	Texture* mpTexture;
	Texture* mpTexture2;
	Scene* mpScene;
	ObjectManager* mpObjectManager;
	int mnC;//クリア方法が選ばれたイルかどうか
	int mnS;//操作方法が選ばれているかどうか
	int mnE;//エンドが選ばれているかどうか 

	bool C;//クリア方法が選ばれたイルかどうか
	bool S;//操作方法が選ばれているかどうか
	bool E;//エンドが選ばれているかどうか

	bool C_Enter;//クリア方法が選ばれたイルかどうか(enter
	bool S_Enter;//操作方法が選ばれているかどうか(enter
	bool E_Enter;//エンドが選ばれているかどうか(enter

	int mnPause;
	int size = GetFontSize();

	std::string filename1;
	int mnHandle1;

	int AllTimer;
	
	int Color1;//カラー変更
	int ColorCount;
	bool Colorflag;
};