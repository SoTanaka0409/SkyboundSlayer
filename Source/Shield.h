#pragma once
#include"Dxlib.h"
#include<String>
#include"Object3D.h"
#include"Model.h"

class Shield :public Object3D
{
public:
	Shield(std::string filename,VECTOR initPos,int hp);

	~Shield();

	void Update();

	void Draw();

	int GetHp() { return mnHp; }
	void SetHp(int hp) { mnHp = hp; }

	float GetSize() { return mnSizeS; }

	bool GetNew() { return NewShield; }
	void SetNew(bool New) { NewShield = New; }
private:
	Model* mpModel;
	
	int mnHp;
	float mnSizeS;//シールドのサイズ
	bool NewShield;//新しく生成されているか

};