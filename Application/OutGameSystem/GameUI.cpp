#include "GameUI.h"
#include "MyRandom.h"
#include "Easing.h"

//コンストラクタ
GameUI::GameUI()
{
}

//デストラクタ
GameUI::~GameUI()
{
}

//借りてくる情報
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

//タイトルシーンUI初期化
void GameUI::TitleSceneInitialize()
{
	TextureManager::LoadTitleTexture();

	titleSprite_->Initialize(titleImage_);
	titleSprite_->Create();
	titleSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	titleSprite_->SetAncP({ 0,0 });

	titleUISprite_->Initialize(titleUIImage_);
	titleUISprite_->Create();
	titleUISprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	titleUISprite_->SetAncP({ 0,0 });

	sceneSprite_->Initialize();
	sceneSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	sceneSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	sceneSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,0 });

	fillSprite_->Initialize();
	fillSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	fillSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	fillSprite_->Update();
}

//ゲームシーンUI初期化
void GameUI::GameSceneInitialize()
{
	//リソースを読み込む
	TextureManager::LoadGameTexture();

	//スプライトの初期化
	playerHPSprite_->Initialize();
	playerHPSprite_->Create(PLAYER_HP_X, PLAYER_HP_Y);
	playerHPSprite_->SetAncP({ 0,0 });


	bossHPSprite_->Initialize();
	bossHPSprite_->SetAncP({ 0,0 });
	bossHPSprite_->Create(275, 55);

	bossMaxHPSprite_->Initialize();
	bossMaxHPSprite_->SetAncP({ 0,0 });
	bossMaxHPSprite_->Create(BOSS_HP_X, BOSS_HP_Y);

	bossHPUISprite_->Initialize();
	bossHPUISprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	bossHPUISprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	bossHPUISprite_->Update();

	gameoverSprite_->Initialize();
	gameoverSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	gameoverSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	gameoverSprite_->Update();

	gameclearSprite_->Initialize();
	gameclearSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_X);
	gameclearSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	gameclearSprite_->Update();

	reticleSprite_->Initialize();
	reticleSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	reticleSprite_->SetSize({ 0,0 });

	sceneSprite_->Initialize();
	sceneSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	sceneSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	sceneSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,MAX_ALPHA });

	fillSprite_->Initialize();
	fillSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	fillSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	fillSprite_->Update();

	dFilterSprite_->Initialize();
	dFilterSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	dFilterSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	dFilterSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,0 });
	dFilterSprite_->Update();

	RBSprite_->Initialize();
	RBSprite_->Create(0, 0);
	RBSprite_->SetSize({ RB_UI_SIZE,RB_UI_SIZE });

	iventSprite_->Initialize();
	iventSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	iventSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	iventSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,0 });
	iventSprite_->Update();

	waringSprite_->Initialize();
	waringSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	waringSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	waringSprite_->Update();

	bulletRreticleSprite_->Initialize();
	bulletRreticleSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	bulletRreticleSprite_->SetSize({ PLAYER_BULLETRETICLE_SIZE,PLAYER_BULLETRETICLE_SIZE });
	bulletRreticleSprite_->Update();

	StepFilterSprite_->Initialize();
	StepFilterSprite_->Create(HALF_SCREEN_SIZE_X, HALF_SCREEN_SIZE_Y);
	StepFilterSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	StepFilterSprite_->Update();


}

//タイトルシーンUI更新処理
void GameUI::TitleUpdate(bool sceneChange)
{
	
	titleSprite_->Update();
	titleUISprite_->Update();
	sceneSprite_->Update();

	if (sceneChange) {
		ChangeTitleAlpha_ += CHANGE_TITLE_ALPHA_VOLUE;
		ChangeTitleAlpha_ = min(ChangeTitleAlpha_, MAX_ALPHA);
		sceneSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE, COLOR_WIHTE, ChangeTitleAlpha_ });
	}
	else {
		ChangeTitleAlpha_ = 0;
		sceneSprite_->SetColor({ COLOR_WIHTE, COLOR_WIHTE, COLOR_WIHTE, ChangeTitleAlpha_ });
	}
}

