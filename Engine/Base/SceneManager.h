#pragma once
#include "TitleScene.h"
#include "GameScene.h"
#include "KeyBoard.h"
#include "GamePad.h"

#include "Object3D.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
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

class SceneManager
{
private:

	std::unique_ptr<GameScene> gamescene_ = std::make_unique<GameScene>();
	std::unique_ptr<TitleScene> titlescene_ = std::make_unique<TitleScene>();
	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();

	int32_t state = 0;

	bool sceneChange_ = 0;

public:

	~SceneManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void ParticleDraw();
	void OffEffectDraw();

	int32_t GetSceneState() { return state; }
	GameScene* GetGameScene() { return gamescene_.get(); }
	TitleScene* GetTitleScene() { return titlescene_.get(); }

	bool ChangeToGameScene();

	//void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
};

