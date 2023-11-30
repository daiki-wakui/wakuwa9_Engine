#include "GameUI.h"
#include "MyRandom.h"

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

void GameUI::SetInfo(Player* player ,Object3D* playerObject)
{
	player_ = player;
	playerObject_ = playerObject;
}

void GameUI::TitleSceneInitialize()
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

void GameUI::GameSceneInitialize()
{
	//リソースを読み込む
	TextureManager::LoadTexture();

	//スプライトの初期化
	playerHPSprite_->Initialize();
	playerHPSprite_->Create(50, 20);
	playerHPSprite_->SetAncP({ 0,0 });

	bossHPSprite_->Initialize();
	bossHPSprite_->Create(640, 80);

	gameoverSprite_->Initialize();
	gameoverSprite_->Create(640, 360);
	gameoverSprite_->SetSize({ 1280,720 });
	gameoverSprite_->Update();

	gameclearSprite_->Initialize();
	gameclearSprite_->Create(640, 360);
	gameclearSprite_->SetSize({ 1280,720 });
	gameclearSprite_->Update();

	reticleSprite_->Initialize();
	reticleSprite_->Create(640, 360);
	reticleSprite_->SetSize({ 0,0 });

	sceneSprite_->Initialize();
	sceneSprite_->Create(640, 360);
	sceneSprite_->SetSize({ 1280,720 });
	sceneSprite_->SetColor({ 1,1,1,1 });

	fillSprite_->Initialize();
	fillSprite_->Create(640, 360);
	fillSprite_->SetSize({ 1280,720 });
	fillSprite_->Update();

	dFilterSprite_->Initialize();
	dFilterSprite_->Create(640, 360);
	dFilterSprite_->SetSize({ 1280,720 });
	dFilterSprite_->SetColor({ 1,1,1,0 });
	dFilterSprite_->Update();

	sSprite_->Initialize();
	sSprite_->Create(0, 0);
	sSprite_->SetSize({ 32,32 });
	//sSprite_->SetAncP({ 0,0 });
	sSprite_->Update();

	RBSprite_->Initialize();
	RBSprite_->Create(0, 0);
	RBSprite_->SetSize({ 160,160 });

	iventSprite_->Initialize();
	iventSprite_->Create(640, 360);
	iventSprite_->SetSize({ 1280,720 });
	iventSprite_->SetColor({ 1,1,1,0 });
	iventSprite_->Update();

	waringSprite_->Initialize();
	waringSprite_->Create(640, 360);
	waringSprite_->SetSize({ 1280,720 });
	waringSprite_->Update();

	bulletRreticleSprite_->Initialize();
	bulletRreticleSprite_->Create(640, 360);
	bulletRreticleSprite_->SetSize({ 32,32 });
	bulletRreticleSprite_->Update();

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

void GameUI::GameUpdate()
{
	dFilterSprite_->Update();
	sceneSprite_->Update();
	iventSprite_->Update();
	waringSprite_->Update();
	RBSprite_->Update();
	fillSprite_->Update();

	iventSprite_->SetColor({ 1,1,1,iventAlpha_ });

	playerHPSprite_->SetSize({ 32.0f * (float)player_->GetHP(),16.0f });
	playerHPSprite_->Update();


	//弾を打ったときの左右レティクル
	if (gamePad_->PushButtonRB()) {
		//manualOK_ = true;

		reticleSize_ = reticleSprite_->GetSize();
		reticleSize_.x += 300;
		reticleSize_.y += 200;
		reticleSize_.x = min(reticleSize_.x, 1280);
		reticleSize_.y = min(reticleSize_.y, 720);
	}
	else {
		reticleSize_ = reticleSprite_->GetSize();
		reticleSize_.x -= 300;
		reticleSize_.y -= 200;
		reticleSize_.x = max(reticleSize_.x, 0);
		reticleSize_.y = max(reticleSize_.y, 0);
	}

	reticleSprite_->SetSize(reticleSize_);
	reticleSprite_->Update();

	//プレイヤーの3D座標をスクリーン変換した座標
	screenPosPlayer_ = playerObject_->Screen();

	//プレイヤーのレティクル座標
	bulletRreticleSprite_->SetPosition({ player_->GetScreenRTPos().x,player_->GetScreenRTPos().y });
	bulletRreticleSprite_->Update();
}

void GameUI::TitleDraw()
{
	titleSprite_->Draw(titleImage_);
	titleUISprite_->Draw(titleUIImage_);
	sceneSprite_->Draw(sceneChangeImage_);
}

void GameUI::GameDraw()
{
	playerHPSprite_->Draw(playerHP_);
	reticleSprite_->Draw(reticleImage_);
	bulletRreticleSprite_->Draw(bulletRreticleImage_);

	///bossHPSprite_->Draw(bossHP_);

//	gameoverSprite_->Draw(gameover_);

	//gameclearSprite_->Draw(gameclear_);



	//RBSprite_->Draw(manualImageRB_);
	//sceneSprite_->Draw(sceneChangeImage_);
	//fillSprite_->Draw(filterImage_);
	//dFilterSprite_->Draw(damageFilter_);
	//iventSprite_->Draw(iventImage_);

//	waringSprite_->Draw(warningImage_);
}

void GameUI::OffDraw()
{
	fillSprite_->Draw(filterImage_);
}

void GameUI::Shake()
{
	randShake_.x = MyRandom::GetFloatRandom(-2.0f, 2.0f);
	randShake_.y = MyRandom::GetFloatRandom(-2.0f, 2.0f);

	playerHPSprite_->SetPosition({ 50 + randShake_.x * 5,20 + randShake_.y * 5 });
	bossHPSprite_->SetPosition({ 640 + randShake_.x * 5,80 + randShake_.y * 5 });
}
