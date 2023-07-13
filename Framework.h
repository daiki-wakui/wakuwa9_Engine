#pragma once
#include "WindowsApp.h"
#include "DirectXBasis.h"
#include "ImGuiManager.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "LightGroup.h"


class Framework
{
private:

	bool isEndGame_ = false;

	//windowsAPIの生成クラス
	std::unique_ptr<WindowsApp> windows_ = std::make_unique<WindowsApp>();

	//keyboradクラスの生成
	std::unique_ptr<KeyBoard> keyboard_ = std::make_unique<KeyBoard>();

	std::unique_ptr<GamePad> gamePad_ = std::make_unique<GamePad>();

	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> directX_ = std::make_unique<DirectXBasis>();

	//imgui
	std::unique_ptr<ImGuiManager> imGuiM_ = std::make_unique<ImGuiManager>();

	//ライトグループ
	std::unique_ptr<LightGroup> lightGroup = std::make_unique<LightGroup>();

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

	WindowsApp* GetWindows() { return windows_.get(); }
	DirectXBasis* GetDirectX() { return directX_.get(); }
	ImGuiManager* GetImGui() { return imGuiM_.get(); }
	KeyBoard* GetInput() { return keyboard_.get(); }
};

