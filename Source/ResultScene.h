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





	int mnAllTimer;
	
	int mnColorFade;//カラー変更
	int mnColorCount;
	bool mbColorFlag;
};