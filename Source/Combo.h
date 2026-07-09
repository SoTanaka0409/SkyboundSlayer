#pragma once
#include"dxlib.h"
#include"string"
#include"Object2D.h"
#include"Texture.h"
#include"iostream"
#include"vector"
class Combo:public Object2D
{
public:
	Combo();

	~Combo();

	void Update();

	void Draw();

	void HitAddCombo();

private:
	int mnCombo;
	int handle;
	Texture* mpTexture0;
	Texture* mpTexture1;
	Texture* mpTexture2;
	Texture* mpTexture3;
	Texture* mpTexture4;
	Texture* mpTexture5;
	Texture* mpTexture6;
	Texture* mpTexture7;
	Texture* mpTexture8;
	Texture* mpTexture9;

};