//ゲームシーンUI更新処理
void GameUI::GameUpdate()
{
	//stepAlpha_
	if (player_->GetIsJustStep()) {
		stepFillTimer_++;

		if (stepFillTimer_ < 10) {
			stepAlpha_ += 0.15f;
			stepAlpha_ = min(stepAlpha_, MAX_ALPHA);
		}
	}
	else {
		stepFillTimer_ = 0;
		stepAlpha_ -= 0.1f;
		stepAlpha_ = max(stepAlpha_, 0);
	}

	StepFilterSprite_->SetColor({ 1,1,1,stepAlpha_ });
	StepFilterSprite_->Update();


	iventSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,iventAlpha_ });
	playerHPSprite_->SetSize({ PLAYER_HP_SIZE_X * (float)player_->GetHP(),PLAYER_HP_SIZE_Y });

	//チュートリアルUI
	RBSprite_->SetPosition({ screenPosPlayer_.x - RB_UI_POS_X_VOLUE,screenPosPlayer_.y - RB_UI_POS_Y_VOLUE });

	if (!isManual_) {
		alphaRB_ += RB_ALPHA_VOLUE;
		alphaRB_ = min(alphaRB_, MAX_ALPHA);
		RBSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,alphaRB_ });
	}
	else {
		alphaRB_ -= RB_ALPHA_VOLUE;
		alphaRB_ = max(alphaRB_, 0);
		RBSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,alphaRB_ });
	}

	//弾を打ったときの左右レティクル
	if (gamePad_->PushButtonRB()) {
		isManual_ = true;

		reticleSize_ = reticleSprite_->GetSize();
		reticleSize_.x += SIDE_RETICLE_SIZE_VOLUE_X;
		reticleSize_.y += SIDE_RETICLE_SIZE_VOLUE_Y;
		reticleSize_.x = min(reticleSize_.x, SCREEN_SIZE_X);
		reticleSize_.y = min(reticleSize_.y, SCREEN_SIZE_Y);
	}
	else {
		reticleSize_ = reticleSprite_->GetSize();
		reticleSize_.x -= SIDE_RETICLE_SIZE_VOLUE_X;
		reticleSize_.y -= SIDE_RETICLE_SIZE_VOLUE_Y;
		reticleSize_.x = max(reticleSize_.x, 0);
		reticleSize_.y = max(reticleSize_.y, 0);
	}

	reticleSprite_->SetSize(reticleSize_);
	
	//プレイヤーの3D座標をスクリーン変換した座標
	screenPosPlayer_ = playerObject_->Screen();

	//プレイヤーのレティクル座標
	bulletRreticleSprite_->SetPosition({ player_->GetScreenRTPos().x,player_->GetScreenRTPos().y });
	
	//ライフ危ない時に出るフィルター
	if (player_->GetHP() <= PLAYER_HP_DANGER) {
		isLifeOne_ = true;
	}
	else {
		isLifeOne_ = false;
	}
	//ライフ危ない時に出るフィルター
	if (isLifeOne_) {
		fillTimer_++;

		if (fillTimer_ < FILLTER_TIMER_FRAME) {
			fillAlpha_ += FILLTER_ALPHA_ADD_VOLUE;
			fillAlpha_ = min(fillAlpha_, MAX_ALPHA);
		}
		else {
			fillAlpha_ -= FILLTER_ALPHA_SUB_VOLUE;
			fillAlpha_ = max(fillAlpha_, 0);
		}
		dFilterSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,fillAlpha_ });

	}
	else {
		fillTimer_ = 0;
	}


	if (hitBox_ && !isIvent_) {
		if (count_ < BLINKING_COUNT) {
			pow_++;
			wSize_.y = waringSprite_->GetSize().y;
		}
		else {
			pow_ = 0;
		}

		//ワーニング点滅
		if (pow_ > WARING_FRAME) {
			pow_ = 0;
			count_++;
		}

		//点滅後の動き
		if (count_ == BLINKING_COUNT) {
			popFrame_++;

			if (popFrame_ > POP_FRAME_MAX) {
				wTimer_++;
				wTimer_ = min(wTimer_, wMax_);

				wSize_ = wSize_.lerp({ SCREEN_SIZE_X,SCREEN_SIZE_Y,0 }, { SCREEN_SIZE_X,0,0 }, Easing::EaseOutCubic(wTimer_, wMax_));

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

//タイトルシーンUI描画関数
void GameUI::TitleDraw()
{
	titleSprite_->Draw(titleImage_);
	titleUISprite_->Draw(titleUIImage_);
	sceneSprite_->Draw(sceneChangeImage_);
}

//ゲームシーンUI描画関数
void GameUI::GameDraw()
{
	StepFilterSprite_->Draw(stepFilterImage_);

	//ボスのイベントムービー中は非表示
	if (!isIvent_) {
		playerHPSprite_->Draw(playerHP_);
		reticleSprite_->Draw(reticleImage_);
		bulletRreticleSprite_->Draw(bulletRreticleImage_);
	}

	//ボスが現れたら表示
	if (hitBox_ == true && boss_->GetArive() == true && isIvent_ == false) {
		bossMaxHPSprite_->Draw(bossMaxHP_);
		bossHPSprite_->Draw(bossHP_);
		bossHPUISprite_->Draw(bossHPImage_);
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

//リセット関数
void GameUI::Reset()
{
	ChangeGameAlpha_ = MAX_ALPHA;
	pow_ = 0;
	count_ = 0;
	waringSprite_->SetSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
}

//ポストエフェクトをかけないスプライトの描画
void GameUI::OffDraw()
{
	fillSprite_->Draw(filterImage_);
}

//ダメージシェイク時の処理
void GameUI::Shake()
{
	randShake_.x = MyRandom::GetFloatRandom(SHACK_MIN, SHACK_MAX);
	randShake_.y = MyRandom::GetFloatRandom(SHACK_MIN, SHACK_MAX);

	playerHPSprite_->SetPosition({ PLAYER_HP_X + randShake_.x * SHACK_RATE,PLAYER_HP_Y + randShake_.y * SHACK_RATE });
	bossHPSprite_->SetPosition({ BOSS_HP_X + randShake_.x * SHACK_RATE,BOSS_HP_Y + randShake_.y * SHACK_RATE });
	bossMaxHPSprite_->SetPosition({ BOSS_HP_X + randShake_.x * SHACK_RATE,BOSS_HP_Y + randShake_.y * SHACK_RATE });
}

//BossHPUIの更新処理
void GameUI::BossHpUI()
{
	bossHPSprite_->SetSize({ BOSS_HP_SIZE_X * (float)boss_->GetHP(),BOSS_HP_SIZE_Y });
	bossHPSprite_->Update();
	bossMaxHPSprite_->SetSize({ BOSS_HP_SIZE_X * 50,BOSS_HP_SIZE_Y });
	bossMaxHPSprite_->Update();

	bossHPUISprite_->Update();
}

//ゲームシーンのシーン遷移のフェード用関数
void GameUI::SceneStartFadeUI() {
	ChangeGameAlpha_ -= CHANGE_GAME_ALPHA_VOLUE;
	ChangeGameAlpha_ = max(ChangeGameAlpha_, 0);
	sceneSprite_->SetColor({ COLOR_WIHTE,COLOR_WIHTE,COLOR_WIHTE,ChangeGameAlpha_ });
}

//ボス戦のイベントムービー用の関数
void GameUI::BossIventSceneUI() {
	if (isIvent_) {
		iventAlpha_ += EVENT_MOVIE_ALPHA_VOLUE;
		iventAlpha_ = min(iventAlpha_, MAX_ALPHA);
	}
	else {
		iventAlpha_ -= EVENT_MOVIE_ALPHA_VOLUE;
		iventAlpha_ = max(iventAlpha_, 0);
	}
}