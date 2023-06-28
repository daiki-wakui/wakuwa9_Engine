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

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <json.hpp>
#include "LevelLoader.h"
#include <fstream>
#include <cassert>
#include <map>

#include "IventBox.h"

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
	Model* enemyModel = Model::LoadFromObj("enemySou");
	Model* enemyModelRed = Model::LoadFromObj("enemySou2");
	Model* cube = Model::LoadFromObj("cube");


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

	/*OutputDebugStringA("文字列リテラルを出力するよ\n");

	std::string a("stringに埋め込んだ文字列を取得するよ\n");
	OutputDebugStringA(a.c_str());*/

	//FBXファイル読み込み
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");

	LevelData* levelData = nullptr;

	// レベルデータの読み込み
	levelData = LevelLoader::LoadFile("obj");

	IventBox* Box = new IventBox;
	bool ivent = false;

	std::map<std::string, Model*> models;
	std::vector<Object3D*> objects;

	models.insert(std::make_pair(std::string("player"), playerModel));
	models.insert(std::make_pair(std::string("enemy"), floorModel));
	models.insert(std::make_pair(std::string("enemySpawn"), enemyModel));
	models.insert(std::make_pair(std::string("enemySpawn2"), enemyModelRed));
	models.insert(std::make_pair(std::string("IventBlock"), cube));


	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		if (objectData.fileName == "camera") {

			DirectX::XMFLOAT3 eye;
			DirectX::XMStoreFloat3(&eye, objectData.translation);
			Object3D::CameraMoveVector({eye.x+2.0f,eye.y-1.5f,eye.z});
		}

		if (objectData.fileName == "IventBlock") {
			// モデルを指定して3Dオブジェクトを生成
			Object3D* newObject = Object3D::Create(1.0f);
			//newObject->SetModel(playerModel);
			newObject->SetModel(model);

			// 座標
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			newObject->SetPosition(pos);

			// 回転角
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			newObject->SetRotation({ rot.x,rot.y,rot.z });


			// 座標
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);

			
			Box->Initialize(model, newObject);
		}
		else {
			// モデルを指定して3Dオブジェクトを生成
			Object3D* newObject = Object3D::Create(1.0f);
			//newObject->SetModel(playerModel);
			newObject->SetModel(model);


			// 座標
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			newObject->SetPosition(pos);

			// 回転角
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			newObject->SetRotation({ rot.x,rot.y - 90,rot.z });


			// 座標
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);

			// 配列に登録
			objects.push_back(newObject);
		}
		
	}

	int32_t state = 0;

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}

		sprite->Update();

		playerObject->Update();
		skyObject->Update();
		objectFloor->Update();

		for (auto& object : objects) {
			object->Update();
		}

		//keyborad更新処理
		input_->Update();

		lightGroup->Update();

		if (input_->keyInstantPush(DIK_SPACE) && Box->arive == true) {
			ivent = (ivent + 1) % 2;
		}

		if (input_->keyInstantPush(DIK_1)) {
			state = (state + 1) % 2;
		}

		if (Box->arive == true) {
			Box->Update();
		}
		


		//デバック用
		imguiM->Begin();

		imguiM->End();

#pragma region DirectX毎フレーム処理

		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		//playerObject->Draw();

		if (ivent == true) {
			skyObject->Draw();
		}
		
		//objectFloor->Draw();

		for (auto& object : objects) {
			object->Draw();
		}

		if (Box->arive == true && state == false) {
			
			Box->Draw();
		}
		

		Object3D::PostDraw();

		sprite->Draw(tex1);

		//描画後処理
		dxBasis->PostDraw();

		imguiM->Draw();

#pragma endregion

	}


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
