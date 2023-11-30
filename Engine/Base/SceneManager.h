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

enum Scene{
	TITLE,
	GAME
};

class SceneManager
{
private:

	//キーボードクラスとゲームパッドを使えるように
	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();

	//シーンの追加
	std::unique_ptr<GameScene> gamescene_ = std::make_unique<GameScene>();
	std::unique_ptr<TitleScene> titlescene_ = std::make_unique<TitleScene>();

	//現在のシーン
	int32_t nowScene_ = 0;

public:

	~SceneManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void ParticleDraw();
	void OffEffectDraw();

	//タイトルシーンからゲームシーンに変わる時
	bool ChangeToGameScene();
	bool ChangeToTitleScene();

public: //getter

	int32_t GetSceneState() { return nowScene_; }
	GameScene* GetGameScene() { return gamescene_.get(); }
	TitleScene* GetTitleScene() { return titlescene_.get(); }
};

