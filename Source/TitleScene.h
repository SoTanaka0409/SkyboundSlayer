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
	float mCameraAngle;


	Texture* mpTexture;
	Scene* mpScene;
	
	



	bool mnResetCamera;

	int mnColorFade;//カラー変更
	int mnColorCount;
	bool mbColorFlag;
	
	
};