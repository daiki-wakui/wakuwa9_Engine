#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "Sound.h"
#include "Sprite.h"
#include "SpriteBasis.h"

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPIの生成クラス
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyboradクラスの生成
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

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

	
	Sound* sound = nullptr;

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//keyborad初期化
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	spBasis->Initialize(dxBasis);
	int tex1 = 0;
	int tex2 = 0;
	int tex3 = 0;
	int tex4 = 0;

	tex1 = spBasis->TextureData(L"Resources/001.png");
	tex2 = spBasis->TextureData(L"Resources/test.png");
	tex3 = spBasis->TextureData(L"Resources/title.png");
	tex4 = spBasis->TextureData(L"Resources/kosi.png");

	spBasis->TextureSetting();

	SpBasis.reset(spBasis);


	//オーディオ初期化
	sound = new Sound();
	sound->Initialize();

	sound->LoadWave("PerituneMaterial.wav");
	sound->LoadWave("Alarm01.wav");

#pragma region  描画初期化処理
	HRESULT result;

	Sprite* sprite = new Sprite();
	sprite->Initialize(spBasis, winApp);

	Sprite* sprite2 = new Sprite();
	sprite2->Initialize(spBasis, winApp);

	Sprite* sprite3 = new Sprite();
	sprite3->Initialize(spBasis, winApp);

	Sprite* sprite4 = new Sprite();
	sprite4->Initialize(spBasis, winApp);

	sprite->Create(150, 150);
	sprite2->Create(50, 50);
	sprite3->Create(200, 200);
	sprite4->Create(300, 300);

	XMFLOAT2 posS = { 0,0 };
	XMFLOAT2 sizeS = { 0,0 };
	float speed = 1;
	float angle = 0;

#pragma endregion

	//OBJからモデルを読み込む
	Model* model = Model::LoadFromObj("sphere");
	Model* model3 = Model::LoadFromObj("floor");
	Model* model2 = Model::LoadFromObj("world");
	Model* model4 = Model::LoadFromObj("player");

	//3Dオブジェクト生成
	Object3D* object3d = Object3D::Create(5.0f);
	Object3D* object3d2 = Object3D::Create(2.0f);
	Object3D* object3d3 = Object3D::Create(100.0f);

	Object3D* objectFloor = Object3D::Create(5.0f);

	//3Dオブジェクトに3Dモデルを紐づけ
	object3d->SetModel(model);
	object3d2->SetModel(model4);
	object3d3->SetModel(model2);

	object3d->SetPosition({ -10,0,+5 });
	object3d2->SetPosition({ +10,0,+5 });

	objectFloor->SetModel(model3);
	objectFloor->SetPosition({ 0,-10,0 });

	int scene = 0;

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}

		sizeS = sprite->GetSize();

		posS.x = 0;
		posS.y = 0;

		angle = 0;

		angle += 1;

		if (input_->keyPush(DIK_D)) {
			//posS.x += speed;
			sizeS.x++;
		}
		else if (input_->keyPush(DIK_A)) {
			//posS.x -= speed;
			sizeS.x--;
		}
		if (input_->keyPush(DIK_W)) {
			//posS.y -= speed;
			sizeS.y++;
		}
		else if (input_->keyPush(DIK_S)) {
			//posS.y += speed;
			sizeS.y--;
		}

		//sprite
		//sprite->SetPosition(posS);
		sprite->SetSize(sizeS);
		//sprite->SetRotation(angle);
		//sprite2->SetRotation(angle);

		sprite2->Update();
		sprite->Update();
		sprite3->Update();
		sprite4->Update();

		object3d->Update();
		object3d2->Update();
		object3d3->Update();
		objectFloor->Update();

		if (input_->keyInstantPush(DIK_5) && scene == 0) {
			sound->PlayWave("PerituneMaterial.wav");
			scene = 1;
		}

		if (input_->keyInstantPush(DIK_6)) {
			sound->StopWAVE("PerituneMaterial.wav");
			scene = 0;
		}

		if (input_->keyInstantPush(DIK_7)) {
			sound->PlayWave("Alarm01.wav");
		}

		//keyborad更新処理
		input_->Update();

#pragma region DirectX毎フレーム処理
		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		object3d->Draw();
		object3d2->Draw();
		object3d3->Draw();
		objectFloor->Draw();

		Object3D::PostDraw();

		sprite->Draw(tex1);
		sprite2->Draw(tex2);
		sprite3->Draw(tex3);
		sprite4->Draw(tex4);
		

		//描画後処理
		dxBasis->PostDraw();


		
#pragma endregion

	}

	sound->Finalize();
	delete sound;

	delete sprite;
	delete sprite2;
	delete sprite3;
	delete sprite4;
	//ウィンドウクラスを登録解除
	winApp->Release();

	return 0;
}
