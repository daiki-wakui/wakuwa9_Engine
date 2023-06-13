#include "GameCore.h"

void GameCore::Initialize()
{
	//windowsAPI������
	winApp->Initalize();
	windows.reset(winApp);

	//DirectX������
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	//keyborad������
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	imguiM->Initialize(winApp, dxBasis);
	ImGuiM.reset(imguiM);

	spBasis->Initialize(dxBasis);

	tex1 = spBasis->TextureData(L"Resources/001.png");
	tex2 = spBasis->TextureData(L"Resources/test.png");
	tex3 = spBasis->TextureData(L"Resources/title.png");

	spBasis->TextureSetting();
	SpBasis.reset(spBasis);

	sprite->Initialize(spBasis, winApp);
	sprite->Create(50, 50);

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//Fbx������
	FbxLoader::GetInstance()->Initialize(dxBasis->GetDevice());

	FbxObject3d::SetCamera(Object3D::GetEye(), Object3D::GetTarget(), Object3D::GetUp());

	FbxObject3d::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//DirectionalLight::StaticInitalize(dxBasis->GetDevice());

	//OBJ���烂�f����ǂݍ���
	floorModel = Model::LoadFromObj("floor");
	skydomModel = Model::LoadFromObj("world");
	playerModel = Model::LoadFromObj("player");

	//3D�I�u�W�F�N�g����
	playerObject = Object3D::Create(2.0f);
	skyObject = Object3D::Create(100.0f);
	objectFloor = Object3D::Create(5.0f);

	//3D�I�u�W�F�N�g��3D���f����R�Â�
	playerObject->SetModel(playerModel);
	skyObject->SetModel(skydomModel);
	objectFloor->SetModel(floorModel);

	playerObject->SetPosition(XMFLOAT3({ 0,0,0 }));
	objectFloor->SetPosition({ 0,-10,0 });

	//FBX�t�@�C���ǂݍ���
	cubeModel = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	objcube = new FbxObject3d;
	objcube->Initialize();
	objcube->SetModel(cubeModel);

	testModel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	testObj = new FbxObject3d;
	
	testObj->Initialize();
	testObj->SetModel(testModel);
	
	testObj->PlayAnimation();

	LightGroup::StaticInitialize(dxBasis->GetDevice());

	//���C�g����
	lightGroup = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup);

	//�I�[�f�B�I������
	sound = new Sound();
	sound->Initialize();

	sound->LoadWave("PerituneMaterial.wav");
	sound->LoadWave("Alarm01.wav");
}

void GameCore::Finalize()
{
	imguiM->Finalize();
	
	sound->Finalize();
	delete sound;
	delete lightGroup;

	delete playerModel;
	delete playerObject;
	delete floorModel;
	delete objectFloor;
	delete skydomModel;
	delete skyObject;

	delete cubeModel;
	delete objcube;

	delete testModel;
	delete testObj;

	FbxLoader::GetInstance()->Finalize();

	winApp->Release();
}

void GameCore::Update()
{
	//keyborad�X�V����
	input_->Update();

	lightGroup->Update();

	sprite->Update();

	playerObject->Update();
	skyObject->Update();
	objectFloor->Update();

	objcube->Update();
	testObj->Update();

	imguiM->Begin();

	imguiM->End();
}

void GameCore::Draw()
{
	// �`��O����
	dxBasis->PreDraw();

	Object3D::PreDraw(dxBasis->GetCommandList());
	FbxObject3d::PreSet(dxBasis->GetCommandList());

	//obj
	//playerObject->Draw();
	skyObject->Draw();
	objectFloor->Draw();

	//fbx
	//objcube->Draw();
	testObj->Draw();

	Object3D::PostDraw();

	sprite->Draw(tex1);

	//�`��㏈��
	dxBasis->PostDraw();
}
