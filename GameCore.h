#pragma once
#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "ImGuiManager.h"
#include "Sprite.h"
#include "SpriteBasis.h"
#include "Sound.h"
#include "DirectionalLight.h"
#include "LightGroup.h"
#include "Object3D.h"
#include "Model.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "DirectionalLight.h"

#include <memory>

#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <cstdint>

class GameCore
{
private:

	//windowsAPIの生成クラス
	std::unique_ptr<WindowsApp> windows;
	//keyboradクラスの生成
	std::unique_ptr<KeyBoard> keyboard;
	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> DirectX;
	
	std::unique_ptr<ImGuiManager> ImGuiM;

	WindowsApp* winApp = new WindowsApp();
	KeyBoard* input_ = new KeyBoard();
	DirectXBasis* dxBasis = new DirectXBasis();
	ImGuiManager* imguiM = new ImGuiManager;

	std::unique_ptr<SpriteBasis> SpBasis;
	SpriteBasis* spBasis = new SpriteBasis();

	Sound* sound = nullptr;

	LightGroup* lightGroup = nullptr;

	//OBJからモデルを読み込む
	Model* floorModel;
	Model* skydomModel;
	Model* playerModel;

	Object3D* playerObject;
	Object3D* skyObject;
	Object3D* objectFloor;

	FbxModel* cubeModel = nullptr;
	FbxObject3d* objcube = nullptr;

	FbxModel* testModel = nullptr;
	FbxObject3d* testObj = nullptr;


	Sprite* sprite = new Sprite();

	int32_t tex1 = 0;
	int32_t tex2 = 0;
	int32_t tex3 = 0;

public:

	//初期化
	void Initialize();

	void Finalize();

	//毎フレーム
	void Update();

	void Draw();

	WindowsApp* GetWindows() { return winApp; }
	DirectXBasis* GetDirectX() { return dxBasis; }
	ImGuiManager* GetImGui() { return imguiM; }
	KeyBoard* GetInput() { return input_; }
};

