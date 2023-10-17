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
	std::unique_ptr<WindowsApp> windows_ = std::make_unique<WindowsApp>();

	//keyboradクラスの生成
	KeyBoard* keyboard_ = KeyBoard::GetInstance();

	GamePad* gamePad_ = GamePad::GetInstance();

	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> directX_ = std::make_unique<DirectXBasis>();

	//imgui
	std::unique_ptr<ImGuiManager> imGuiM_ = std::make_unique<ImGuiManager>();

	//ライトグループ
	std::unique_ptr<LightGroup> lightGroup = std::make_unique<LightGroup>();

	//スプライト基盤
	std::unique_ptr<SpriteBasis> spBasis_ = std::make_unique<SpriteBasis>();

	//サウンド
	std::unique_ptr<Sound> sound_ = std::make_unique<Sound>();

	
public:

	std::unique_ptr<SceneManager> sceneManager_ = nullptr;


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

	WindowsApp* GetWindows() { return windows_.get(); }
	DirectXBasis* GetDirectX() { return directX_.get(); }
	ImGuiManager* GetImGui() { return imGuiM_.get(); }
	KeyBoard* GetInput() { return keyboard_; }
	GamePad* GetInputPad() { return gamePad_; }
	SpriteBasis* GetSpBasis() { return spBasis_.get(); }
	Sound* GetSound() { return sound_.get(); }
};

