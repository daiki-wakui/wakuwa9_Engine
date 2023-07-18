#include "TitleScene.h"

WindowsApp* TitleScene::windows_ = nullptr;
DirectXBasis* TitleScene::directX_ = nullptr;
ImGuiManager* TitleScene::imGuiM_ = nullptr;
SpriteBasis* TitleScene::spBasis_ = nullptr;
Sound* TitleScene::sound_ = nullptr;
KeyBoard* TitleScene::keyboard_ = nullptr;
GamePad* TitleScene::gamePad_ = nullptr;

void TitleScene::SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound)
{
	TitleScene::windows_ = windows;
	TitleScene::directX_ = directX;
	TitleScene::imGuiM_ = imGuiM;
	TitleScene::spBasis_ = spBasis;
	TitleScene::sound_ = sound;
}

void TitleScene::SetInputInfo(KeyBoard* keyboard, GamePad* gamePad)
{
	TitleScene::keyboard_ = keyboard;
	TitleScene::gamePad_ = gamePad;
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
