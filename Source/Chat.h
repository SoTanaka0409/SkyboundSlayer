#pragma once
#include"dxlib.h"
#include"string"
#include"Texture.h"

class Chat
{
public:
	Chat();
	~Chat();
	
	void Draw(std::string chatdate);
	void Reset();
private: 
	Texture* mpTexture;
	int mnFirstFont;
	int fontsize;
};