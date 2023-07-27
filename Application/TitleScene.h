#pragma once
#include "Framework.h"
#include "BaseScene.h"

#include "Model.h"
#include "Sprite.h"
#include "Sound.h"

class TitleScene : public BaseScene
{
private:

	static DirectXBasis* directX_;
	static SpriteBasis* spBasis_;
	static WindowsApp* windows_;
	static ImGuiManager* imGuiM_;
	static Sound* sound_;

	static KeyBoard* keyboard_;
	static GamePad* gamePad_;

	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Model> skydomModel_;
	std::unique_ptr<Object3D> skyObject_;

	//‰æ‘œ
	int32_t tex1_ = 0;

	int32_t title_ = 0;

	int32_t scene_ = 0;

	XMFLOAT2 pos = {0,0};
	float power = 1;

	bool start_ = false;
	bool change_ = false;

	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();

	bool playBGM_ = false;

public:

	bool GetChange() { return change_; }
	void SetChange(bool change) { change_ = change; }
	bool GetStart() { return start_; }
	void SetStart(bool start) { start_ = start; }

	static void SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound_);
	static void SetInputInfo(KeyBoard* keyboard, GamePad* gamePad);

	//‰Šú‰»
	void Initialize() override;

	//I—¹
	void Finalize()override;

	//–ˆƒtƒŒ[ƒ€
	void Update()override;

	//•`‰æ
	void Draw()override;

};

