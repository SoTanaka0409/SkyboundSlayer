#pragma once
#include"Object3D.h"
#include<string>
#include"Debug.h"



class Stage :public Object3D
{
public:
	Stage(VECTOR initPos,std::string StageModelName, std::string stageCollsionModelName, VECTOR scale = VGet(-1.0f, -1.0f, -1.0f), std::string textureFilename = "");

	~Stage();

	void Update()override;

	void Draw()override;

	//繧ｹ繝・・繧ｸ縺ｨ繧ｫ繝励そ繝ｫ蝙九→縺ｮ蠖薙◆繧雁愛螳・
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	//繧ｹ繝・・繧ｸ縺ｨ邱壼・縺ｨ縺ｮ蠖薙◆繧雁愛螳・
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);
	VECTOR CheckHit_LineDebug(VECTOR pos1, VECTOR pos2);


private:
	Debug* mpDebug;
	int mnModelHandle; //繧ｹ繝・・繧ｸ繝｢繝・Ν縺ｮ繝上Φ繝峨Ν
	int mnCollisionHandle;;//繧ｹ繝・・繧ｸ縺ｮ蟄舌Μ繧ｸ繝ｧ繝ｳ繝｢繝・Ν縺ｮ繝上Φ繝峨Ν



};
