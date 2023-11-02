#include "TextureManager.h"

void TextureManager::LoadTexture()
{
	titleImage_ = spBasis_->TextureData(L"Resources/title.png");
	titleUIImage_ = spBasis_->TextureData(L"Resources/titleUI.png");
	sceneChangeImage_ = spBasis_->TextureData(L"Resources/sceneChange.png");
	filterImage_ = spBasis_->TextureData(L"Resources/fillter.png");
	testImage_ = spBasis_->TextureData(L"Resources/testB.png");

	spBasis_->TextureSetting();

}
