#include"Stage.h"
#include"Master.h"

Stage::Stage(VECTOR initPos,std::string stageModelName, std::string stageCollisionModelName, VECTOR scale, std::string textureFilename)//蠎ｧ讓吶・蜴溽せ縺ｨ縺励※縺翫￥
	:Object3D(initPos)
{
	//繧ｿ繧ｰ險ｭ螳・
	SetTag(Object3D::Tag3D_Stage);
	//繧ｹ繝・・繧ｸ繝｢繝・Ν縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	mnModelHandle = MV1LoadModel(stageModelName.c_str());
	
	//繧ｳ繝ｪ繧ｸ繝ｧ繝ｳ繝｢繝・Ν(蠖薙◆繧雁愛螳夂畑縺ｮ繝｢繝・Ν)縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	mnCollisionHandle = MV1LoadModel(stageCollisionModelName.c_str());

	//繝・ヵ繧ｩ繝ｫ繝医・蠑墓焚縺梧ｸ｡縺輔ｌ縺溷ｴ蜷医・蠕捺擂縺ｮ繧ｹ繧ｱ繝ｼ繝ｫ繧剃ｽｿ逕ｨ
	if (scale.x == -1.0f && scale.y == -1.0f && scale.z == -1.0f) {
		float StageSize = 300.0f;
		MV1SetScale(mnModelHandle, VGet(StageSize, 50.0f, StageSize));
		MV1SetScale(mnCollisionHandle, VGet(StageSize, StageSize, StageSize));
	} else {
		MV1SetScale(mnModelHandle, scale);
		MV1SetScale(mnCollisionHandle, scale);
	}
	MV1SetPosition(mnCollisionHandle, initPos);
	MV1SetPosition(mnModelHandle, initPos);
	MV1SetupCollInfo(mnCollisionHandle);

	if (!textureFilename.empty()) {
		int texHandle = Master::mpResourceManager->LoadGraphics(textureFilename);
		if (texHandle != -1) {
			MV1SetTextureGraphHandle(mnModelHandle, 0, texHandle, FALSE);
		}
	}
	
}

Stage::~Stage()
{
	//隱ｭ縺ｿ霎ｼ繧薙□繝｢繝・Ν縺ｮ遐ｴ譽・
	MV1DeleteModel(mnModelHandle);
	MV1DeleteModel(mnCollisionHandle);
}

void Stage::Update()
{

}
void Stage::Draw()
{
	//繧ｹ繝・・繧ｸ繝｢繝・Ν縺ｮ謠冗判
	MV1DrawModel(mnModelHandle);

	//繧ｳ繝ｪ繧ｸ繝ｧ繝ｳ繝｢繝・Ν縺ｮ謠冗判(繝ｯ繧､繝､繝ｼ繝輔Ξ繝ｼ繝縺ｿ縺溘＞縺ｪ諢溘§縺ｧ謠冗判)
	// ///蠖薙◆繧雁愛螳夂畑縺ｮ繝｢繝・Ν縺ｨ縺励※菴懊ｉ繧後※縺・ｋ
	//隱ｭ縺ｿ霎ｼ繧繝｢繝・Ν縲∬牡縲・
}

bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	//逕滓・縺励※縺・◆蠖薙◆繧雁愛螳壹ｒ蝓ｺ縺ｫ繧ｫ繝励そ繝ｫ縺ｨ縺ｮ蠖薙◆繧雁愛螳壹ｒ陦後≧
	//繧ｳ繝ｪ繧ｸ繝ｧ繝ｳ邨先棡莉｣蜈･逕ｨ繝昴Μ繧ｴ繝ｳ驟榊・
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(mnCollisionHandle, -1, pos1, pos2, r);

	if (Master::mpDebug->Getdebug() == true)
	{
		//繝昴Μ繧ｴ繝ｳ縺ｫ荳縺､莉･荳雁ｽ薙◆縺｣縺ｦ縺・ｋ蝣ｴ蜷・
		if (result.HitNum >= 1)
		{
			//蝗樊焚繧貞ｽ薙◆縺｣縺溷屓謨ｰ繧貞屓縺・
			for (int i = 0; i < result.HitNum; i++)
			{
				//3D縺ｮ荳芽ｧ貞ｽ｢繧呈緒逕ｻ縺吶ｋ
				DrawTriangle3D(
					result.Dim[i].Position[0],
					result.Dim[i].Position[1],
					result.Dim[i].Position[2],
					GetColor(255, 0, 0),
					0
				);
			}
		}
	}
	//蠖薙◆繧雁愛螳壽ュ蝣ｱ縺ｮ蠕檎援縺･縺・
	MV1CollResultPolyDimTerminate(result);


	return(result.HitNum >= 1);
}

VECTOR Stage::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);

	//蠖薙◆繧雁愛螳壽ュ蝣ｱ縺ｨ邱壼・縺ｨ縺ｮ蠖薙◆繧雁愛螳壹ｒ陦後≧
	auto result = MV1CollCheck_Line(mnCollisionHandle, -1, pos1, pos2);

	//蠖薙◆縺｣縺ｦ縺・◆蝣ｴ蜷・
	if (result.HitFlag)//result.HitNum >= 1
	{
		//蠖薙◆縺｣縺溷区焚縺ｮ繝昴ず繧ｷ繝ｧ繝ｳ繧池eturn縺吶ｋ繧医≧縺ｫ蜿門ｾ励☆繧・
		//螢√・譎ゅ∩縺溘＞縺ｫHitPosition繧偵→繧句ｿ・ｦ√↑縺・
		ret = result.HitPosition;
	}

	return ret;
}

VECTOR Stage::CheckHit_LineDebug(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);

	//蠖薙◆繧雁愛螳壽ュ蝣ｱ縺ｨ邱壼・縺ｨ縺ｮ蠖薙◆繧雁愛螳壹ｒ陦後≧
	auto result = MV1CollCheck_Line(mnCollisionHandle, -1, pos1, pos2);

	//蠖薙◆縺｣縺ｦ縺・◆蝣ｴ蜷・
	if (result.HitFlag)//result.HitNum >= 1
	{
		//蠖薙◆縺｣縺溷区焚縺ｮ繝昴ず繧ｷ繝ｧ繝ｳ繧池eturn縺吶ｋ繧医≧縺ｫ蜿門ｾ励☆繧・
		//螢√・譎ゅ∩縺溘＞縺ｫHitPosition繧偵→繧句ｿ・ｦ√↑縺・
		ret = result.HitPosition;
		if (Master::mpDebug->Getdebug() == true)
		{
			DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit: x:%f, y:%f. z:%f", ret.x, ret.y, ret.z);
		}
	}
	else
	{
		if (Master::mpDebug->Getdebug() == true)
		{
			DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit None");

		}
	}

	return ret;
}