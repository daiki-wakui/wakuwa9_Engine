#include "GameUI.h"

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

void GameUI::Initialize()
{
	TextureManager::LoadTexture();

	titleSprite_->Initialize(titleImage_);
	titleSprite_->Create();
	titleSprite_->SetSize({ 1280,720 });
	titleSprite_->SetAncP({ 0,0 });

	titleUISprite_->Initialize(titleUIImage_);
	titleUISprite_->Create();
	titleUISprite_->SetSize({ 1280,720 });
	titleUISprite_->SetAncP({ 0,0 });

	sceneSprite_->Initialize();
	sceneSprite_->Create(640, 360);
	sceneSprite_->SetSize({ 1280,720 });
	sceneSprite_->SetColor({ 1,1,1,0 });

	fillSprite_->Initialize();
	fillSprite_->Create(640, 360);
	fillSprite_->SetSize({ 1280,720 });
	fillSprite_->Update();
}

void GameUI::TitleUpdate(bool sceneChange)
{
	titleSprite_->Update();
	titleUISprite_->Update();
	sceneSprite_->Update();

	if (sceneChange) {
		ChangeAlpha_ += 0.05f;
		ChangeAlpha_ = min(ChangeAlpha_, 1);
		sceneSprite_->SetColor({ 1, 1, 1, ChangeAlpha_ });
	}
	else {
		ChangeAlpha_ = 0;
		sceneSprite_->SetColor({ 1, 1, 1, ChangeAlpha_ });
	}

}

void GameUI::Draw()
{
	titleSprite_->Draw(titleImage_);
	titleUISprite_->Draw(titleUIImage_);
	sceneSprite_->Draw(sceneChangeImage_);
}

void GameUI::OffDraw()
{
	fillSprite_->Draw(filterImage_);
}
