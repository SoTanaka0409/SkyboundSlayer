#pragma once
#include "Dxlib.h"
#include "Scene.h"

class ResultWin : public Scene
{
public:
	ResultWin();
	~ResultWin();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	void HandleReturnInput();
	void DrawResultPanel();
	void DrawResultHeader();
	void DrawResultStats();
	void DrawResultFooter();

	int mnAllTimer;
};
