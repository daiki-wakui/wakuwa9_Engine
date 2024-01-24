#include "GameScene.h"
#include "SoundManager.h"
#include "MyRandom.h"
#include "Easing.h"

//初期化
void GameScene::Initialize()
{
	//3Dモデルの読み込みと生成
	Model3DManager::LoadGame3DModel();
	Object3DGenerate();

	eventBox_ = std::make_unique<EventBox>();
	ChangeBox_ = std::make_unique<EventBox>();

	Reset();

	player_->Initialize(playerModel_.get(), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
	player_->SetBulletModel(cubeModel_.get(), bulletObject_.get());

	particleMan_->Initialize(L"Resources/effect1.png");
	particleMan_->Update();

	resetOn_ = false;

	gameUI_->GameSceneInitialize();

	/*ff_->Initialize(Vector3{0,10,-300}, ffv_, poriModel_.get());
	ff_->SetScale(5.0f);*/
}

//後始末
void GameScene::Finalize()
{
	models.clear();
	objects.clear();
	collisions_.clear();
}

//更新処理
void GameScene::Update()
{

	popFTimer_++;
	if (popFTimer_ > 10) {
		ffpos_ = player_->GetWorldPos();

		ffpos_.x += -player_->GetFrontVec().x * MyRandom::GetFloatRandom(-800, 800);
		ffpos_.y += -player_->GetFrontVec().y * MyRandom::GetFloatRandom(-100, 800);
		ffpos_.z += -player_->GetFrontVec().z * MyRandom::GetFloatRandom( 100, 150);


		ffv_.x = MyRandom::GetFloatRandom(-0.1f, 0.1f);
		ffv_.y = MyRandom::GetFloatRandom(-0.1f, 0.1f);
		ffv_.z = MyRandom::GetFloatRandom(-0.1f, 0.1f);

		std::unique_ptr<FieldEffect> newFF = std::make_unique<FieldEffect>();
		newFF->Initialize(ffpos_,player_->GetRot(), ffv_, poriFModel_.get());
		fEffects_.push_back(std::move(newFF));
		popFTimer_ = 0;
	}

	//弾の更新処理
	for (std::unique_ptr<FieldEffect>& ff : fEffects_) {
		ff->Update();
	}

	//デスフラグが立った弾を削除
	fEffects_.remove_if([](std::unique_ptr<FieldEffect>& ff) {
		return ff->IsDead();
	});

	//ff_->Update();

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
	
	for (auto& object : objects) {
		object->Update();
	}

	player_->Update();
	if (player_->GetIsShot()) {

		SoundManager::GetInstance()->PlayWave("Shot.wav", BULLET_SHOT_VOLUE);
		player_->SetIsShot(false);
	}

	player_->SetEnemy(boss_.get());

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

	//enemyの死亡フラグ
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
	});

	//敵の動き
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update(start_);
	}

	for (std::unique_ptr<CollisionBox>& collision : collisions_) {
		collision->Update();
	}

	for (std::unique_ptr<Door>& door : doors_) {
		door->Update();
	}

	AllCollison();
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
			newObj->Initialize(startEffect_, v, konnpeModel_.get());
			effects_.push_back(std::move(newObj));
			size++;
		}

		if (size == EFFECT_NUM) {
			isEffect_ = false;
		}
	}

	skyObject_->SetPosition(player_->GetWorldPos());
	skyObject_->Update();
}

//描画関数
void GameScene::Draw()
{
	
	shadowObject_->Draw();

	for (auto& object : objects) {
		object->Draw();
	}

	for (std::unique_ptr<Door>& door : doors_) {
		door->Draw();
	}

	if (player_->IsDead() == false && isIvent_ == false) {
		player_->Draw();
	}

	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		if (enemy->GetIsMove()) {
			enemy->Draw();
		}
	}

	if (hitBox_ == true && boss_->GetArive() == true) {
		boss_->Draw();
	}

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

	for (std::unique_ptr<FieldEffect>& ff : fEffects_) {
		ff->Draw();
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
	objects.clear();
	enemys_.clear();
	doors_.clear();
	ReLoad(filename);
}

