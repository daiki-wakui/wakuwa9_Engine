#include "GameScene.h"
#include "SoundManager.h"
#include "MyRandom.h"
#include "Easing.h"

void GameScene::Initialize()
{
	//3Dモデルの読み込みと生成
	Model3DManager::Load3DModel();
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
}

void GameScene::Finalize()
{
	models.clear();
	objects.clear();
	collisions_.clear();
}

void GameScene::Update()
{

	if (isShake_) {
		shakeTimer_++;

		if (shakeTimer_ > 30) {
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

	shadowObject_->SetPosition({ playerObject_->GetPosition().x,2,playerObject_->GetPosition().z });
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
		
		playBGM_ = false;
		power = 1;
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

	if (gamePad_->PushInstantY()) {
		player_->SetPos(dPoint_->GetWorldPos());
	}
	
	for (auto& object : objects) {
		object->Update();
	}

	player_->Update();
	if (player_->GetIsShot()) {

		SoundManager::GetInstance()->PlayWave("Shot.wav", 0.25f);
		player_->SetIsShot(false);
	}

	Vector3 podPos;

	if (coolTime_ < 0) {
		isShotEffect_ = true;
		coolTime_ = 2;
	}

	if (gamePad_->PushButtonRB()) {
		podRot.y = podObject_->GetRotation().y - 180;
		podRot.z += 225;
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

		if (timer_ > 60) {

			if (!bossBGM_) {
				SoundManager::GetInstance()->Update(2);
				bossBGM_ = true;
			}
		}

		//イベントシーン終わり
		if (timer_ > maxTime_) {
			isIvent_ = false;
			Vector3 eye = { 0,20,-30 };

			Object3D::SetEye(eye);
			eye = { 0,10,0 };
			Object3D::SetTarget(eye);
			iventEye_ = { 450,100,750 };
			gameUI_->SetMovieEnd(true);
			SoundManager::GetInstance()->PlayWave("Warning.wav", 2);
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

void GameScene::SpriteDraw()
{
	gameUI_->GameDraw();
}

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

		Vector3 v = { 0,1.5f,0 };

		for (int i = 0; i < 10; i++) {

			std::unique_ptr<Effect> newObj = std::make_unique<Effect>();
			newObj->Initialize(startEffect_, v, konnpeModel_.get());
			effects_.push_back(std::move(newObj));
			size++;
		}

		if (size == 10) {
			isEffect_ = false;
		}
	}

	skyObject_->SetPosition(player_->GetWorldPos());
	skyObject_->Update();
}

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

	SpriteDraw();
}

void GameScene::pDraw()
{
	particleMan_->Draw();

	if (!isIvent_) {
		player_->pDraw();
	}
}

void GameScene::EditorLoad(const std::string filename)
{
	objects.clear();
	enemys_.clear();
	doors_.clear();
	ReLoad(filename);
}

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
			playerObject_->SetScale(Vector3({ 1,1,1 }));
			playerObject_->SetPosition(newObject[objSize_]->GetPosition());
			playerObject_->SetRotation(newObject[objSize_]->GetRotation());
			playerObject_->SetCamera({ 0, 10, -30.0f }, { 0, 10, 0 });

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
			newObject[objSize_]->SetPosition({ rocalPos.x+15,rocalPos.y,rocalPos.z });

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
			newEnemy[enemySize_]->SetBulletModel(cubeModel_.get());
			


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


			newObject[objSize_]->SetScale({ 15,15,15 });
			boss_->Initialize(model,newObject[objSize_]->GetPosition(), newObject[objSize_].get(), player_.get());
			boss_->SetBulletModel(bossBulletModel_.get());
			boss_->SetBossModels(frameModel_.get(), bulletCononModel_.get());

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

void GameScene::AllCollison()
{
	//当たり判定
	Vector3 posA, posB;
	float r1 = 1.0f;	//イベントのスケール
	float r2 = 1.0f;	//自機のスケール

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	posA = player_->GetWorldPos();

	if (eventBox_->GetIsArive()) {
		posB = eventBox_->GetWorldPos();

		//AとBの距離
		r1 = 6.0f;	//イベントのスケール
		r2 = 13.0f;	//自機のスケール

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
		r1 = 3.0f;	//イベントのスケール
		r2 = 13.0f;	//自機のスケール

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
			r1 = 20.0f;	//敵のスケール
			r2 = 1.0f;	//弾のスケール

			if (Collison(posA, posB, r1, r2) && hitBox_) {
				bullet->isDead_ = true;
				boss_->OnCollision();

				SoundManager::GetInstance()->PlayWave("Hit.wav", 0.12f);
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
			r1 = 7.0f;	//敵のスケール
			r2 = 1.0f;	//弾のスケール

			if (Collison(posA, posB, r1, r2)) {
				bullet->isDead_ = true;

				if (!player_->Getinvincible()) {

					SoundManager::GetInstance()->PlayWave("noise.wav", 0.5f);

					isShake_ = true;
				}
				player_->OnCollision();

				if (player_->GetHP() <= 1) {
					SoundManager::GetInstance()->PlayWave("electric_shock3.wav", 1.1f);
				}
			}
		}
	}

	//ボス弾と自機の当たり判定
	if (boss_->GetArive()) {
		for (const std::unique_ptr<BossBullet>& bullet : boss_->GetBullets()) {
			//敵弾の座標
			posB = bullet->GetWorldPos();

			//AとBの距離
			r1 = 7.0f;	//敵のスケール
			r2 = 1.0f;	//弾のスケール

			if (Collison(posA, posB, r1, r2)) {
				bullet->isDead_ = true;

				if (!player_->Getinvincible()) {

					SoundManager::GetInstance()->PlayWave("noise.wav", 0.5f);

					isShake_ = true;
				}
				player_->OnCollision();

			}
		}
	}

	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		//AとBの距離
		r1 = 3.0f;	//自機のスケール
		r2 = 175.0f;	//索敵範囲のスケール

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
			r1 = 7.0f;	//敵のスケール
			r2 = 1.0f;	//弾のスケール

			if (Collison(posA, posB, r1, r2)) {
				bullet->isDead_ = true;
				enemy->OnCollision();
				BulletEffect = true;
				SoundManager::GetInstance()->PlayWave("Hit.wav", 0.12f);
			}

			if (enemy->IsDead()) {
				isEffect_ = true;
				startEffect_ = enemy->GetWorldPos();
			}
		}
	}
}

void GameScene::Object3DGenerate()
{
	shadowObject_ = std::make_unique<Object3D>();
	shadowObject_->SetModel(shadowModel_.get());
	shadowObject_->Initialize();
	shadowObject_->SetScale({ 1.5f, 1.5f, 1.5f });

	poriObject_ = std::make_unique<Object3D>();
	poriObject_->SetModel(poriModel_.get());
	poriObject_->Initialize();
	poriObject_->SetScale({ 5,5,5 });
	poriObject_->SetPosition({ 0,10,10 });

	//3Dオブジェクト生成
	playerObject_ = std::make_unique<Object3D>();
	playerObject_->SetModel(playerModel_.get());
	playerObject_->Initialize();
	playerObject_->SetScale(Vector3({ 1,1,1 }));
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
	skyObject_->SetScale(Vector3({ 900,900,900 }));
	skyObject_->SetPosition({ 0,0,100 });

	bossObject_ = std::make_unique<Object3D>();
	bossObject_->SetModel(enemyModel_.get());
	bossObject_->Initialize();
	bossObject_->SetScale({ 15,15,15 });
	bossObject_->SetPosition({ 0,20,370 });
}
