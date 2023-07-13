#include "TitleScene.h"

void TitleScene::SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound)
{
	windows_ = windows;
	directX_ = directX;
	imGuiM_ = imGuiM;
	spBasis_ = spBasis;
	sound_ = sound;
}

void TitleScene::SetInputInfo(KeyBoard* keyboard, GamePad* gamePad)
{
	keyboard_ = keyboard;
	gamePad_ = gamePad;
}

void TitleScene::Initialize()
{
	tex1_ = spBasis_->TextureData(L"Resources/backTitle.png");

	spBasis_->TextureSetting();


	titleSprite_->Initialize(spBasis_, windows_);
	titleSprite_->Create(640, 360);
	titleSprite_->SetSize({ 1280,720 });
	titleSprite_->Update();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	titleSprite_->Update();

}

void TitleScene::Draw()
{
	titleSprite_->Draw(tex1_);
}
