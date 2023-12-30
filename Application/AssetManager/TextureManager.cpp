#include "TextureManager.h"

//コンストラクタ
TextureManager::TextureManager()
{
}

//デストラクタ
TextureManager::~TextureManager()
{
}

//タイトルシーンで使うスプライト読み込み
void TextureManager::LoadTitleTexture()
{
	//タイトルシーンに使うアセット
	titleImage_ = spBasis_->TextureData(L"Resources/title.png");
	titleUIImage_ = spBasis_->TextureData(L"Resources/titleUI.png");
	sceneChangeImage_ = spBasis_->TextureData(L"Resources/sceneChange.png");
	filterImage_ = spBasis_->TextureData(L"Resources/fillter.png");
	testImage_ = spBasis_->TextureData(L"Resources/testB.png");

	spBasis_->TextureSetting();

}

//ゲームシーンで使うスプライト読み込み
void TextureManager::LoadGameTexture()
{
	titleImage_ = spBasis_->TextureData(L"Resources/title.png");
	sceneChangeImage_ = spBasis_->TextureData(L"Resources/sceneChange.png");
	filterImage_ = spBasis_->TextureData(L"Resources/fillter.png");
	//ゲームシーンに使うアセット
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
	bulletRreticleImage_ = spBasis_->TextureData(L"Resources/shotReticle.png");


	spBasis_->TextureSetting();
}
