#pragma once
#include "WindowsApp.h"
#include "DirectXBasis.h"
#include "ImGuiManager.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "LightGroup.h"
#include "SpriteBasis.h"
#include "Sound.h"
#include "SceneManager.h"
#include "AbstactSceneFactory.h"

#include "Object3D.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"

#include <memory>

class Framework
{
private:
	MSG msg{};

	bool isEndGame_ = false;

	//windowsAPIの生成クラス
	WindowsApp* windows_ = WindowsApp::GetInstance();

	//keyboradクラスの生成
	KeyBoard* keyboard_ = KeyBoard::GetInstance();

	GamePad* gamePad_ = GamePad::GetInstance();

	//DirectXの基盤生成クラス
	DirectXBasis* directX_ = DirectXBasis::GetInstance();

	//imgui
	ImGuiManager* imGuiM_ = ImGuiManager::GetInstance();

	//ライトグループ
	std::unique_ptr<LightGroup> lightGroup = std::make_unique<LightGroup>();

	//スプライト基盤
	SpriteBasis* spBasis_ = SpriteBasis::GetInstance();

	//サウンド
	Sound* sound_ = Sound::GetInstance();

	SceneManager* sceneManager_ = SceneManager::GetInstance();
	AbstractSceneFactory* sceneFactory_ = nullptr;

public:

	virtual ~Framework() = default;

	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//毎フレーム
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	virtual void IsEnd();

	virtual bool GetIsEnd() { return isEndGame_; }

	//実行
	void Run();
};

