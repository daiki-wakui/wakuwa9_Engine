#pragma once
#include "WindowsApp.h"
#include "DirectXBasis.h"
#include "ImGuiManager.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "LightGroup.h"
#include "SpriteBasis.h"
#include "Sound.h"

class BaseScene
{
public:

	~BaseScene() = default;

	//初期化
	virtual void Initialize() = 0;

	//終了
	virtual void Finalize() = 0;

	//毎フレーム
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

};

