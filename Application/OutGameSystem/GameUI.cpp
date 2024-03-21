#include "GameUI.h"
#include "MyRandom.h"
#include "Easing.h"

//コンストラクタ
GameUI::GameUI()
{
	//jsonファイルから定数を読み込み
	json_ = std::make_unique<constJsonValue>();
	json_->LoadConstValue("Resources/json/gameUIConst.json");
}

//デストラクタ
GameUI::~GameUI()
{
}

//借りてくる情報
void GameUI::SetInfo(Object3D* playerObject, Boss* boss)
{
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
	titleSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"),json_->LoadFloat("SCREEN_SIZE_Y") });
	titleSprite_->SetAncP({ 0,0 });

	titleUISprite_->Initialize(titleUIImage_);
	titleUISprite_->Create();
	titleUISprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"),json_->LoadFloat("SCREEN_SIZE_Y") });
	titleUISprite_->SetAncP({ 0,0 });

	sceneSprite_->Initialize();
	sceneSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	sceneSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"),json_->LoadFloat("SCREEN_SIZE_Y") });
	sceneSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),0 });

	fillSprite_->Initialize();
	fillSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	fillSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y") });
	fillSprite_->Update();
}

//ゲームシーンUI初期化
void GameUI::GameSceneInitialize()
{
	//リソースを読み込む
	TextureManager::LoadGameTexture();

	//スプライトの初期化
	playerHPSprite_->Initialize();
	playerHPSprite_->Create(json_->LoadFloat("PLAYER_HP_X"), json_->LoadFloat("PLAYER_HP_Y"));
	playerHPSprite_->SetAncP({ 0,0 });

	bossHPSprite_->Initialize();
	bossHPSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("BOSS_HP_Y"));

	gameoverSprite_->Initialize();
	gameoverSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	gameoverSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"),json_->LoadFloat("SCREEN_SIZE_Y") });
	gameoverSprite_->Update();

	gameclearSprite_->Initialize();
	gameclearSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	gameclearSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y") });
	gameclearSprite_->Update();

	reticleSprite_->Initialize();
	reticleSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	reticleSprite_->SetSize({ 0,0 });

	sceneSprite_->Initialize();
	sceneSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	sceneSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y") });
	sceneSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("MAX_ALPHA") });

	fillSprite_->Initialize();
	fillSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	fillSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y") });
	fillSprite_->Update();

	dFilterSprite_->Initialize();
	dFilterSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	dFilterSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y") });
	dFilterSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),0 });
	dFilterSprite_->Update();

	RBSprite_->Initialize();
	RBSprite_->Create(0, 0);
	RBSprite_->SetSize({ json_->LoadFloat("RB_UI_SIZE"),json_->LoadFloat("RB_UI_SIZE")});

	iventSprite_->Initialize();
	iventSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	iventSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y") });
	iventSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),0 });
	iventSprite_->Update();

	waringSprite_->Initialize();
	waringSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	waringSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y") });
	waringSprite_->Update();

	bulletRreticleSprite_->Initialize();
	bulletRreticleSprite_->Create(json_->LoadFloat("HALF_SCREEN_SIZE_X"), json_->LoadFloat("HALF_SCREEN_SIZE_Y"));
	bulletRreticleSprite_->SetSize({ json_->LoadFloat("PLAYER_BULLETRETICLE_SIZE"),json_->LoadFloat("PLAYER_BULLETRETICLE_SIZE") });
	bulletRreticleSprite_->Update();

}

//タイトルシーンUI更新処理
void GameUI::TitleUpdate(bool sceneChange)
{
	titleSprite_->Update();
	titleUISprite_->Update();
	sceneSprite_->Update();

	if (sceneChange) {
		ChangeTitleAlpha_ += json_->LoadFloat("CHANGE_TITLE_ALPHA_VOLUE");
		ChangeTitleAlpha_ = min(ChangeTitleAlpha_, json_->LoadFloat("MAX_ALPHA"));
		sceneSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"), json_->LoadFloat("COLOR_WIHTE"),  json_->LoadFloat("COLOR_WIHTE"), ChangeTitleAlpha_ });
	}
	else {
		ChangeTitleAlpha_ = 0;
		sceneSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),  json_->LoadFloat("COLOR_WIHTE"),  json_->LoadFloat("COLOR_WIHTE"), ChangeTitleAlpha_ });
	}
}

