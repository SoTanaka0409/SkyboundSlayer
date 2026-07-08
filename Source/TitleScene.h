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


	//////result‚É‚Á‚Ä‚¢‚­ˆ×//////
	char String[256];
	int InputHandle;
	int Name;
private:
	int mnSkyBoxHandle;
	int mnStageHandle;
	int mnCastleHandle;
	float mCameraAngle;

	Texture* mpTexture;
	Scene* mpScene;
	
	



	bool mnResetCamera;

	int mnColorFade;//ƒJƒ‰[•ÏX
	int mnColorCount;
	bool mbColorFlag;
	
	
};