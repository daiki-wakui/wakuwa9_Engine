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

	//windowsAPI�̐����N���X
	std::unique_ptr<WindowsApp> windows_ = std::make_unique<WindowsApp>();

	//keyborad�N���X�̐���
	std::unique_ptr<KeyBoard> keyboard_ = std::make_unique<KeyBoard>();

	std::unique_ptr<GamePad> gamePad_ = std::make_unique<GamePad>();

	//DirectX�̊�Ր����N���X
	std::unique_ptr<DirectXBasis> directX_ = std::make_unique<DirectXBasis>();

	//imgui
	std::unique_ptr<ImGuiManager> imGuiM_ = std::make_unique<ImGuiManager>();

	//���C�g�O���[�v
	std::unique_ptr<LightGroup> lightGroup = std::make_unique<LightGroup>();

public:

	virtual ~Framework() = default;

	//������
	virtual void Initialize();

	//�I��
	virtual void Finalize();

	//���t���[��
	virtual void Update();

	//�`��
	virtual void Draw() = 0;

	virtual void IsEnd();

	virtual bool GetIsEnd() { return isEndGame_; }

	//���s
	void Run();

	WindowsApp* GetWindows() { return windows_.get(); }
	DirectXBasis* GetDirectX() { return directX_.get(); }
	ImGuiManager* GetImGui() { return imGuiM_.get(); }
	KeyBoard* GetInput() { return keyboard_.get(); }
};

