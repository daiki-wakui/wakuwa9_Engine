#include "GameUI.h"
#include "MyRandom.h"
#include "Easing.h"

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

void GameUI::SetInfo(Player* player ,Object3D* playerObject, Boss* boss)
{
	player_ = player;
	playerObject_ = playerObject;
	boss_ = boss;
}

void GameUI::boolInfo(bool hitBox, bool isIvent)
{
	hitBox_ = hitBox;
	isIvent_ = isIvent;
}

void GameUI::TitleSceneInitialize()
{
	TextureManager::LoadTitleTexture();

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
	TextureManager::LoadGameTexture();

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
		ChangeTitleAlpha_ += 0.05f;
		ChangeTitleAlpha_ = min(ChangeTitleAlpha_, 1);
		sceneSprite_->SetColor({ 1, 1, 1, ChangeTitleAlpha_ });
	}
	else {
		ChangeTitleAlpha_ = 0;
		sceneSprite_->SetColor({ 1, 1, 1, ChangeTitleAlpha_ });
	}

}

void GameUI::GameUpdate()
{
	iventSprite_->SetColor({ 1,1,1,iventAlpha_ });
	playerHPSprite_->SetSize({ 32.0f * (float)player_->GetHP(),16.0f });

	//チュートリアルUI
	RBSprite_->SetPosition({ screenPosPlayer_.x - 175,screenPosPlayer_.y - 90 });

	if (!isManual_) {
		alphaRB_ += 0.15f;
		alphaRB_ = min(alphaRB_, 1);
		RBSprite_->SetColor({ 1,1,1,alphaRB_ });
	}
	else {
		alphaRB_ -= 0.15f;
		alphaRB_ = max(alphaRB_, 0);
		RBSprite_->SetColor({ 1,1,1,alphaRB_ });
	}

	//弾を打ったときの左右レティクル
	if (gamePad_->PushButtonRB()) {
		isManual_ = true;

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
	
	//プレイヤーの3D座標をスクリーン変換した座標
	screenPosPlayer_ = playerObject_->Screen();

	//プレイヤーのレティクル座標
	bulletRreticleSprite_->SetPosition({ player_->GetScreenRTPos().x,player_->GetScreenRTPos().y });
	
	//ライフ危ない時に出るフィルター
	if (player_->GetHP() <= 1) {
		isLifeOne_ = true;
	}
	else {
		isLifeOne_ = false;
	}
	//ライフ危ない時に出るフィルター
	if (isLifeOne_) {
		fillTimer_++;

		if (fillTimer_ < 50) {
			fillAlpha_ += 0.07f;
			fillAlpha_ = min(fillAlpha_, 1);
		}
		else {
			fillAlpha_ -= 0.05f;
			fillAlpha_ = max(fillAlpha_, 0);
		}
		dFilterSprite_->SetColor({ 1,1,1,fillAlpha_ });

	}
	else {
		fillTimer_ = 0;
	}


	if (hitBox_ && !isIvent_) {
		if (count_ < 3) {
			pow_++;
			wSize_.y = waringSprite_->GetSize().y;
		}
		else {
			pow_ = 0;
		}

		//ワーニング点滅
		if (pow_ > 2) {
			pow_ = 0;
			count_++;
		}

		//点滅後の動き
		if (count_ == 3) {
			popFrame_++;

			if (popFrame_ > 60) {
				wTimer_++;
				wTimer_ = min(wTimer_, wMax_);

				wSize_ = wSize_.lerp({ 1280,720,0 }, { 1280,0,0 }, Easing::EaseOutCubic(wTimer_, wMax_));

				waringSprite_->SetSize({ wSize_.x,wSize_.y });
			}
		}
	}

	dFilterSprite_->Update();
	sceneSprite_->Update();
	iventSprite_->Update();
	waringSprite_->Update();
	RBSprite_->Update();
	fillSprite_->Update();
	playerHPSprite_->Update();
	reticleSprite_->Update();
	bulletRreticleSprite_->Update();
}

void GameUI::TitleDraw()
{
	titleSprite_->Draw(titleImage_);
	titleUISprite_->Draw(titleUIImage_);
	sceneSprite_->Draw(sceneChangeImage_);
}

void GameUI::Reset()
{
	ChangeGameAlpha_ = 1;
	pow_ = 0;
	count_ = 0;
	waringSprite_->SetSize({ 1280,720 });
}

void GameUI::GameDraw()
{

	//ボスのイベントムービー中は非表示
	if (!isIvent_) {
		playerHPSprite_->Draw(playerHP_);
		reticleSprite_->Draw(reticleImage_);
		bulletRreticleSprite_->Draw(bulletRreticleImage_);
	}

	//ボスが現れたら表示
	if (hitBox_ == true && boss_->GetArive() == true && isIvent_ == false) {
		bossHPSprite_->Draw(bossHP_);
	}
	
	//ゲームオーバー表示
	if (player_->IsDead()) {
		gameoverSprite_->Draw(gameover_);
	}

	//ゲームクリア表示
	if (!boss_->GetArive()) {
		gameclearSprite_->Draw(gameclear_);
	}
	
	if (pow_ < 1 && hitBox_ && movieEnd_) {
		waringSprite_->Draw(warningImage_);
	}


	RBSprite_->Draw(manualImageRB_);
	sceneSprite_->Draw(sceneChangeImage_);
	fillSprite_->Draw(filterImage_);
	dFilterSprite_->Draw(damageFilter_);
	iventSprite_->Draw(iventImage_);
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

void GameUI::BossHpUI()
{
	bossHPSprite_->SetSize({ 16.0f * (float)boss_->GetHP(),32.0f });
	bossHPSprite_->Update();
}

void GameUI::SceneStartFadeUI() {
	ChangeGameAlpha_ -= 0.05f;
	ChangeGameAlpha_ = max(ChangeGameAlpha_, 0);
	sceneSprite_->SetColor({ 1,1,1,ChangeGameAlpha_ });
}

void GameUI::BossIventSceneUI() {
	if (isIvent_) {
		iventAlpha_ += 0.05f;
		iventAlpha_ = min(iventAlpha_, 1);
	}
	else {
		iventAlpha_ -= 0.05f;
		iventAlpha_ = max(iventAlpha_, 0);
	}
}