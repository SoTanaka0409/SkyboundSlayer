#include"haikei.h"
#include"Object2D.h"
#include"ObjectManager.h"
#include"Master.h"
#include"Scene.h"
#include"InputManager.h"

haikei::haikei(VECTOR initPos, std::string filename)
	:Object2D(initPos)

{
	mpTexture = new Texture("", initPos, true);
}
haikei::~haikei()
{
}

void haikei::Update()
{
	Object2D::Update();
	mpTexture->Update();
}

void haikei::Draw()
{
	mpTexture->Draw();
	Object2D::Draw();
}

