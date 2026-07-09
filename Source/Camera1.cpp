#include"Camera1.h"
#include"Config.h"
#include<cmath>
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Object3D.h"
#include"SceneManager.h"
#include"Player3D.h"
#include"Scene.h"

Camera1::Camera1()
	:mfHorizontalAngle(0.0f)
	, mfVerticalAngle(0.0f)
	, position_(VGet(0.0f, 0.0f, 0.0f))
	, mvLookAtPosition(VGet(0.0f, 0.0f, 0.0f))
	, target_(nullptr)
{


}

Camera1::~Camera1()
{

}

void Camera1::Initialize()
{
	//�J�����̃N���b�s���O�����̐ݒ�
	SetCameraNearFar(100.0f, Config::CameraFar);//10050000�܂ł̋�����������

	//�w�i�F��ݒ�i�D�F�j
	SetBackgroundColor(0, 0, 0);

	//�J�����̐ݒ�𔽉f
	SetCameraPositionAndTarget_UpVecY(position_, mvLookAtPosition);

	//�X�V�������x�s���Ă���
	Update();

}

void Camera1::Update()
{
	auto mpPlayer = Master::mpPlayer;
	Player3D* pPlayer = Master::mpPlayer;
	UpdateRotation();
	//�^�[�Q�b�g�����Ȃ�������
	if (target_ == nullptr)
	{
		target_ = Master::mpPlayer;
	}
	if (target_ != nullptr)
	{
		mvLookAtPosition = target_->GetPosition();
		mvLookAtPosition.y += 80.0f;
	}
	else
	{
		//�����_�������ɂ��炷
		mvLookAtPosition.y = 80.0f;
	}

	{
		VECTOR temp; //��Ɨp�ϐ�


		mvLookAtPosition = VSub(pPlayer->GetPosition() , position_);
		mvLookAtPosition = VNorm(mvLookAtPosition);

		//��ŋ��߂����W�ɒ����_�̍��W�𑫂�����̂��J�����̍��W�ƂȂ�
		//�J�����ݒ�𔽉f
		SetCameraPositionAndTarget_UpVecY(pPlayer->GetPosition(), mvLookAtPosition);
	}

}

void Camera1::UpdateRotation()
{
	////�����L�[�ŃJ��������
	if (mfHorizontalAngle >= 180.0f)
	{
		mfHorizontalAngle -= 360.0f;
	}
	if (mfHorizontalAngle <= -180.0f)
	{
		mfHorizontalAngle += 360.0f;
	}

	if (mfVerticalAngle >= 80.0f)
	{
		mfVerticalAngle = 80.0f;
	}

	if (mfVerticalAngle <= -80.0f)
	{
		mfVerticalAngle = -80.0f;
	}


	float camAngleY = 0.0f; // ���������i���E�j
	float camAngleX = 0.0f; // ���������i�㉺�j

	// ���x
	const float MOUSE_SENSITIVITY = 0.05f;

	// �J�����̋���
	float camDistance = 300.0f;



	// �J�[�\�����\����

		// �}�E�X�̈ړ��ʂ�擾
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	// ���S���W
	if (!CheckHitKey(KEY_INPUT_0))
	{
		int centerX = 640;
		int centerY = 360;
	}
	auto sceneType = Master::mpSceneManager->GetCurrentSceneType();
	if (sceneType == SceneManager::SCENE_GAME || sceneType == SceneManager::SCENE_TUTORIAL || sceneType == SceneManager::SCENE_3D || sceneType == SceneManager::SCENE_3DHARD || sceneType == SceneManager::SCENE_LEVEL || sceneType == SceneManager::SCENE_TEST_COLLISION) {
		SetMousePoint(centerX, centerY);



	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;

	// ��]�p�x��X�V
	mfHorizontalAngle -= deltaX * MOUSE_SENSITIVITY;
	mfVerticalAngle += deltaY * MOUSE_SENSITIVITY;
	}

	// �㉺�̉�]�𐧌��i���������Ԃ�Ȃ��悤�Ɂj
	/*if (camAngleX < -DX_PI_F / 2.0f) camAngleX = -DX_PI_F / 2.0f;
	if (camAngleX > DX_PI_F / 2.0f) camAngleX = DX_PI_F / 2.0f;*/

	//// �J�����̈ʒu�ƒ����_��v�Z
	//VECTOR temp = VGet(
	//	camTarget.x + camDistance * sinf(camAngleY) * cosf(camAngleX),
	//	camTarget.y + camDistance * sinf(camAngleX),
	//	camTarget.z + camDistance * cosf(camAngleY) * cosf(camAngleX)


}
