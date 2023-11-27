#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::LoadTexture()
{
	testDDSImage_ = spBasis_->TextureData(L"Resources/test.dds");

	titleImage_ = spBasis_->TextureData(L"Resources/title.png");
	titleUIImage_ = spBasis_->TextureData(L"Resources/titleUI.png");
	sceneChangeImage_ = spBasis_->TextureData(L"Resources/sceneChange.png");
	filterImage_ = spBasis_->TextureData(L"Resources/fillter.png");
	testImage_ = spBasis_->TextureData(L"Resources/testB.png");


	playerHP_ = spBasis_->TextureData(L"Resources/playerHP.png");
	bossHP_ = spBasis_->TextureData(L"Resources/red.png");

	gameover_ = spBasis_->TextureData(L"Resources/gameover.png");
	gameclear_ = spBasis_->TextureData(L"Resources/gameclear.png");

	reticleImage_ = spBasis_->TextureData(L"Resources/reticle.png");

	tesImage_ = spBasis_->TextureData(L"Resources/test.png");

	targetImage_ = spBasis_->TextureData(L"Resources/targetPoint.png");

	iventImage_ = spBasis_->TextureData(L"Resources/Ivent.png");

	warningImage_ = spBasis_->TextureData(L"Resources/warning.png");

	manualImageRB_ = spBasis_->TextureData(L"Resources/RB.png");

	exImage_ = spBasis_->TextureData(L"Resources/ex.png");

	damageFilter_ = spBasis_->TextureData(L"Resources/damageFilter.png");


	spBasis_->TextureSetting();

}
