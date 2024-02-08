#include "GameScene.h"
#include "SoundManager.h"
#include "MyRandom.h"
#include "Easing.h"
#include "Model3DManager.h"

//初期化
void GameScene::Initialize()
{
	json_ = std::make_unique<constJsonValue>();
	json_->LoadConstValue("Resources/json/gameSceneConst.json");

	//3Dモデルの読み込みと生成
	Object3DGenerate();
	lv->Initialize();
	Reset();

	player_->Initialize(m->Get3DModel("player"), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
	player_->SetBulletModel(m->Get3DModel("whiteCube"), bulletObject_.get());

	//particleMan_->Initialize(L"Resources/effect1.png");
	//particleMan_->Update();

	resetOn_ = false;

	gameUI_->GameSceneInitialize();
	col_->SetObjectList(lv.get());
}

//後始末
void GameScene::Finalize()
{

}

//更新処理
void GameScene::Update()
{

	if (isShake_) {
		shakeTimer_++;

		if (shakeTimer_ > json_->LoadFloat("MAX_SHACK_TIME")) {
			isShake_ = false;
			shakeTimer_ = 0;
		}
		gameUI_->Shake();

		Vector3 toEye = Object3D::GetEye();
		Vector3 toTerget = Object3D::GetTarget();
		toEye.x += gameUI_->GetRandShake().x;
		toEye.y += gameUI_->GetRandShake().y;
		toTerget.x += gameUI_->GetRandShake().x;
		toTerget.y += gameUI_->GetRandShake().y;

		Object3D::SetEye(toEye);
		Object3D::SetTarget(toTerget);
	}

	if (keyboard_->keyInstantPush(DIK_N)) {
		isDebugBoss_ = true;
	}

	//リセット
	if (isChangeStage_) {
		bossBGM_ = false;
		hitBox_ = false;

		lv->EditorLoad("obj");
		SoundManager::GetInstance()->SetFiledBGM(true);

		isChangeStage_ = false;
	}

	if (gameUI_->GetGameSceneChangeAlpha() == 0) {
		SoundManager::GetInstance()->Update(1);
	}

	ObjectUpdate();

	//スプライト更新処理
	SpriteUpdate();

	shadowObject_->SetPosition({ playerObject_->GetPosition().x,json_->LoadFloat("SHADOW_POS_Y"),playerObject_->GetPosition().z});
	shadowObject_->Update();
	
	Vector3 rocalEye = Object3D::GetEye();
	Vector3 rocalTarget = Object3D::GetTarget();

	if (change_) {
		SoundManager::GetInstance()->StopBGM();
	}

	Vector3 podPos;

	if (coolTime_ < 0) {
		isShotEffect_ = true;
		coolTime_ = json_->LoadInt("COOLTIME_NUM");
	}

	if (gamePad_->PushButtonRB()) {
		podRot.y = podObject_->GetRotation().y - json_->LoadFloat("BIT_ROT_VOLUE_Y");
		podRot.z += json_->LoadFloat("BIT_ROT_VOLUE_Z");
		coolTime_--;
	}

	podPos = podObject_->GetPosition();
	
	poriObject_->SetPosition(podPos);
	poriObject_->SetRotation(podRot);
	poriObject_->Update();

	if (isDebugBoss_) {
		isIvent_ = false;
		hitBox_ = true;
	}
	
	gameUI_->BossIventSceneUI();

	if (isIvent_) {
		timer_++;
		iventEye_ = iventEye_.lerp(iventEye_, endEye_, Easing::EaseInCubic(timer_, maxTime_));

		Vector3 tmpEye;
		tmpEye = iventEye_;
		Vector3 target;
		target = iventTarget_;

		Object3D::SetEye(tmpEye);
		Object3D::SetTarget(target);

		if (timer_ > json_->LoadFloat("EVENT_TIME_FRAME")) {

			if (!bossBGM_) {
				SoundManager::GetInstance()->Update(2);
				bossBGM_ = true;
			}
		}

		//イベントシーン終わり
		if (timer_ > maxTime_) {
			isIvent_ = false;
			Vector3 eye = json_->LoadVector3("LNIT_EYE");

			Object3D::SetEye(eye);
			eye = json_->LoadVector3("LNIT_TERGET");
			Object3D::SetTarget(eye);
			iventEye_ = json_->LoadVector3("LNIT_EVENT_EYE");
			gameUI_->SetMovieEnd(true);
			SoundManager::GetInstance()->PlayWave("Warning.wav", json_->LoadFloat("WARNING_VOLUE"));
		}
	}
}

//スプライト更新処理
void GameScene::SpriteUpdate()
{
	gameUI_->SetInfo(player_.get(),playerObject_.get(),boss_.get());
	gameUI_->boolInfo(hitBox_, isIvent_);
	gameUI_->GameUpdate();

	//シーン切り替え
	if (start_) {

		gameUI_->SceneStartFadeUI();

		if (!resetOn_) {
			resetOn_ = true;
			Reset();
		}
	}

	//ボスHP、BGM鳴らし
	if (hitBox_ == true) {

		if (!bossBGM_) {
			SoundManager::GetInstance()->StopBGM();
			isIvent_ = true;
		}

		if (hitBox_ == true && boss_->GetArive() == true) {
			boss_->Update(isIvent_);
		}

		gameUI_->BossHpUI();
	}
}

//スプライト描画
void GameScene::SpriteDraw()
{
	gameUI_->GameDraw();
}

//オブジェクト更新処理
void GameScene::ObjectUpdate()
{
	//particleMan_->Update();

	//倒した時のオブジェクトエフェクト
	if (keyboard_->keyInstantPush(DIK_K)) {
		if (!isEffect_) {
			isEffect_ = true;
		}
	}

	skyObject_->SetPosition(playerObj_->GetWorldPos());
	skyObject_->Update();

	lv->Update();
	col_->Update();
}

//描画関数
void GameScene::Draw()
{
	lv->Darw();

	shadowObject_->Draw();
	//obj
	skyObject_->Draw();

	if (isShotEffect_) {
		poriObject_->Draw();
		isShotEffect_ = false;
	}

	//SpriteDraw();
}
void GameScene::ParticleDraw()
{
}
//シーンリセット関数
void GameScene::Reset()
{
	timer_ = 0;
	alpha_ = 0;
	bossBGM_ = false;
	hitBox_ = false;
	SoundManager::GetInstance()->SetVolue(0);
	SoundManager::GetInstance()->SetFiledBGM(false);

	lv->EditorLoad("d");

	gameUI_->Reset();

	playerObj_ = lv->GetPlayer();
}

//3dオブジェクト生成
void GameScene::Object3DGenerate()
{
	shadowObject_ = std::make_unique<Object3D>();
	shadowObject_->SetModel(m->Get3DModel("shadow"));
	shadowObject_->Initialize();
	shadowObject_->SetScale(json_->LoadVector3("SHADOW_SCALE"));

	poriObject_ = std::make_unique<Object3D>();
	poriObject_->SetModel(m->Get3DModel("bulletEffctModel"));
	poriObject_->Initialize();
	poriObject_->SetScale(json_->LoadVector3("SHOT_EFFECT_SCALE"));
	poriObject_->SetPosition(json_->LoadVector3("SHOT_EFFECT_POS"));

	//3Dオブジェクト生成
	playerObject_ = std::make_unique<Object3D>();
	playerObject_->SetModel(m->Get3DModel("player"));
	playerObject_->Initialize();
	playerObject_->SetScale(Vector3(json_->LoadVector3("PLAYER_SCALE")));
	playerObject_->SetPosition({ 0,0,0 });

	podObject_ = std::make_unique<Object3D>();
	podObject_->SetModel(m->Get3DModel("playerBit"));
	podObject_->Initialize();

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(m->Get3DModel("whiteCube"));
	bulletObject_->Initialize();

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(m->Get3DModel("worldGame"));
	skyObject_->Initialize();
	skyObject_->SetScale(json_->LoadVector3("SKY_SCALE"));
	skyObject_->SetPosition(json_->LoadVector3("SKY_POS"));

	bossObject_ = std::make_unique<Object3D>();
	bossObject_->SetModel(m->Get3DModel("bossBody"));
	bossObject_->Initialize();
	bossObject_->SetScale(json_->LoadVector3("BOSS_SCALE"));
	bossObject_->SetPosition(json_->LoadVector3("BOSS_POS"));
}
