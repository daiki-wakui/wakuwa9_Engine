#pragma once
#include "Framework.h"
#include "BaseScene.h"

#include "Model.h"
#include "Sprite.h"
#include "Sound.h"

class TitleScene : public BaseScene
{
private:

	DirectXBasis* directX_ = nullptr;
	SpriteBasis* spBasis_ = nullptr;
	WindowsApp* windows_ = nullptr;
	ImGuiManager* imGuiM_ = nullptr;
	Sound* sound_ = nullptr;

	KeyBoard* keyboard_ = nullptr;
	GamePad* gamePad_ = nullptr;

	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();

	//‰æ‘œ
	int32_t tex1_ = 0;

public:

	void SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound_);
	void SetInputInfo(KeyBoard* keyboard, GamePad* gamePad);

	//‰Šú‰»
	void Initialize() override;

	//I—¹
	void Finalize()override;

	//–ˆƒtƒŒ[ƒ€
	void Update()override;

	//•`‰æ
	void Draw()override;

};

