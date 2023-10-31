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

	DirectXBasis* directX_ = DirectXBasis::GetInstance();
	SpriteBasis* spBasis_ = SpriteBasis::GetInstance();
	WindowsApp* windows_ = WindowsApp::GetInstance();
	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();
	ImGuiManager* imGuiM_ = ImGuiManager::GetInstance();
	Sound* sound_ = Sound::GetInstance();

	
	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_ = std::make_unique<PostEffect>();
	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect2_ = std::make_unique<PostEffect>();


	std::unique_ptr<GameScene> gamescene_ = std::make_unique<GameScene>();
	std::unique_ptr<TitleScene> titlescene_ = std::make_unique<TitleScene>();

	int32_t state = 0;
	std::unique_ptr<ParticleManager> particleMan_ = std::make_unique<ParticleManager>();

	int32_t isDebug;

public:

	//初期化
	void Initialize() override;

	void Finalize() override;
	
	//毎フレーム
	void Update() override;

	void Draw() override;
};

