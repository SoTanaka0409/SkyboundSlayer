#include"Camera.h"
#include"Config.h"
#include<cmath>
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Object3D.h"
#include"Scene.h"


Camera::Camera()

	:mfHorizontalAngle(0.0f)
	,mfVerticalAngle(0.0f)
	,position_(VGet(0.0f,0.0f,0.0f))
	,mvLookAtPosition(VGet(0.0f,0.0f,0.0f))
	,target_(nullptr)
	,Camera1(true)
	,Camera3(false)
	, mnShakeTime(0)
	, mnShakeTimeCount(0)
	, mfShakeAngle(0.0f)
	, mfShakeTimeCounter(0.0f)
	, mfShakeTime(0.0f)
	, mfShakeWidth(0.0f)
	, mfShakeAngleSpeed(0.0f)
	, mfStepTime(0.0f)
	, mvShakePosition(VGet(0.0f, 0.0f, 0.0f))
{
	Camera1 = true;
}

Camera::~Camera()
{
	
}

void Camera::Initialize()
{
	target_ = nullptr;
	//�J�����̃N���b�s���O�����̐ݒ�
	SetCameraNearFar(100.0f, Config::CameraFar);//10050000�܂ł̋�����������

	//�w�i�F��ݒ�i�D�F�j
	SetBackgroundColor(0, 0, 0);

	//�J�����̐ݒ�𔽉f
	SetCameraPositionAndTarget_UpVecY(position_, mvLookAtPosition);

	//�X�V�������x�s���Ă���
	Update();

	
}

void Camera::Update()
{
	/*if (mpTitleScene->GetResetCameraPlayer() == true)
	{
		target_ = nullptr;
		mpTitleScene->ResetCameraPlayer(false);
	}*/
	
	VECTOR temp; //��Ɨp�ϐ�
	if (target_ == nullptr)
	{
		target_ = Master::mpPlayer;

	}
	if(Camera1==true)
	{
		// ��New��
   // ��ʗh�ꏈ��
		Shake();
		Camera3 = false;
		const float distance = 300.0f;
		temp.x = 250.0f * cosf(mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f)) * sinf(mfHorizontalAngle / 180.0f * DX_PI_F);
		temp.y = 250.0f * sinf(mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f));
		temp.z = -(distance * cosf(mfVerticalAngle / 180.0f * DX_PI_F) * cosf(mfHorizontalAngle / 180.0f * DX_PI_F));
		dir = VGet(sinf(mfVerticalAngle), 0.0f, cosf(mfVerticalAngle));
		VECTOR dir2 = VGet(sinf(mfHorizontalAngle), 0.0f, cosf(mfHorizontalAngle));
		auto mpPlayer = Master::mpPlayer;
		Player3D* pPlayer = Master::mpPlayer;
		
		
		UpdateRotation();
		

		//�^�[�Q�b�g�����Ȃ�������
		
		if (target_ != nullptr)
		{
			mvLookAtPosition = VAdd(target_->GetPosition(), temp);
			position_ = VAdd(target_->GetPosition(), dir);

			position_.y += 160.0f + dir.x + dir.z;


		}
		else
		{
			//�����_�������ɂ��炷
			position_.y = 160.0f;
		}

		// ��New��
		// ��ʗh��̕�����Z����悤�ɕύX
		// �J�����ݒ�𔽉f
		SetCameraPositionAndTarget_UpVecY(VAdd(position_, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
		//��]�ݒ�
	}
	if (Camera3 == true)
	{
		Camera1 = false;
		
		
		//��ŋ��߂����W�ɒ����_�̍��W�𑫂�����̂��J�����̍��W�ƂȂ�
		
		
		
		auto mpPlayer = Master::mpPlayer;
		Player3D* pPlayer = Master::mpPlayer;
		
		UpdateRotation();
		//�^�[�Q�b�g�����Ȃ�������
		/*if (target_ == nullptr)
		{
			target_ = Master::mpPlayer;
		}*/
		if (target_ != nullptr)
		{
			mvLookAtPosition = target_->GetPosition();
			mvLookAtPosition.y += 240.0f;
			
		}
		else
		{
			//�����_�������ɂ��炷
			mvLookAtPosition.y = 160.0f;
		}
		// ��New��
   // ��ʗh�ꏈ��
		Shake();
		{
			VECTOR temp; //��Ɨp�ϐ�
			const float distance = 500.0f;
			temp.x = 400.0f * cosf(mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f)) * sinf(mfHorizontalAngle / 180.0f * DX_PI_F);
			temp.y = 400.0f * sinf(-mfVerticalAngle / 180.0f * (3.1415926535897932384626433832795f));
			temp.z = -(distance * cosf(mfVerticalAngle / 180.0f * DX_PI_F) * cosf(mfHorizontalAngle / 180.0f * DX_PI_F));
			position_ = VAdd(temp, mvLookAtPosition);
			/*mvLookAtPosition = VSub(target_->GetPosition(), position_);
			mvLookAtPosition = VNorm(mvLookAtPosition);*/
			// ��New��
		// ��ʗh��̕�����Z����悤�ɕύX
		 // �J�����ݒ�𔽉f
			SetCameraPositionAndTarget_UpVecY(VAdd(position_, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));

			//��ŋ��߂����W�ɒ����_�̍��W�𑫂�����̂��J�����̍��W�ƂȂ�
			//�J�����ݒ�𔽉f
		}
	}
	
}

void Camera::UpdateRotation()
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
		mfVerticalAngle -= deltaY * MOUSE_SENSITIVITY;
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
// ��New��
// ��ʗh��
void Camera::Shake()
{
	if (mfShakeTimeCounter < mfShakeTime)
	{
		// sinf �𗘗p���ėh�炵���W��Z�o
		// note: ��UY���W������h�炵�Ă݂�
		mvShakePosition.y = sinf(mfShakeAngle) * (1.0f - (mfShakeTimeCounter / mfShakeTime)) * mfShakeWidth;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		// �h�炵�����Ɏg�p���� sinf �ɓn���p�x�̕ύX����
		mfShakeAngle += mfShakeAngleSpeed * mfStepTime;

		// �h�炷���Ԃ�o�߂�����
		mfShakeTimeCounter += mfStepTime;
	}
	else
	{
		// �h�炳��Ă��Ȃ��ꍇ�͗h�炵�����ɂ����Z���W��O�ɂ���
		mvShakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

// ��New��
// ��ʗh��ݒ�
void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)
{
	mfShakeTimeCounter = 0.0f;
	mfShakeTime = time;
	mfShakeWidth = width;
	mfShakeAngleSpeed = angleSpeed;
	mfStepTime = stepTime;
}

void Camera::Finalize()
{



}
