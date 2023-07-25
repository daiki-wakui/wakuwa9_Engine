#include "Framework.h"

void Framework::Initialize()
{
	//windowsAPI������
	windows_->Initalize();

	//DirectX������
	directX_->Initialize(windows_.get());

	//keyborad������
	keyboard_->Initialize(windows_->GetHInstancee(), windows_->GetHwnd());

	gamePad_->Update();

	imGuiM_->Initialize(windows_.get(), directX_.get());

	spBasis_->Initialize(directX_.get());

	Object3D::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());

	ParticleManager::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());


	//Fbx������
	FbxLoader::GetInstance()->Initialize(directX_->GetDevice());
	
	//�J�����Z�b�g
	XMFLOAT3 eye = Object3D::GetEye();
	XMFLOAT3 target = Object3D::GetTarget();
	XMFLOAT3 up = Object3D::GetUp();
	FbxObject3d::SetCamera(eye, target, up);

	FbxObject3d::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());

	//���C�g�O���[�v������
	LightGroup::StaticInitialize(directX_->GetDevice());

	//���C�g����
	lightGroup->Initialize();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());

	//�I�[�f�B�I������
	sound_->Initialize();

	sceneManager_ = std::make_unique<SceneManager>();
}

void Framework::Finalize()
{

	imGuiM_->Finalize();

	sound_->Finalize();
	FbxLoader::GetInstance()->Finalize();

	windows_->Release();
}

void Framework::Update()
{
	IsEnd();

	//keyborad�X�V����
	keyboard_->Update();

	gamePad_->Update();

	lightGroup->Update();

	//sceneManager_->Update();
}

void Framework::IsEnd(){

	//�~�{�^���ŏI�����b�Z�[�W��������
	if (GetWindows()->gameloopExit(msg) == true || GetInput()->keyInstantPush(DIK_ESCAPE) == true) {
		isEndGame_ = true;	//�Q�[�����[�v�I��
	}
}

void Framework::Run()
{
	//�Q�[���̏�����
	Initialize();


	while (true)
	{
		IsEnd();

		//���t���[���X�V
		Update();

		if (GetIsEnd()) {
			break;
		}

		//�`��
		Draw();
	}


	//�Q�[���̏I��
	Finalize();
}
