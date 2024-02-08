#pragma once
#include "WindowsApp.h"
#include "DirectXBasis.h"
#include "ImGuiManager.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "LightGroup.h"
#include "SpriteBasis.h"
#include "Sound.h"
#include "Model3DManager.h"

class SceneManager;

class BaseScene
{
private:

	DirectXBasis* directX_ = DirectXBasis::GetInstance();
	SpriteBasis* spBasis_ = SpriteBasis::GetInstance();
	WindowsApp* windows_ = WindowsApp::GetInstance();
	ImGuiManager* imGuiM_ = ImGuiManager::GetInstance();
	Sound* sound_ = Sound::GetInstance();

	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();

protected:

	SceneManager* sceneManager_ = nullptr;

public:
	
	~BaseScene() = default;

	//初期化
	virtual void Initialize() = 0;

	//終了
	virtual void Finalize() = 0;

	//毎フレーム
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
};

