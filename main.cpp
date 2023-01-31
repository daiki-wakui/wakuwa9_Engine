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

	float fightPos[3] = { 10,0.0f,5 };

	int State = 0;


	spBasis->Initialize(dxBasis);
	int tex1 = 0;
	int tex2 = 0;
	int tex3 = 0;

	tex1 = spBasis->TextureData(L"Resources/001.png");
	tex2 = spBasis->TextureData(L"Resources/test.png");
	tex3 = spBasis->TextureData(L"Resources/title.png");

	spBasis->TextureSetting();

	SpBasis.reset(spBasis);

	HRESULT result;

	Sprite* sprite = new Sprite();
	sprite->Initialize(spBasis, winApp);

	Sprite* sprite2 = new Sprite();
	sprite2->Initialize(spBasis, winApp);

	Sprite* sprite3 = new Sprite();
	sprite3->Initialize(spBasis, winApp);

	sprite->Create(150, 150);
	sprite2->Create(50, 50);
	sprite3->Create(900, 120);

	XMFLOAT2 posS = { 0,0 };
	XMFLOAT2 sizeS = { 0,0 };
	float speed = 1;
	float angle = 0;

	//OBJからモデルを読み込む
	Model* model = Model::LoadFromObj("sphere",true);
	Model* model3 = Model::LoadFromObj("floor");
	Model* model2 = Model::LoadFromObj("world");
	Model* model4 = Model::LoadFromObj("player");
	Model* model5 = Model::LoadFromObj("floor2");

	//3Dオブジェクト生成
	Object3D* object3d = Object3D::Create(5.0f);
	Object3D* object3d2 = Object3D::Create(2.0f);
	Object3D* object3d3 = Object3D::Create(100.0f);

	Object3D* objectFloor = Object3D::Create(5.0f);

	Object3D* objectFloor2 = Object3D::Create(5.0f);

	//3Dオブジェクトに3Dモデルを紐づけ
	object3d->SetModel(model);
	object3d2->SetModel(model4);
	object3d3->SetModel(model2);

	object3d->SetPosition({ -10,0,+5 });
	object3d2->SetPosition(XMFLOAT3(fightPos));

	objectFloor->SetModel(model3);
	objectFloor->SetPosition({ 0,-10,0 });

	objectFloor2->SetModel(model5);
	objectFloor2->SetPosition({ 0,-10,0 });

	//オーディオ初期化
	sound = new Sound();
	sound->Initialize();

	sound->LoadWave("PerituneMaterial.wav");
	sound->LoadWave("Alarm01.wav");

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
		sprite3->SetSize(XMFLOAT2{ 1280 / 2,720 / 2 });

		sprite2->Update();
		sprite->Update();
		sprite3->Update();

		object3d->Update();
		object3d2->Update();
		object3d3->Update();
		objectFloor->Update();
		objectFloor2->Update();

		if (input_->keyInstantPush(DIK_5) && scene == 0) {
			sound->PlayWave("PerituneMaterial.wav");
			scene = 1;
		}

		if (input_->keyInstantPush(DIK_6)) {
			if (scene == 1) {
				sound->StopWAVE("PerituneMaterial.wav");
			}

			scene = 0;
		}

		if (input_->keyInstantPush(DIK_7)) {
			sound->PlayWave("Alarm01.wav");
		}

		//keyborad更新処理
		input_->Update();

		//light->Update();
		lightGroup->Update();

		//光線方向初期化				  上 奥
		static XMVECTOR lightDir = { 0,1,5,0 };

		if (input_->keyPush(DIK_RIGHT)) {
			fightPos[0]++;
		}
		else if (input_->keyPush(DIK_LEFT)) {
			fightPos[0]--;
		}
		if (input_->keyPush(DIK_UP)) {
			fightPos[1]++;
		}
		else if (input_->keyPush(DIK_DOWN)) {
			fightPos[1]--;
		}

		if (input_->keyInstantPush(DIK_1)) {
			State = 1;
		}
		if (input_->keyInstantPush(DIK_2)) {
			State = 2;
		}
		if (input_->keyInstantPush(DIK_3)) {
			State = 3;
		}
		if (input_->keyInstantPush(DIK_4)) {
			State = 4;
		}

		if (State == 1) {
			lightGroup->SetDirLightActive(0, true);
			lightGroup->SetDirLightActive(1, true);
			lightGroup->SetDirLightActive(2, true);

			lightGroup->SetSpotLightActive(0, false);
			lightGroup->SetPointLightActive(0, false);

			lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));

			lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0],lightDir0[1],lightDir0[2],0 }));
			lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));

			lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0],lightDir1[1],lightDir1[2],0 }));
			lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));

			lightGroup->SetDirLightDir(2, XMVECTOR({ lightDir2[0],lightDir2[1],lightDir2[2],0 }));
			lightGroup->SetDirLightColor(2, XMFLOAT3(lightColor2));
		}

		if (State == 2) {
			lightGroup->SetDirLightActive(0, false);
			lightGroup->SetDirLightActive(1, false);
			lightGroup->SetDirLightActive(2, false);

			lightGroup->SetSpotLightActive(0, false);

			lightGroup->SetPointLightActive(0, true);

			lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
			lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
			lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
		}

		if (State == 3) {
			lightGroup->SetDirLightActive(0, false);
			lightGroup->SetDirLightActive(1, false);
			lightGroup->SetDirLightActive(2, false);
			lightGroup->SetPointLightActive(0, false);
			lightGroup->SetPointLightActive(1, false);
			lightGroup->SetPointLightActive(2, false);


			lightGroup->SetSpotLightActive(0, true);

			lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0],spotLightDir[1],spotLightDir[2] ,0 }));
			lightGroup->SetSpotLightPos(0, XMFLOAT3({ spotLightPos }));
			lightGroup->SetSpotLightColor(0, XMFLOAT3({ spotLightColor }));
			lightGroup->SetSpotLightAtten(0, XMFLOAT3({ spotLightAtten }));
			lightGroup->SetSpotLightFactorAngle(0, XMFLOAT2(spotLightFactorAngle));
		}

		if (State == 4) {
			lightGroup->SetDirLightActive(0, false);
			lightGroup->SetDirLightActive(1, false);
			lightGroup->SetDirLightActive(2, false);
			lightGroup->SetPointLightActive(0, false);
			lightGroup->SetPointLightActive(1, false);
			lightGroup->SetPointLightActive(2, false);

			lightGroup->SetSpotLightActive(0, false);

			lightGroup->SetDirLightActive(0, true);
			lightGroup->SetDirLightActive(1, true);
			lightGroup->SetDirLightActive(2, true);
			lightGroup->SetCircleShadowActive(0, true);

			lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0],circleShadowDir[1],circleShadowDir[2] ,0 }));
			lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({ fightPos[0],fightPos[1],fightPos[2] }));
			lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
			lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
		}

		

		object3d2->SetPosition(XMFLOAT3({ fightPos[0], fightPos[1], fightPos[2] }));

		XMFLOAT3 pos3d;
		XMFLOAT3 pos3d2;

		pos3d = object3d->GetRotation();
		pos3d2 = object3d2->GetPosition();

		pos3d.y++;

		object3d->SetRotation(pos3d);
		object3d2->SetRotation(pos3d);

#pragma region DirectX毎フレーム処理

		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		object3d->Draw();
		object3d2->Draw();
		object3d3->Draw();
		

		if (State == 1 || State == 4) {
			objectFloor2->Draw();
		}
		else {
			objectFloor->Draw();
		}

		Object3D::PostDraw();

		sprite->Draw(tex1);
		sprite2->Draw(tex2);
		sprite3->Draw(tex3);

		//描画後処理
		dxBasis->PostDraw();
#pragma endregion

	}


	sound->Finalize();
	delete sound;
	//delete light;
	delete sprite;
	delete sprite2;
	delete sprite3;
	delete lightGroup;
	delete model;
	delete model2;
	delete model3;
	delete model4;
	delete object3d;
	delete object3d2;
	delete object3d3;
	delete objectFloor;
	delete objectFloor2;
	//ウィンドウクラスを登録解除
	winApp->Release();

	return 0;
}
