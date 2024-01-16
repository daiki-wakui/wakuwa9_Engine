#include "GameScene.h"
#include "SoundManager.h"
#include "MyRandom.h"
#include "Easing.h"
#include "Model3DManager.h"

//初期化
void GameScene::Initialize()
{
	//3Dモデルの読み込みと生成
	Object3DGenerate();

	eventBox_ = std::make_unique<EventBox>();
	ChangeBox_ = std::make_unique<EventBox>();


	lv->Initialize();
	Reset();

	player_->Initialize(m->Get3DModel("player"), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
	player_->SetBulletModel(m->Get3DModel("whiteCube"), bulletObject_.get());

	particleMan_->Initialize(L"Resources/effect1.png");
	particleMan_->Update();

	resetOn_ = false;

	gameUI_->GameSceneInitialize();
}

//後始末
void GameScene::Finalize()
{
	models.clear();
	objects.clear();
	collisions_.clear();
	delete lv;
}

//更新処理
void GameScene::Update()
{

	if (isShake_) {
		shakeTimer_++;

		if (shakeTimer_ > MAX_SHACK_TIME) {
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

		EditorLoad("obj");
		SoundManager::GetInstance()->SetFiledBGM(true);

		isChangeStage_ = false;
	}

	if (gameUI_->GetGameSceneChangeAlpha() == 0) {
		SoundManager::GetInstance()->Update(1);
	}

	ObjectUpdate();

	//スプライト更新処理
	SpriteUpdate();

	shadowObject_->SetPosition({ playerObject_->GetPosition().x,SHADOW_POS_Y,playerObject_->GetPosition().z });
	shadowObject_->Update();

	//弾の更新処理
	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Update();
	}

	//デスフラグが立った弾を削除
	effects_.remove_if([](std::unique_ptr<Effect>& bullet) {
		return bullet->IsDead();
	});
	
	Vector3 rocalEye = Object3D::GetEye();
	Vector3 rocalTarget = Object3D::GetTarget();

	if (change_) {
		SoundManager::GetInstance()->StopBGM();
	}

	//ゲーム画面
	if (eventBox_->GetIsArive()) {
		eventBox_->Update();
	}
	if (ChangeBox_->GetIsArive()) {
		ChangeBox_->Update();
	}
	
	if (dPoint_->GetIsArive()) {
		dPoint_->Update();
	}
	
	/*for (auto& object : objects) {
		object->Update();
	}*/

	//player_->Update();
	//if (player_->GetIsShot()) {

	//	SoundManager::GetInstance()->PlayWave("Shot.wav", BULLET_SHOT_VOLUE);
	//	player_->SetIsShot(false);
	//}

	//player_->SetEnemy(boss_.get());

	Vector3 podPos;

	if (coolTime_ < 0) {
		isShotEffect_ = true;
		coolTime_ = COOLTIME_NUM;
	}

	if (gamePad_->PushButtonRB()) {
		podRot.y = podObject_->GetRotation().y - BIT_ROT_VOLUE_Y;
		podRot.z += BIT_ROT_VOLUE_Z;
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

		if (timer_ > EVENT_TIME_FRAME) {

			if (!bossBGM_) {
				SoundManager::GetInstance()->Update(2);
				bossBGM_ = true;
			}
		}

		//イベントシーン終わり
		if (timer_ > maxTime_) {
			isIvent_ = false;
			Vector3 eye = LNIT_EYE;

			Object3D::SetEye(eye);
			eye = LNIT_TERGET;
			Object3D::SetTarget(eye);
			iventEye_ = LNIT_EVENT_EYE;
			gameUI_->SetMovieEnd(true);
			SoundManager::GetInstance()->PlayWave("Warning.wav", WARNING_VOLUE);
		}
	}
	//AllCollison();
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
	particleMan_->Update();

	//倒した時のオブジェクトエフェクト
	if (keyboard_->keyInstantPush(DIK_K)) {
		if (!isEffect_) {
			isEffect_ = true;
		}
	}

	if (isEffect_) {
		int size = 0;
		//弾の生成と初期化

		Vector3 v = { 0,EFFECT_Y,0 };

		for (int i = 0; i < EFFECT_NUM; i++) {

			std::unique_ptr<Effect> newObj = std::make_unique<Effect>();
			newObj->Initialize(startEffect_, v, m->Get3DModel("effctModel"));
			effects_.push_back(std::move(newObj));
			size++;
		}

		if (size == EFFECT_NUM) {
			isEffect_ = false;
		}
	}

	skyObject_->SetPosition({0,0,0});
	skyObject_->Update();

	lv->Update();
}

//描画関数
void GameScene::Draw()
{
	lv->Darw();

	shadowObject_->Draw();

	

	//obj
	skyObject_->Draw();
	
	//弾の更新処理
	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Draw();
	}

	if (isShotEffect_) {
		poriObject_->Draw();
		isShotEffect_ = false;
	}

	SpriteDraw();
}

