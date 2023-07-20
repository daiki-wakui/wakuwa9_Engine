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

	//�摜
	int32_t tex1_ = 0;

	int32_t title_ = 0;


public:

	static void SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound_);
	static void SetInputInfo(KeyBoard* keyboard, GamePad* gamePad);

	//������
	void Initialize() override;

	//�I��
	void Finalize()override;

	//���t���[��
	void Update()override;

	//�`��
	void Draw()override;

};

