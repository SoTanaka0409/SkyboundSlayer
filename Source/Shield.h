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

   
	int GetHp() { return hp_; }
  
	void SetHp(int hp) { hp_ = hp; }

  
	float GetSize() { return size_s_; }

   
	bool GetNew() { return NewShield; }
  
	void SetNew(bool New) { NewShield = New; }
private:
	Model* model_;
	
	int hp_;
	float size_s_;//シールドのサイズ
	bool NewShield;//新しく生成されているか

};