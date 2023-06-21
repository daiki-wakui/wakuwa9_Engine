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

#include "PostEffect.h"

class GameCore
{
private:

	//windowsAPI�̐����N���X
	std::unique_ptr<WindowsApp> windows_ = std::make_unique<WindowsApp>();

	//keyborad�N���X�̐���
	std::unique_ptr<KeyBoard> keyboard_ = std::make_unique<KeyBoard>();

	//DirectX�̊�Ր����N���X
	std::unique_ptr<DirectXBasis> directX_ = std::make_unique<DirectXBasis>();

	//imgui
	std::unique_ptr<ImGuiManager> imGuiM_ = std::make_unique<ImGuiManager>();

	//���C�g�O���[�v
	std::unique_ptr<LightGroup> lightGroup = std::make_unique<LightGroup>();

	//OBJ���烂�f����ǂݍ���
	std::unique_ptr<Model> floorModel_;
	std::unique_ptr<Model> skydomModel_;

	std::unique_ptr<Object3D> skyObject_;
	std::unique_ptr<Object3D> objectFloor_;

	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Object3D> playerObject_;

	//FBX
	std::unique_ptr<FbxModel> testModel_;
	std::unique_ptr<FbxObject3d> testObj_;

	//�X�v���C�g���
	std::unique_ptr<SpriteBasis> spBasis_ = std::make_unique<SpriteBasis>();

	//�X�v���C�g
	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();

	//�摜
	int32_t tex1_ = 0;
	int32_t tex2_ = 0;
	int32_t tex3_ = 0;

	int32_t postTex = 100;

	//�|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect_ = std::make_unique<PostEffect>();
	
	//�T�E���h
	std::unique_ptr<Sound> sound_ = std::make_unique<Sound>();

public:

	//������
	void Initialize();

	void Finalize();

	//���t���[��
	void Update();

	void Draw();

	WindowsApp* GetWindows() { return windows_.get(); }
	DirectXBasis* GetDirectX() { return directX_.get(); }
	ImGuiManager* GetImGui() { return imGuiM_.get(); }
	KeyBoard* GetInput() { return keyboard_.get(); }
};

