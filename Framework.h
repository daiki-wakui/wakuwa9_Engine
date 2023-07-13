#pragma once
#include "WindowsApp.h"
#include "DirectXBasis.h"
#include "ImGuiManager.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "LightGroup.h"
#include "SpriteBasis.h"
#include "Sound.h"

#include "Object3D.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"

#include <memory>

class Framework
{
private:
	MSG msg{};

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

	//�X�v���C�g���
	std::unique_ptr<SpriteBasis> spBasis_ = std::make_unique<SpriteBasis>();

	//�T�E���h
	std::unique_ptr<Sound> sound_ = std::make_unique<Sound>();

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
	GamePad* GetInputPad() { return gamePad_.get(); }
	SpriteBasis* GetSpBasis() { return spBasis_.get(); }
	Sound* GetSound() { return sound_.get(); }
};

