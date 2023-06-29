#include "GameCore.h"

void GameCore::Initialize()
{
	//windowsAPI������
	windows_->Initalize();

	//DirectX������
	directX_->Initialize(windows_.get());

	//keyborad������
	keyboard_->Initialize(windows_->GetHInstancee(), windows_->GetHwnd());

	imGuiM_->Initialize(windows_.get(), directX_.get());

	spBasis_->Initialize(directX_.get());

	postTex = spBasis_->TextureData(L"Resources/white1x1.png");

	tex1_ = spBasis_->TextureData(L"Resources/001.png");
	tex2_ = spBasis_->TextureData(L"Resources/test.png");
	tex3_ = spBasis_->TextureData(L"Resources/title.png");

	backTex_ = spBasis_->TextureData(L"Resources/backTitle.png");

	spBasis_->TextureSetting();

	sprite_->Initialize(spBasis_.get(), windows_.get());
	sprite_->Create(50, 50);

	backSprite_->Initialize(spBasis_.get(), windows_.get());
	backSprite_->Create(640, 360);
	backSprite_->SetSize({ 1280,720 });
	backSprite_->Update();

	postEffect_->SetDirectX(spBasis_.get(), windows_.get(),keyboard_.get());
	postEffect_->Initialize(0);

	gaussianEffect_->SetDirectX(spBasis_.get(), windows_.get(), keyboard_.get());
	gaussianEffect_->Initialize(1);

	Object3D::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());

	//Fbx������
	FbxLoader::GetInstance()->Initialize(directX_->GetDevice());

	XMFLOAT3 eye = Object3D::GetEye();
	XMFLOAT3 target = Object3D::GetTarget();
	XMFLOAT3 up = Object3D::GetUp();

	FbxObject3d::SetCamera(eye, target, up);

	FbxObject3d::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());

	//DirectionalLight::StaticInitalize(dxBasis->GetDevice());

	//OBJ���烂�f����ǂݍ���
	playerModel_ = std::make_unique<Model>();
	playerModel_->LoadFromObj("player");

	floorModel_ = std::make_unique<Model>();
	floorModel_->LoadFromObj("floor");

	skydomModel_ = std::make_unique<Model>();
	skydomModel_->LoadFromObj("world");

	//3D�I�u�W�F�N�g����
	playerObject_ = std::make_unique<Object3D>();
	//playerObject->SetModel(playerModel2);
	playerObject_->SetModel(playerModel_.get());
	playerObject_->Initialize();
	playerObject_->SetScale(XMFLOAT3({ 2,2,2 }));
	playerObject_->SetPosition(XMFLOAT3({ 20,0,5 }));

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(XMFLOAT3({ 100,100,100 }));

	objectFloor_ = std::make_unique<Object3D>();
	objectFloor_->SetModel(floorModel_.get());
	objectFloor_->Initialize();
	objectFloor_->SetScale(XMFLOAT3({ 5,5,5 }));
	objectFloor_->SetPosition({ 0,-10,0 });

	//FBX�t�@�C���ǂݍ���

	testModel_ = std::make_unique<FbxModel>();
	FbxLoader::GetInstance()->LoadModelFromFile(testModel_.get(), "boneTest");

	testObj_ = std::make_unique<FbxObject3d>();
	testObj_->Initialize();
	testObj_->SetModel(testModel_.get());
	
	testObj_->PlayAnimation();

	LightGroup::StaticInitialize(directX_->GetDevice());

	//���C�g����
	lightGroup->Initialize();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());

	//�I�[�f�B�I������
	sound_->Initialize();

	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("Alarm01.wav");
}

void GameCore::Finalize()
{
	imGuiM_->Finalize();
	
	sound_->Finalize();

	testModel_->fbxScene_->Destroy();

	FbxLoader::GetInstance()->Finalize();

	windows_->Release();
}

void GameCore::Update()
{
	//keyborad�X�V����
	keyboard_->Update();

	lightGroup->Update();

	sprite_->Update();

	playerObject_->Update();
	skyObject_->Update();
	objectFloor_->Update();

	//testObj_->Update();

	//sound_->PlayWave("Alarm01.wav");

	imGuiM_->Begin();

	imGuiM_->End();
}

void GameCore::Draw()
{

	postEffect_->PreDrawScene(directX_->GetCommandList());
	
	//backSprite_->Draw(backTex_);

	Object3D::PreDraw(directX_->GetCommandList());
	FbxObject3d::PreSet(directX_->GetCommandList());

	//obj
	//playerObject_->Draw();
	skyObject_->Draw();
	//objectFloor_->Draw();

	//fbx
	//testObj_->Draw();

	Object3D::PostDraw();

	sprite_->Draw(tex1_);

	postEffect_->PostDrawScene(directX_->GetCommandList());

	// �`��O����
	directX_->PreDraw();

	postEffect_->Draw();

	//�`��㏈��
	directX_->PostDraw();
}
