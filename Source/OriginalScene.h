#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include<string>



class OriginalScene :public Scene
{
public:
	OriginalScene();

	~OriginalScene();

	void Draw();

	void Update();

	void Initialize();

	void SetIsName(bool isname) { ifIsName = isname; }
	bool GetIsName() { return ifIsName; }

	void Finalize();

private:
	bool ifIsName;
	int AllTimer;

	std::string name;
	bool endInput = false;

};