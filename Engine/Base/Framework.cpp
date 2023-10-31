#include "Framework.h"

using namespace DirectX;

void Framework::Initialize()
{
	//windowsAPI初期化
	windows_->Initalize();

	//DirectX初期化
	directX_->Initialize();

	//keyborad初期化
	keyboard_->Initialize(windows_->GetHInstancee(), windows_->GetHwnd());

	gamePad_->Update();

	imGuiM_->Initialize();

	spBasis_->Initialize();

	Object3D::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());

	

	//Fbx初期化
	FbxLoader::GetInstance()->Initialize(directX_->GetDevice());
	
	//カメラセット
	XMFLOAT3 eye = Object3D::GetEye();
	XMFLOAT3 target = Object3D::GetTarget();
	XMFLOAT3 up = Object3D::GetUp();
	FbxObject3d::SetCamera(eye, target, up);

	FbxObject3d::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());

	ParticleManager::SetCamera(eye, target, up);
	ParticleManager::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());


	//ライトグループ初期化
	LightGroup::StaticInitialize(directX_->GetDevice());

	//ライト生成
	lightGroup->Initialize();
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());

	//オーディオ初期化
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

	//keyborad更新処理
	keyboard_->Update();

	gamePad_->Update();

	lightGroup->Update();

	//sceneManager_->Update();
}

void Framework::IsEnd(){

	//×ボタンで終了メッセージがきたら
	if (windows_->gameloopExit(msg) == true || keyboard_->keyInstantPush(DIK_ESCAPE) == true) {
		isEndGame_ = true;	//ゲームループ終了
	}
}

void Framework::Run()
{
	//ゲームの初期化
	Initialize();


	while (true)
	{
		IsEnd();

		//毎フレーム更新
		Update();

		if (GetIsEnd()) {
			break;
		}

		//描画
		Draw();
	}


	//ゲームの終了
	Finalize();
}
