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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//windowsAPI�̐����N���X
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyborad�N���X�̐���
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectX�̊�Ր����N���X
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//ImGui�N���X�̐���
	std::unique_ptr<ImGuiManager> ImGuiM;
	ImGuiManager* imguiM = new ImGuiManager;

	//Sprite�̊�Ր����N���X
	std::unique_ptr<SpriteBasis> SpBasis;
	SpriteBasis* spBasis = new SpriteBasis();

	//windowsAPI������
	winApp->Initalize();
	windows.reset(winApp);
	MSG msg{};

	//DirectX������
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	imguiM->Initialize(winApp,dxBasis);
	ImGuiM.reset(imguiM);

	//Fbx������
	FbxLoader::GetInstance()->Initialize(dxBasis->GetDevice());

	Sound* sound = nullptr;

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//DirectionalLight::StaticInitalize(dxBasis->GetDevice());
	LightGroup::StaticInitialize(dxBasis->GetDevice());

	//keyborad������
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	//DirectionalLight* light = nullptr;
	LightGroup* lightGroup = nullptr;

	//���C�g����
	lightGroup = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup);

	//���s����
	float ambientColor0[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//�_����
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;

	//�X�|�b�g���C�g
	float spotLightDir[3] = { 0,-1,0 };
	float spotLightPos[3] = { 0,5,0 };
	float spotLightColor[3] = { 1,1,1 };
	float spotLightAtten[3] = { 0.0f,0.0f,0.0f };
	float spotLightFactorAngle[2] = { 20.0f,30.0f};

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.0f,0.0f };
	float circleShadowFactorAngle[2] = { 1.0f,2.0f };

	float playerPos[3] = { 0,0.0f,5 };

	int State = 0;


	spBasis->Initialize(dxBasis);

	//�Q�[���V�[���Ɏg��������

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

	//OBJ���烂�f����ǂݍ���
	Model* floorModel = Model::LoadFromObj("floor");
	Model* skydomModel = Model::LoadFromObj("world");
	Model* playerModel = Model::LoadFromObj("player");
	Model* enemyModel = Model::LoadFromObj("enemySou");
	Model* enemyModelRed = Model::LoadFromObj("enemySou2");


	//3D�I�u�W�F�N�g����
	Object3D* playerObject = Object3D::Create(2.0f);
	Object3D* skyObject = Object3D::Create(100.0f);
	Object3D* objectFloor = Object3D::Create(5.0f);

	//3D�I�u�W�F�N�g��3D���f����R�Â�
	playerObject->SetModel(playerModel);
	skyObject->SetModel(skydomModel);

	playerObject->SetPosition(XMFLOAT3(playerPos));

	objectFloor->SetModel(floorModel);
	objectFloor->SetPosition({ 0,-10,0 });

	//�I�[�f�B�I������
	sound = new Sound();
	sound->Initialize();

	sound->LoadWave("PerituneMaterial.wav");
	sound->LoadWave("Alarm01.wav");

	/*OutputDebugStringA("�����񃊃e�������o�͂����\n");

	std::string a("string�ɖ��ߍ��񂾕�������擾�����\n");
	OutputDebugStringA(a.c_str());*/

	//FBX�t�@�C���ǂݍ���
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");

	LevelData* levelData = nullptr;

	// ���x���f�[�^�̓ǂݍ���
	levelData = LevelLoader::LoadFile("obj");

	std::map<std::string, Model*> models;
	std::vector<Object3D*> objects;

	models.insert(std::make_pair(std::string("player"), playerModel));
	models.insert(std::make_pair(std::string("enemy"), floorModel));
	models.insert(std::make_pair(std::string("enemySpawn"), enemyModel));
	models.insert(std::make_pair(std::string("enemySpawn2"), enemyModelRed));


	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		// �t�@�C��������o�^�ς݃��f��������
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

		// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		Object3D* newObject = Object3D::Create(1.0f);
		//newObject->SetModel(playerModel);
		newObject->SetModel(model);


		// ���W
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->SetPosition(pos);

		// ��]�p
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject->SetRotation({rot.x,rot.y-90,rot.z});


		// ���W
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->SetScale(scale);

		// �z��ɓo�^
		objects.push_back(newObject);
	}

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (winApp->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}

		sprite->Update();

		playerObject->Update();
		skyObject->Update();
		objectFloor->Update();

		for (auto& object : objects) {
			object->Update();
		}

		//keyborad�X�V����
		input_->Update();

		lightGroup->Update();


		//�f�o�b�N�p
		imguiM->Begin();

		imguiM->End();

#pragma region DirectX���t���[������

		// �`��O����
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		//playerObject->Draw();
		//skyObject->Draw();
		//objectFloor->Draw();

		for (auto& object : objects) {
			object->Draw();
		}

		Object3D::PostDraw();

		sprite->Draw(tex1);

		//�`��㏈��
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
	//�E�B���h�E�N���X��o�^����
	winApp->Release();

	return 0;
}