//フィールド情報再読み込み
void GameScene::ReLoad(const std::string filename)
{
	// レベルデータの読み込み
	levelData_ = LevelLoader::LoadFile(filename);

	models.insert(std::make_pair(std::string("player"), playerModel_.get()));
	models.insert(std::make_pair(std::string("debugpoint"), playerModel_.get()));
	models.insert(std::make_pair(std::string("boss"), bossModel_.get()));
	models.insert(std::make_pair(std::string("enemySpawn"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("enemyc"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("enemySpawn2"), enemyModel2_.get()));
	models.insert(std::make_pair(std::string("filed"), filedModel_.get()));
	models.insert(std::make_pair(std::string("IventBlock"), cubeModel_.get()));
	models.insert(std::make_pair(std::string("changeBlock"), cubeModel_.get()));
	models.insert(std::make_pair(std::string("FliedBlock"), filedCubeModel_.get()));
	models.insert(std::make_pair(std::string("wallBlock"), cubeModel_.get()));
	models.insert(std::make_pair(std::string("FliedT"), filedTentoModel_.get()));
	models.insert(std::make_pair(std::string("Fliedtou"), filedTouModel_.get()));
	models.insert(std::make_pair(std::string("dr"), drModel_.get()));
	models.insert(std::make_pair(std::string("d"), LeftDoorModel_.get()));
	models.insert(std::make_pair(std::string("bossf"), bossFiledModel_.get()));
	models.insert(std::make_pair(std::string("bossf2"), bossFiledGateModel_.get()));

	// レベルデータからオブジェクトを生成、配置
	for (int32_t i = 0; i < levelData_->objects.size(); i++) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(levelData_->objects[i].fileName);
		if (it != models.end()) {
			model = it->second;
		}

		//対応するモデルがなければ飛ばす
		if (model == nullptr) {
			continue;
		}
		//blender上のカメラセット
		if (levelData_->objects[i].fileName == "camera") {

			Vector3 eyeVec;
			DirectX::XMFLOAT3 eye;
			DirectX::XMStoreFloat3(&eye, levelData_->objects[i].translation);

			eyeVec.x = eye.x;
			eyeVec.y = eye.y;
			eyeVec.z = eye.z;
			Object3D::CameraMoveVector(eyeVec);

			continue;
		}



		if (levelData_->objects[i].fileName == "player") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			playerObject_->Initialize();
			playerObject_->SetScale(PLAYER_SCALE);
			playerObject_->SetPosition(newObject[objSize_]->GetPosition());
			playerObject_->SetRotation(newObject[objSize_]->GetRotation());
			playerObject_->SetCamera(LNIT_EYE, LNIT_TERGET);

			player_.reset();
			player_ = std::make_unique<Player>();
			player_->SetBulletModel(playerBulletCubeModel_.get(), bulletObject_.get());
			player_->Initialize(playerModel_.get(), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
		}
		else if (levelData_->objects[i].fileName == "debugpoint") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			dPoint_->Initialize(model, newObject[objSize_].get());
			objSize_++;
		}
		else if (levelData_->objects[i].fileName == "dr") {
			//オブジェクト生成と座標情報代入
 			Inport(model, i);
			// 座標
			DirectX::XMFLOAT3 rocalPos;
			DirectX::XMStoreFloat3(&rocalPos, levelData_->objects[i].translation);
			newObject[objSize_]->SetPosition({ rocalPos.x+ DOOR_POS_VOLUE_X,rocalPos.y,rocalPos.z });

			newDoor[doorCount_] = std::make_unique<Door>();
			newDoor[doorCount_]->Initialize(model, newObject[objSize_].get());
			doors_.push_back(std::move(newDoor[doorCount_]));


			objSize_++;
			doorCount_++;
		}
		else if (levelData_->objects[i].fileName == "d") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);
			// 座標
			DirectX::XMFLOAT3 rocalPos;
			DirectX::XMStoreFloat3(&rocalPos, levelData_->objects[i].translation);
			newObject[objSize_]->SetPosition({ rocalPos.x,rocalPos.y,rocalPos.z });


			newDoor[doorCount_] = std::make_unique<Door>();
			newDoor[doorCount_]->Initialize(model, newObject[objSize_].get(), true);
			doors_.push_back(std::move(newDoor[doorCount_]));

			objSize_++;
			doorCount_++;
		}
		//eventボックスの配置
		else if (levelData_->objects[i].fileName == "IventBlock") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			eventBox_->Initialize(model, newObject[objSize_].get());
			objSize_++;
		}
		else if (levelData_->objects[i].fileName == "changeBlock") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			ChangeBox_->Initialize(model, newObject[objSize_].get(), true);
			objSize_++;
		}
		else if (levelData_->objects[i].fileName == "wallBlock") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			//オブジェクト生成と座標情報代入
			collBox[collSize_] = std::make_unique<CollisionBox>();

			collBox[collSize_]->Initialize(model, newObject[objSize_].get());
			collBox[collSize_]->SetScale(newObject[objSize_]->GetScale());

			collisions_.push_back(std::move(collBox[collSize_]));

			objSize_++;
			collSize_++;
		}
		else if (levelData_->objects[i].fileName == "enemySpawn") {

			//オブジェクト生成と座標情報代入
			Inport(model, i);

			//オブジェクト生成と座標情報代入
			newEnemy[enemySize_] = std::make_unique<Enemy>();

			newEnemy[enemySize_]->SetShadow(shadowModel_.get());
			newEnemy[enemySize_]->Initialize(newObject[objSize_].get(), newObject[objSize_]->GetPosition(), player_.get());
			newEnemy[enemySize_]->SetBulletModel(bossBulletModel_.get());
			


			//敵を登録する
			enemys_.push_back(std::move(newEnemy[enemySize_]));

			objSize_++;
			enemySize_++;
		}
		else if (levelData_->objects[i].fileName == "enemyc") {

			//オブジェクト生成と座標情報代入
			Inport(model, i);

			//オブジェクト生成と座標情報代入
			newEnemy[enemySize_] = std::make_unique<Enemy>();

			newEnemy[enemySize_]->SetShadow(shadowModel_.get());
			newEnemy[enemySize_]->Initialize(newObject[objSize_].get(), newObject[objSize_]->GetPosition(), player_.get(),4,0);
			newEnemy[enemySize_]->SetBulletModel(cubeModel_.get());

			//敵を登録する
			enemys_.push_back(std::move(newEnemy[enemySize_]));

			objSize_++;
			enemySize_++;
		}
		else if (levelData_->objects[i].fileName == "boss") {
			//オブジェクト生成と座標情報代入
 			Inport(model, i);


			newObject[objSize_]->SetScale(BOSS_SCALE);
			boss_->Initialize(model,newObject[objSize_]->GetPosition(), newObject[objSize_].get(), player_.get());
			boss_->SetBulletModel(bossBulletModel_.get());
			boss_->SetBossModels(frameModel_.get());

			objSize_++;
		}
		else {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			// 配列に登録
			objects.push_back(newObject[objSize_].get());
			objSize_++;
		}

	}
}

