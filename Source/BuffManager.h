#pragma once
#include"vector"
#include"Buff.h"
#include"map"
class BuffManager
{
public:
	BuffManager();
	~BuffManager();
	void AddBuff(Buff* date);
	void Draw();
	void Update();
	float GetBuff(Object3D::StatusState state);
	void DeleteList();

	std::vector<Buff*>buffList;
private:
	
};