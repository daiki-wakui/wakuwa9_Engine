#include "GameCore.h"

void GameCore::Initialize()
{
	//windowsAPI初期化
	winApp->Initalize();
	windows.reset(winApp);

	//DirectX初期化
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	//keyborad初期化
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

	//Fbx初期化
	FbxLoader::GetInstance()->Initialize(dxBasis->GetDevice());

	FbxObject3d::SetCamera(Object3D::GetEye(), Object3D::GetTarget(), Object3D::GetUp());

	FbxObject3d::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//DirectionalLight::StaticInitalize(dxBasis->GetDevice());

	//OBJからモデルを読み込む
	floorModel = Model::LoadFromObj("floor");
	skydomModel = Model::LoadFromObj("world");
	playerModel = Model::LoadFromObj("player");

	//3Dオブジェクト生成
	playerObject = Object3D::Create(2.0f);
	skyObject = Object3D::Create(100.0f);
	objectFloor = Object3D::Create(5.0f);

	//3Dオブジェクトに3Dモデルを紐づけ
	playerObject->SetModel(playerModel);
	skyObject->SetModel(skydomModel);
	objectFloor->SetModel(floorModel);

	playerObject->SetPosition(XMFLOAT3({ 0,0,0 }));
	objectFloor->SetPosition({ 0,-10,0 });

	//FBXファイル読み込み
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

	//ライト生成
	lightGroup = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup);

	//オーディオ初期化
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
	//keyborad更新処理
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
	// 描画前処理
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

	//描画後処理
	dxBasis->PostDraw();
}
