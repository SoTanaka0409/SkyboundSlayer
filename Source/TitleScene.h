#pragma once
#include"Dxlib.h"
#include"Scene.h"
#include"Texture.h"
#include"Camera.h"

class TitleScene :public Scene
{
public:
	TitleScene();

	~TitleScene();

	void Initialize();

	void Draw();

	void Update();

	void Finalize();

	void ResetCameraPlayer(bool resetCamera) { mnResetCamera = resetCamera; }
	bool GetResetCameraPlayer() { return mnResetCamera; }


	//////resultに持っていく為//////
	char String[256];
	int InputHandle;
	int Name;
private:
	Texture* mpTexture;
	Scene* mpScene;
	
	
	int mnC;//クリア方法が選ばれたイルかどうか
	int mnS;//操作方法が選ばれているかどうか
	int mnE;//エンドが選ばれているかどうか

	bool C;//クリア方法が選ばれたイルかどうか
	bool S;//操作方法が選ばれているかどうか
	bool E;//エンドが選ばれているかどうか
	bool T;

	bool C_Enter;//クリア方法が選ばれたイルかどうか
	bool S_Enter;//操作方法が選ばれているかどうか
	bool E_Enter;//エンドが選ばれているかどうか

	int mnPause;
	int size = GetFontSize();
	bool mnResetCamera;

	int Color1;//カラー変更
	int ColorCount;
	bool Colorflag;
	
	
};