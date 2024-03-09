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
	stepFilterImage_ = spBasis_->TextureData(L"Resources/step_fillter.png");
	//ゲームシーンに使うアセット
	playerHP_ = spBasis_->TextureData(L"Resources/playerHP.png");
	bossHP_ = spBasis_->TextureData(L"Resources/red.png");
	bossMaxHP_ = spBasis_->TextureData(L"Resources/black.png");
	bossHPImage_ = spBasis_->TextureData(L"Resources/bossHP.png");

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

	editImage_[0] = spBasis_->TextureData(L"Resources/z_gui/edit.png");
	editImage_[1] = spBasis_->TextureData(L"Resources/z_gui/edit_ak.png");
	editImage_[2] = spBasis_->TextureData(L"Resources/z_gui/edit_move.png");
	editImage_[3] = spBasis_->TextureData(L"Resources/z_gui/edit_wit.png");
	nowEditImage_ = spBasis_->TextureData(L"Resources/z_gui/edit_now.png");
	EditBImage_ = spBasis_->TextureData(L"Resources/z_gui/edit_b.png");

	spBasis_->TextureSetting();
}
