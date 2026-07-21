#include "DxLib.h"
#include "Config.h"
#include "Texture.h"
#include "Collision.h"
#include "SceneManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene3D.h"
#include "Player3D.h"
#include "Camera.h"
#include "ScoreManager.h"
#include "ResourceManager.h"
#include "Debug.h"
#include "DrawHp.h"
#include "EffekseerManager.h"
#include "EnemyManager.h"
#include "ColliderManager.h"
#include "EffectPool.h"
#include "ItemManager.h"
#include "InfClassManager.h"
#include "InfClass.h"
#include "BuffManager.h"
#include "Chat.h"
#include "Save.h"

// 繧ｵ繝ｼ繝薙せ繝ｭ繧ｱ繝ｼ繧ｿ繝ｼ縺ｨ縺励※縺ｮ繝槭せ繧ｿ繝ｼ繧ｯ繝ｩ繧ｹ縺ｮ髱咏噪螳滉ｽ灘喧
Player3D* Master::player_ = nullptr;
SceneManager* Master::scene_manager_ = new SceneManager();
SoundManager* Master::sound_manager_ = new SoundManager();
ResourceManager* Master::resource_manager_ = new ResourceManager();
Debug* Master::debug_ = new Debug();
DrawHp* Master::draw_hp_ = new DrawHp();
Camera* Master::camera_ = new Camera();
ScoreManager* Master::score_manager_ = new ScoreManager(0);
EnemyManager* Master::enemy_manager_ = new EnemyManager();
ItemManager* Master::item_manager_ = new ItemManager();
InfClassManager* Master::inf_class_manager_ = new InfClassManager();
BuffManager* Master::buff_manager_ = new BuffManager();
Chat* Master::chat_ = new Chat();
Save* Master::save_ = new Save();

bool Master::is_pause_on_ = false;
bool Master::is_stat_shop_on_ = false;
bool Master::is_safe_point_on_ = false;
bool Master::is_near_shop_on_ = false;
bool Master::is_save_ = false;
bool Master::is_cutscene_playing_ = false;
int Master::game_clear_count_ = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	SetDoubleStartValidFlag(TRUE);
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS);
	SetMainWindowText("Skybound Slayer");
	if (DxLib_Init() == -1) return -1;

	EffekseerManager::GetInstance()->Init();

	// 迺ｰ蠅・・險ｭ螳夲ｼ・D遨ｺ髢灘・菴薙ｒ蝮・ｸ縺ｫ辣ｧ繧峨＠縲∵囓驛ｨ縺ｧ縺ｮ繝｢繝・Ν隕冶ｪ肴ｧ繧堤｢ｺ菫昴☆繧九◆繧√・繝吶・繧ｹ繝ｩ繧､繝郁ｨｭ螳・
	SetLightEnable(TRUE);
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));

	// 髱槫酔譛溯ｪｭ縺ｿ霎ｼ縺ｿ・壼､ｧ驥上・繧｢繧ｻ繝・ヨ繧呈干縺医ｋ譛ｬ菴懊↓縺翫＞縺ｦ縲√Ο繝ｼ繝我ｸｭ縺ｮ繝輔Μ繝ｼ繧ｺ繧帝亟縺舌◆繧・撼蜷梧悄繝ｭ繝ｼ繝峨ｒ蠢・医→縺吶ｋ
	SetUseASyncLoadFlag(TRUE);
	Master::sound_manager_->Initialize();
	Master::scene_manager_->Initialize();
	Master::score_manager_->Initialize();
	Master::camera_->Initialize();

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);
	SetFontSize(20);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		int time = GetNowCount();

		// 繝ｭ繝ｼ繝・ぅ繝ｳ繧ｰ逶｣隕厄ｼ壹Ο繝ｼ繝牙ｮ御ｺ・∪縺ｧ繧ｲ繝ｼ繝騾ｲ陦鯉ｼ・pdate・峨ｒ繝ｭ繝・け縺励・・遘ｻ縺ｮ荳肴紛蜷医ｒ髦ｲ豁｢
		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}
		else
		{
			Master::draw_hp_->Update();
			Master::camera_->Update();
			Master::scene_manager_->Update();
			Master::inf_class_manager_->Update();
			EffekseerManager::GetInstance()->Update();
			EffectPool::GetInstance()->Update();
		}

		Master::scene_manager_->Draw();
		Master::score_manager_->Draw();
		EffekseerManager::GetInstance()->Draw();
		EffectPool::GetInstance()->Draw();

		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}

		ScreenFlip();

		// 繧｢繝ｼ繧ｭ繝・け繝√Ε險ｭ險茨ｼ夐ｫ倥Μ繝輔Ξ繝・す繝･繝ｬ繝ｼ繝育腸蠅・〒繧ら黄逅・ｼ皮ｮ励′蟠ｩ螢翫＠縺ｪ縺・ｈ縺・√ョ繝ｫ繧ｿ繧ｿ繧､繝縺ｫ鬆ｼ繧峨★蝗ｺ螳壹・17ms・育ｴ・0FPS・峨え繧ｧ繧､繝医〒蜷梧悄繧呈球菫・
		while (GetNowCount() - time < 17) {}

		// 繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ縺ｨ繝｡繝｢繝ｪ謗・勁・壹Γ繧､繝ｳ繝ｫ繝ｼ繝励・譛蠕後〒蜑企勁莠育ｴ・＆繧後◆繝ｪ繧ｽ繝ｼ繧ｹ繧偵∪縺ｨ繧√※隗｣謾ｾ縺吶ｋ縺薙→縺ｧ縲ゞpdate/Draw荳ｭ縺ｮ荳肴ｭ｣繧｢繧ｯ繧ｻ繧ｹ繧帝亟縺・
		if (GetASyncLoadNum() == 0)
		{
			Master::scene_manager_->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
			Master::scene_manager_->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
			ColliderManager::GetInstance()->DeleteAllColliderIfNeeded();
			Master::scene_manager_->ChangeSceneIfNeeded();
		}
	}

	// 邨ゆｺ・・逅・ｼ壹・繝阪・繧ｸ繝｣繝ｼ縺ｮFinalize縺ｨ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ蜑企勁
	Master::scene_manager_->Finalize();
	delete Master::scene_manager_;
	Master::sound_manager_->Finalize();
	delete Master::sound_manager_;
	Master::score_manager_->Finalize();
	delete Master::score_manager_;
	Master::camera_->Finalize();
	delete Master::camera_;
	delete Master::resource_manager_;

	ColliderManager::GetInstance()->Finalize();
	EffekseerManager::GetInstance()->End();
	DxLib_End();

	return 0;
}