//オブジェクト配置時の情報
void GameScene::Inport(Model* model, int32_t size)
{
	newObject[objSize_] = std::make_unique<Object3D>();
	newObject[objSize_]->SetModel(model);
	newObject[objSize_]->Initialize();

	// 座標
	DirectX::XMFLOAT3 rocalPos;
	DirectX::XMStoreFloat3(&rocalPos, levelData_->objects[size].translation);
	newObject[objSize_]->SetPosition({ rocalPos.x,rocalPos.y, rocalPos.z });

	// 回転角
	DirectX::XMFLOAT3 rot;
	DirectX::XMStoreFloat3(&rot, levelData_->objects[size].rotation);
	newObject[objSize_]->SetRotation({ rot.x,rot.y,rot.z });


	// 座標
	DirectX::XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, levelData_->objects[size].scaling);
	newObject[objSize_]->SetScale({ scale.x,scale.y,scale.z });
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

	EditorLoad("d");

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

		if (player_->GetIsStep()) {
			player_->SetIsJustStep(true);
			player_->SetIsInvincible(true);
			player_->SetInvincibleFrame_(200);
		}

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

				if (player_->GetIsStep()) {
					player_->SetIsJustStep(true);
					player_->SetIsInvincible(true);
				}

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
	shadowObject_->SetModel(shadowModel_.get());
	shadowObject_->Initialize();
	shadowObject_->SetScale(SHADOW_SCALE);

	poriObject_ = std::make_unique<Object3D>();
	poriObject_->SetModel(poriModel_.get());
	poriObject_->Initialize();
	poriObject_->SetScale(SHOT_EFFECT_SCALE);
	poriObject_->SetPosition(SHOT_EFFECT_POS);

	ffo_ = std::make_unique<Object3D>();
	ffo_->SetModel(poriModel_.get());
	ffo_->Initialize();
	ffo_->SetScale(SHOT_EFFECT_SCALE);
	ffo_->SetPosition({ 50,10,-200 });

	//3Dオブジェクト生成
	playerObject_ = std::make_unique<Object3D>();
	playerObject_->SetModel(playerModel_.get());
	playerObject_->Initialize();
	playerObject_->SetScale(Vector3(PLAYER_SCALE));
	playerObject_->SetPosition({ 0,0,0 });

	podObject_ = std::make_unique<Object3D>();
	podObject_->SetModel(podModel_.get());
	podObject_->Initialize();

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(playerBulletCubeModel_.get());
	bulletObject_->Initialize();

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomGameModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(SKY_SCALE);
	skyObject_->SetPosition(SKY_POS);

	bossObject_ = std::make_unique<Object3D>();
	bossObject_->SetModel(enemyModel_.get());
	bossObject_->Initialize();
	bossObject_->SetScale(BOSS_SCALE);
	bossObject_->SetPosition(BOSS_POS);
}