//パーティクル描画
void GameScene::ParticleDraw()
{
	particleMan_->Draw();

	if (!isIvent_) {
		player_->ParticleDraw();
	}
}

//フィールド情報読み込み
void GameScene::EditorLoad(const std::string filename)
{

}

//フィールド情報再読み込み
void GameScene::ReLoad(const std::string filename)
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

	for (std::unique_ptr<Door>& door : doors_) {
		door->SetTutorial(true);
	}

	gameUI_->Reset();
}

//当たり判定関数
bool GameScene::Collison(Vector3 posa, Vector3 posb, float aScale, float bScale)
{
	float r = aScale + bScale;

	Vector3 dis;
	dis.x = posb.x - posa.x;
	dis.y = posb.y - posa.y;
	dis.z = posb.z - posa.z;

	if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && player_->GetHP() > 0) {
		return true;
	}

	return false;
}

//ゲームシーン上の当たり判定
void GameScene::AllCollison()
{
	//当たり判定
	Vector3 posA, posB;
	float r1;	//イベントのスケール
	float r2;	//自機のスケール

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	posA = player_->GetWorldPos();

	if (eventBox_->GetIsArive()) {
		posB = eventBox_->GetWorldPos();

		//AとBの距離
		r1 = COL_PLAYER_SCALE;	//イベントのスケール
		r2 = COL_EVENTBOX_SCALE;	//自機のスケール

		if (Collison(posA, posB, r1, r2)) {
			if (!isTutorial_) {
				hitBox_ = true;
			}
			else {
				isChangeStage_ = true;
			}
		}
	}

	if (ChangeBox_->GetIsArive()) {
		posB = ChangeBox_->GetWorldPos();

		//AとBの距離
		r1 = COL_PLAYER_SCALE;	//イベントのスケール
		r2 = COL_EVENTBOX_SCALE;	//自機のスケール

		if (Collison(posA, posB, r1, r2)) {
			if (ChangeBox_->GetLoadEditor()) {
				isChangeStage_ = true;
			}
		}
	}

	//自機の弾とボスの当たり判定
	if (boss_->GetArive()) {
		posA = boss_->GetWorldPos();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet->GetWorldPos();

			//AとBの距離
			r1 = COL_BOSS_SCALE;	//敵のスケール
			r2 = COL_BULLET_SCALE;	//弾のスケール

			if (Collison(posA, posB, r1, r2) && hitBox_) {
				bullet->isDead_ = true;
				boss_->OnCollision();

				SoundManager::GetInstance()->PlayWave("Hit.wav", HIT_SE_VOLUE);
			}
		}
	}

	posA = player_->GetWorldPos();

	//自機と敵の弾の当たり判定
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		for (const std::unique_ptr<EnemyBullet>& bullet : enemy->GetBullets()) {
			//敵弾の座標
			posB = bullet->GetWorldPos();

			//AとBの距離
			r1 = COL_PLAYER_SCALE;	//敵のスケール
			r2 = COL_BULLET_SCALE;	//弾のスケール

			if (Collison(posA, posB, r1, r2)) {
				bullet->isDead_ = true;

				if (!player_->Getinvincible()) {

					SoundManager::GetInstance()->PlayWave("noise.wav", NOISE_SE_VOLUE);

					isShake_ = true;
				}
				player_->OnCollision();

				if (player_->GetHP() <= 1) {
					SoundManager::GetInstance()->PlayWave("electric_shock3.wav", SHOCK_SE_VOLUE);
				}
			}
		}
	}

	posA = player_->GetWorldPos();
	posB = boss_->GetBossTailWorldPos();

	r1 = COL_PLAYER_SCALE;
	r2 = COL_BOSS_TAIL_SCALE;

	if (Collison(posA, posB, r1, r2)) {
		if (!player_->Getinvincible()) {

			SoundManager::GetInstance()->PlayWave("noise.wav", NOISE_SE_VOLUE);

			isShake_ = true;
		}
		player_->OnCollision();

	}

	//ボス弾と自機の当たり判定
	if (boss_->GetArive()) {
		for (const std::unique_ptr<BossBullet>& bullet : boss_->GetBullets()) {
			//敵弾の座標
			posB = bullet->GetWorldPos();

			//AとBの距離
			r1 = COL_PLAYER_SCALE;	//敵のスケール
			r2 = COL_BULLET_SCALE;	//弾のスケール

			if (Collison(posA, posB, r1, r2)) {
				bullet->isDead_ = true;

				if (!player_->Getinvincible()) {

					SoundManager::GetInstance()->PlayWave("noise.wav", NOISE_SE_VOLUE);

					isShake_ = true;
				}
				player_->OnCollision();

			}
		}
	}

	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		//AとBの距離
		r1 = COL_PLAYER_SCALE;	//自機のスケール
		r2 = COL_SEARCH_ENEMY_SCALE;	//索敵範囲のスケール

		posA = player_->GetWorldPos();
		posB = enemy->GetWorldPos();

		if (Collison(posA, posB, r1, r2)) {
			enemy->SetIsMove(true);
		}
	}

	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		posA = enemy->GetWorldPos();

		//自機の弾と敵の当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet->GetWorldPos();

			//AとBの距離
			r1 = COL_ENEMY_SCALE;	//敵のスケール
			r2 = COL_BULLET_SCALE;	//弾のスケール

			if (Collison(posA, posB, r1, r2)) {
				bullet->isDead_ = true;
				enemy->OnCollision();
				BulletEffect = true;
				SoundManager::GetInstance()->PlayWave("Hit.wav", HIT_SE_VOLUE);
			}

			if (enemy->IsDead()) {
				isEffect_ = true;
				startEffect_ = enemy->GetWorldPos();
			}
		}
	}
}

//3dオブジェクト生成
void GameScene::Object3DGenerate()
{
	shadowObject_ = std::make_unique<Object3D>();
	shadowObject_->SetModel(m->Get3DModel("shadow"));
	shadowObject_->Initialize();
	shadowObject_->SetScale(SHADOW_SCALE);

	poriObject_ = std::make_unique<Object3D>();
	poriObject_->SetModel(m->Get3DModel("bulletEffctModel"));
	poriObject_->Initialize();
	poriObject_->SetScale(SHOT_EFFECT_SCALE);
	poriObject_->SetPosition(SHOT_EFFECT_POS);

	//3Dオブジェクト生成
	playerObject_ = std::make_unique<Object3D>();
	playerObject_->SetModel(m->Get3DModel("player"));
	playerObject_->Initialize();
	playerObject_->SetScale(Vector3(PLAYER_SCALE));
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
	skyObject_->SetScale(SKY_SCALE);
	skyObject_->SetPosition(SKY_POS);

	bossObject_ = std::make_unique<Object3D>();
	bossObject_->SetModel(m->Get3DModel("bossBody"));
	bossObject_->Initialize();
	bossObject_->SetScale(BOSS_SCALE);
	bossObject_->SetPosition(BOSS_POS);
}
