#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "LightGroup.h"
#include "Sprite.h"
#include "SpriteBasis.h"
#include "Sound.h"
#include "ImGuiManager.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "GameCore.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GameCore game;

	game.Initialize();

	//windowsAPIの生成クラス
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyboradクラスの生成
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//ImGuiクラスの生成
	std::unique_ptr<ImGuiManager> ImGuiM;
	ImGuiManager* imguiM = new ImGuiManager;

	//Spriteの基盤生成クラス
	std::unique_ptr<SpriteBasis> SpBasis;
	SpriteBasis* spBasis = new SpriteBasis();

	//windowsAPI初期化
	winApp->Initalize();
	windows.reset(winApp);
	MSG msg{};

	//DirectX初期化
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	imguiM->Initialize(winApp,dxBasis);
	ImGuiM.reset(imguiM);

	//Fbx初期化
	FbxLoader::GetInstance()->Initialize(dxBasis->GetDevice());

	Sound* sound = nullptr;

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	FbxObject3d::SetCamera(Object3D::GetEye(), Object3D::GetTarget(), Object3D::GetUp());

	FbxObject3d::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//DirectionalLight::StaticInitalize(dxBasis->GetDevice());
	LightGroup::StaticInitialize(dxBasis->GetDevice());

	//keyborad初期化
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	//DirectionalLight* light = nullptr;
	LightGroup* lightGroup = nullptr;

	//ライト生成
	lightGroup = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup);

	//平行光源
	float ambientColor0[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//点光源
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;

	//スポットライト
	float spotLightDir[3] = { 0,-1,0 };
	float spotLightPos[3] = { 0,5,0 };
	float spotLightColor[3] = { 1,1,1 };
	float spotLightAtten[3] = { 0.0f,0.0f,0.0f };
	float spotLightFactorAngle[2] = { 20.0f,30.0f};

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.0f,0.0f };
	float circleShadowFactorAngle[2] = { 1.0f,2.0f };

	float playerPos[3] = { 0,0.0f,5 };

	int State = 0;


	spBasis->Initialize(dxBasis);

	//ゲームシーンに使う初期化

	int tex1 = 0;
	int tex2 = 0;
	int tex3 = 0;

	tex1 = spBasis->TextureData(L"Resources/001.png");
	tex2 = spBasis->TextureData(L"Resources/test.png");
	tex3 = spBasis->TextureData(L"Resources/title.png");

	spBasis->TextureSetting();

	SpBasis.reset(spBasis);

	Sprite* sprite = new Sprite();
	sprite->Initialize(spBasis, winApp);

	sprite->Create(150, 150);

	

	//OBJからモデルを読み込む
	Model* floorModel = Model::LoadFromObj("floor");
	Model* skydomModel = Model::LoadFromObj("world");
	Model* playerModel = Model::LoadFromObj("player");

	//3Dオブジェクト生成
	Object3D* playerObject = Object3D::Create(2.0f);
	Object3D* skyObject = Object3D::Create(100.0f);
	Object3D* objectFloor = Object3D::Create(5.0f);

	//3Dオブジェクトに3Dモデルを紐づけ
	playerObject->SetModel(playerModel);
	skyObject->SetModel(skydomModel);

	playerObject->SetPosition(XMFLOAT3(playerPos));

	objectFloor->SetModel(floorModel);
	objectFloor->SetPosition({ 0,-10,0 });

	//オーディオ初期化
	sound = new Sound();
	sound->Initialize();

	sound->LoadWave("PerituneMaterial.wav");
	sound->LoadWave("Alarm01.wav");

	OutputDebugStringA("文字列リテラルを出力するよ\n");

	std::string a("stringに埋め込んだ文字列を取得するよ\n");
	OutputDebugStringA(a.c_str());

	//FBXファイル読み込み
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");

	FbxModel* cube = nullptr;
	FbxObject3d* objcube = nullptr;

	cube = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	objcube = new FbxObject3d;
	objcube->Initialize();
	objcube->SetModel(cube);
	//ですとらくたno

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}

		game.Update();

		sprite->Update();

		playerObject->Update();
		skyObject->Update();
		objectFloor->Update();

		objcube->Update();

		//keyborad更新処理
		input_->Update();

		lightGroup->Update();


		//デバック用
		imguiM->Begin();

		imguiM->End();

#pragma region DirectX毎フレーム処理

		game.Draw();

		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		playerObject->Draw();
		skyObject->Draw();
		objectFloor->Draw();

		objcube->Draw(dxBasis->GetCommandList());

		Object3D::PostDraw();

		sprite->Draw(tex1);

		//描画後処理
		dxBasis->PostDraw();

		imguiM->Draw();

#pragma endregion

	}

	game.Finalize();

	sound->Finalize();
	delete sound;
	delete sprite;
	delete lightGroup;
	delete floorModel;
	delete playerModel;
	delete playerObject;
	delete skyObject;
	delete objectFloor;

	FbxLoader::GetInstance()->Finalize();

	imguiM->Finalize();
	//ウィンドウクラスを登録解除
	winApp->Release();

	return 0;
}
