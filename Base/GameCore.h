#pragma once
#include "Framework.h"
#include "GameScene.h"
#include "TitleScene.h"

#include "PostEffect.h"
#include "ParticleManager.h"

#include <fstream>
#include <cassert>
#include <map>

#include <memory>
#include <string>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <cstdint>



class GameCore : public Framework 
{
private:

	DirectXBasis* directX_ = Framework::GetDirectX();
	SpriteBasis* spBasis_ = Framework::GetSpBasis();
	WindowsApp* windows_ = Framework::GetWindows();
	KeyBoard* keyboard_ = Framework::GetInput();
	GamePad* gamePad_ = Framework::GetInputPad();
	ImGuiManager* imGuiM_ = Framework::GetImGui();
	Sound* sound_ = Framework::GetSound();

	
	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_ = std::make_unique<PostEffect>();
	std::unique_ptr<PostEffect> gaussianEffect_ = std::make_unique<PostEffect>();
	
	std::unique_ptr<BaseScene> scene = std::make_unique<TitleScene>();

	std::unique_ptr<GameScene> gamescene_ = std::make_unique<GameScene>();

	std::unique_ptr<TitleScene> titlescene_ = std::make_unique<TitleScene>();

	int32_t state = 0;

	std::unique_ptr<ParticleManager> object3d = std::make_unique<ParticleManager>();


public:

	//初期化
	void Initialize() override;

	void Finalize() override;
	
	//毎フレーム
	void Update() override;

	void Draw() override;
};