//ゲームシーンUI更新処理
void GameUI::GameUpdate()
{
	iventSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"), json_->LoadFloat("COLOR_WIHTE"), json_->LoadFloat("COLOR_WIHTE"),iventAlpha_ });

	//チュートリアルUI
	RBSprite_->SetPosition({ screenPosPlayer_.x - json_->LoadFloat("RB_UI_POS_X_VOLUE"),screenPosPlayer_.y - json_->LoadFloat("RB_UI_POS_Y_VOLUE") });

	if (!isManual_) {
		alphaRB_ += json_->LoadFloat("RB_ALPHA_VOLUE");
		alphaRB_ = min(alphaRB_, json_->LoadFloat("MAX_ALPHA"));
		RBSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),alphaRB_ });
	}
	else {
		alphaRB_ -= json_->LoadFloat("RB_ALPHA_VOLUE");
		alphaRB_ = max(alphaRB_, 0);
		RBSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),alphaRB_ });
	}

	//弾を打ったときの左右レティクル
	if (gamePad_->PushButtonRB()) {
		isManual_ = true;

		reticleSize_ = reticleSprite_->GetSize();
		reticleSize_.x += json_->LoadFloat("SIDE_RETICLE_SIZE_VOLUE_X");
		reticleSize_.y += json_->LoadFloat("SIDE_RETICLE_SIZE_VOLUE_Y");
		reticleSize_.x = min(reticleSize_.x, json_->LoadFloat("SCREEN_SIZE_X"));
		reticleSize_.y = min(reticleSize_.y, json_->LoadFloat("SCREEN_SIZE_Y"));
	}
	else {
		reticleSize_ = reticleSprite_->GetSize();
		reticleSize_.x -= json_->LoadFloat("SIDE_RETICLE_SIZE_VOLUE_X");
		reticleSize_.y -= json_->LoadFloat("SIDE_RETICLE_SIZE_VOLUE_Y");
		reticleSize_.x = max(reticleSize_.x, 0);
		reticleSize_.y = max(reticleSize_.y, 0);
	}

	reticleSprite_->SetSize(reticleSize_);
	
	//プレイヤーの3D座標をスクリーン変換した座標
	screenPosPlayer_ = playerObject_->Screen();

	//ライフ危ない時に出るフィルター
	if (isLifeOne_) {
		fillTimer_++;

		if (fillTimer_ < json_->LoadFloat("FILLTER_TIMER_FRAME")) {
			fillAlpha_ += json_->LoadFloat("FILLTER_ALPHA_ADD_VOLUE");
			fillAlpha_ = min(fillAlpha_, json_->LoadFloat("MAX_ALPHA"));
		}
		else {
			fillAlpha_ -= json_->LoadFloat("FILLTER_ALPHA_SUB_VOLUE");
			fillAlpha_ = max(fillAlpha_, 0);
		}
		dFilterSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),fillAlpha_ });

	}
	else {
		fillTimer_ = 0;
	}


	if (hitBox_ && !isIvent_) {
		if (count_ < json_->LoadInt("BLINKING_COUNT")) {
			pow_++;
			wSize_.y = waringSprite_->GetSize().y;
		}
		else {
			pow_ = 0;
		}

		//ワーニング点滅
		if (pow_ > json_->LoadInt("WARING_FRAME")) {
			pow_ = 0;
			count_++;
		}

		//点滅後の動き
		if (count_ == json_->LoadInt("BLINKING_COUNT")) {
			popFrame_++;

			if (popFrame_ > json_->LoadFloat("POP_FRAME_MAX")) {
				wTimer_++;
				wTimer_ = min(wTimer_, wMax_);

				wSize_ = wSize_.lerp({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y"),0 }, { json_->LoadFloat("SCREEN_SIZE_X"),0,0 }, Easing::EaseOutCubic(wTimer_, wMax_));

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
	/*if (player_->IsDead()) {
		gameoverSprite_->Draw(gameover_);
	}*/

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
	ChangeGameAlpha_ = json_->LoadFloat("MAX_ALPHA");
	pow_ = 0;
	count_ = 0;
	waringSprite_->SetSize({ json_->LoadFloat("SCREEN_SIZE_X"), json_->LoadFloat("SCREEN_SIZE_Y") });
}

//ポストエフェクトをかけないスプライトの描画
void GameUI::OffDraw()
{
	fillSprite_->Draw(filterImage_);
}

//ダメージシェイク時の処理
void GameUI::Shake()
{
	randShake_.x = MyRandom::GetFloatRandom(json_->LoadFloat("SHACK_MIN"), json_->LoadFloat("SHACK_MAX"));
	randShake_.y = MyRandom::GetFloatRandom(json_->LoadFloat("SHACK_MIN"), json_->LoadFloat("SHACK_MAX"));

	playerHPSprite_->SetPosition({ json_->LoadFloat("PLAYER_HP_X") + randShake_.x * json_->LoadFloat("SHACK_RATE"),json_->LoadFloat("PLAYER_HP_Y") + randShake_.y * json_->LoadFloat("SHACK_RATE") });
	bossHPSprite_->SetPosition({ json_->LoadFloat("HALF_SCREEN_SIZE_X") + randShake_.x * json_->LoadFloat("SHACK_RATE"),json_->LoadFloat("BOSS_HP_Y") + randShake_.y * json_->LoadFloat("SHACK_RATE") });
}

//BossHPUIの更新処理
void GameUI::BossHpUI()
{
	bossHPSprite_->SetSize({ json_->LoadFloat("BOSS_HP_SIZE_X") * (float)boss_->GetHP(),json_->LoadFloat("BOSS_HP_SIZE_Y") });
	bossHPSprite_->Update();
}

//ゲームシーンのシーン遷移のフェード用関数
void GameUI::SceneStartFadeUI() {
	ChangeGameAlpha_ -= json_->LoadFloat("CHANGE_GAME_ALPHA_VOLUE");
	ChangeGameAlpha_ = max(ChangeGameAlpha_, 0);
	sceneSprite_->SetColor({ json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),json_->LoadFloat("COLOR_WIHTE"),ChangeGameAlpha_ });
}

//ボス戦のイベントムービー用の関数
void GameUI::BossIventSceneUI() {
	if (isIvent_) {
		iventAlpha_ += json_->LoadFloat("EVENT_MOVIE_ALPHA_VOLUE");
		iventAlpha_ = min(iventAlpha_, json_->LoadFloat("MAX_ALPHA"));
	}
	else {
		iventAlpha_ -= json_->LoadFloat("EVENT_MOVIE_ALPHA_VOLUE");
		iventAlpha_ = max(iventAlpha_, 0);
	}
}