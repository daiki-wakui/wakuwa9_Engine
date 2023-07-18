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
private:

	DirectXBasis* directX_ = nullptr;
	SpriteBasis* spBasis_ = nullptr;
	WindowsApp* windows_ = nullptr;
	ImGuiManager* imGuiM_ = nullptr;
	Sound* sound_ = nullptr;

	KeyBoard* keyboard_ = nullptr;
	GamePad* gamePad_ = nullptr;

public:

	void SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound);
	void SetInputInfo(KeyBoard* keyboard, GamePad* gamePad);

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

