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

	//‰Šú‰»
	virtual void Initialize() = 0;

	//I—¹
	virtual void Finalize() = 0;

	//–ˆƒtƒŒ[ƒ€
	virtual void Update() = 0;

	//•`‰æ
	virtual void Draw() = 0;

};

