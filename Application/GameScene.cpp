#include "GameScene.h"
#include "Easing.h"

void GameScene::SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound)
{
	windows_ = windows;
	directX_ = directX;
	imGuiM_ = imGuiM;
	spBasis_ = spBasis;
	sound_ = sound;
}

void GameScene::SetInputInfo(KeyBoard* keyboard, GamePad* gamePad)
{
	keyboard_ = keyboard;
	gamePad_ = gamePad;
}

void GameScene::Initialize()
{

	playerHP_ = spBasis_->TextureData(L"Resources/playerHP.png");
	bossHP_ = spBasis_->TextureData(L"Resources/red.png");

	title_ = spBasis_->TextureData(L"Resources/title.png");;
	gameover_ = spBasis_->TextureData(L"Resources/gameover.png");
	gameclear_ = spBasis_->TextureData(L"Resources/gameclear.png");

	//シーン遷移時
	scene_ = spBasis_->TextureData(L"Resources/scene.png");

	//フィルター
	fillter_ = spBasis_->TextureData(L"Resources/fillter.png");

	reticleImage_ = spBasis_->TextureData(L"Resources/reticle.png");

	tesImage_ = spBasis_->TextureData(L"Resources/test.png");

	targetImage_ = spBasis_->TextureData(L"Resources/targetPoint.png");

	iventImage_ = spBasis_->TextureData(L"Resources/Ivent.png");

	spBasis_->TextureSetting();

	playerHPSprite_->Initialize(spBasis_, windows_);
	playerHPSprite_->Create(100, 30);
	bossHPSprite_->Initialize(spBasis_, windows_);
	bossHPSprite_->Create(640, 30);

	gameoverSprite_->Initialize(spBasis_, windows_);
	gameoverSprite_->Create(640, 360);
	gameoverSprite_->SetSize({ 1280,720 });
	gameoverSprite_->Update();

	gameclearSprite_->Initialize(spBasis_, windows_);
	gameclearSprite_->Create(640, 360);
	gameclearSprite_->SetSize({ 1280,720 });
	gameclearSprite_->Update();

	reticleSprite_->Initialize(spBasis_, windows_);
	reticleSprite_->Create(640, 360);
	reticleSprite_->SetSize({ 0,0 });

	sceneSprite_->Initialize(spBasis_, windows_);
	sceneSprite_->Create(640, 360);
	sceneSprite_->SetSize({ 1920,1920 });

	fillSprite_->Initialize(spBasis_, windows_);
	fillSprite_->Create(640, 360);
	fillSprite_->SetSize({ 1280,720 });
	fillSprite_->Update();

	sSprite_->Initialize(spBasis_, windows_);
	sSprite_->Create(0, 0);
	sSprite_->SetSize({ 32,32 });
	//sSprite_->SetAncP({ 0,0 });
	sSprite_->Update();

	iventSprite_->Initialize(spBasis_, windows_);
	iventSprite_->Create(640, 360);
	iventSprite_->SetSize({ 1280,720 });
	iventSprite_->SetColor({ 1,1,1,0 });
	iventSprite_->Update();


	//OBJからモデルを読み込む
	playerModel_ = std::make_unique<Model>();
	playerModel_->LoadFromObj("player");

	podModel_ = std::make_unique<Model>();
	podModel_->LoadFromObj("pod");

	skydomModel_ = std::make_unique<Model>();
	skydomModel_->LoadFromObj("world");

	enemyModel_ = std::make_unique<Model>();
	enemyModel_->LoadFromObj("enemySou");

	cubeModel_ = std::make_unique<Model>();
	cubeModel_->LoadFromObj("cube");

	filedCubeModel_ = std::make_unique<Model>();
	filedCubeModel_->LoadFromObj("cube2");

	filedModel_ = std::make_unique<Model>();
	filedModel_->LoadFromObj("filed");

	poriModel_ = std::make_unique<Model>();
	poriModel_->LoadFromObj("pori");

	poriObject_ = std::make_unique<Object3D>();
	poriObject_->SetModel(poriModel_.get());
	poriObject_->Initialize();
	poriObject_->SetScale({ 5,5,5 });
	poriObject_->SetPosition({ 0,10,10 });


	eventBox_ = std::make_unique<EventBox>();

	//3Dオブジェクト生成
	playerObject_ = std::make_unique<Object3D>();
	playerObject_->SetModel(playerModel_.get());
	playerObject_->Initialize();
	playerObject_->SetScale(XMFLOAT3({ 1,1,1 }));
	playerObject_->SetPosition({ 0,0,0 });

	podObject_ = std::make_unique<Object3D>();
	podObject_->SetModel(podModel_.get());
	podObject_->Initialize();

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(cubeModel_.get());
	bulletObject_->Initialize();

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(XMFLOAT3({ 600,600,600 }));
	skyObject_->SetPosition({ 0,0,100 });

	bossObject_ = std::make_unique<Object3D>();
	bossObject_->SetModel(enemyModel_.get());
	bossObject_->Initialize();
	bossObject_->SetScale({ 15,15,15 });
	bossObject_->SetPosition({ 0,20,370 });

	//FBXファイル読み込み
	testModel_ = std::make_unique<FbxModel>();
	FbxLoader::GetInstance()->LoadModelFromFile(testModel_.get(), "boneTest");

	testObj_ = std::make_unique<FbxObject3d>();
	testObj_->Initialize();
	testObj_->SetModel(testModel_.get());
	testObj_->PlayAnimation();

	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("Alarm01.wav");
	sound_->LoadWave("ElectricWild.wav");
	sound_->LoadWave("Alone.wav");
	sound_->LoadWave("Shot.wav");
	sound_->LoadWave("Hit.wav");
	sound_->LoadWave("noise.wav");

	ReLoad();

	player_->Initialize(playerModel_.get(), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
	player_->SetBulletModel(cubeModel_.get(), bulletObject_.get());

	particleMan_->Initialize();
	particleMan_->Update();
}

void GameScene::Finalize()
{
	testModel_->fbxScene_->Destroy();

	models.clear();
	objects.clear();
	collisions_.clear();
}

void GameScene::Update()
{
	sceneSprite_->Update();
	iventSprite_->Update();

	if (keyboard_->keyInstantPush(DIK_K)) {
		if (isIvent_ == false) {
			isIvent_ = true;
		}
		else {
			isIvent_ = false;
			alpha_ = 0;
			XMFLOAT3 eye = { 0,20,-30 };


			Object3D::SetEye(eye);
			
			//playerObject_->SetEye(eye);
			eye = { 0,10,0 };
			Object3D::SetTarget(eye);
			//playerObject_->SetTarget(eye);
			timer_ = 0;
			iventEye_ = { 360,20,700 };
		}
	}

	if (isIvent_) {
		alpha_ += 0.05f;
		alpha_ = min(alpha_, 1);
		timer_++;

		iventEye_ = iventEye_.lerp(iventEye_, endEye_, Easing::EaseInCubic(timer_, maxTime_));

		XMFLOAT3 eye;
		eye.x = iventEye_.x;
		eye.y = iventEye_.y;
		eye.z = iventEye_.z;

		XMFLOAT3 target;
		target.x = iventTarget_.x;
		target.y = iventTarget_.y;
		target.z = iventTarget_.z;

		Object3D::SetEye(eye);
		Object3D::SetTarget(target);

		/*playerObject_->SetEye(eye);
		playerObject_->SetTarget(target);*/

		if (timer_ > maxTime_) {
			isIvent_ = false;
			XMFLOAT3 eye = { 0,20,-30 };


			Object3D::SetEye(eye);

			//playerObject_->SetEye(eye);
			eye = { 0,10,0 };
			Object3D::SetTarget(eye);
			//playerObject_->SetTarget(eye);
			iventEye_ = { 360,20,700 };
		}
	}
	else {
  		alpha_ -= 0.05f;
		alpha_ = max(alpha_, 0);
	}
	
	XMFLOAT3 eye = Object3D::GetEye();
	XMFLOAT3 target = Object3D::GetTarget();

	iventSprite_->SetColor({ 1,1,1,alpha_ });

	if (gamePad_->PushButtonRB()) {
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

	if (start_) {
		pos = sceneSprite_->GetSize();

		power--;

		pos.x += power;
		pos.y += power;

		if (pos.y < 0) {
			pos.y = 0;
			pos.x = 0;
			start_ = false;
			
			
			if (!playBGM_) {
				sound_->PlayWave("ElectricWild.wav",0.5f);
				playBGM_ = true;
				Reset();
			}
			
		}

		sceneSprite_->SetSize(pos);
	}

	if (change_) {
		sound_->StopWAVE("ElectricWild.wav");
		if (hitBox_ == true) {
			sound_->StopWAVE("Alone.wav");
		}
		
		playBGM_ = false;
		power = 1;
		sceneSprite_->SetSize({ 1920,1920 });
	}

	particleMan_->Update();

	fillSprite_->Update();

	skyObject_->SetPosition(player_->GetWorldPos());

	skyObject_->Update();

	if (keyboard_->keyInstantPush(DIK_B)) {
		Reset();
	}

	//ゲーム画面
	eventBox_->Update();

	for (auto& object : objects) {
		object->Update();
	}

	if (player_->IsDead() == false && isIvent_ == false) {
		player_->Update();
		if (player_->GetIsShot()) {
			sound_->PlayWave("Shot.wav");
			player_->SetIsShot(false);
		}
	}

	//poriObject_->SetPosition(playerObject_->GetPosition());
	poriObject_->Update(true);

	if (!enemys_.empty()) {
		screenPos_ = enemys_.front()->GetObj()->Screen();

		spPos_.x = screenPos_.x;
		spPos_.y = screenPos_.y;

		sSprite_->SetPosition(spPos_);

		sSprite_->Update();

		player_->SetEnemy(enemys_.front().get());
	}
	

	//enemyの死亡フラグ
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});

	//敵の動き
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update(!start_);
	}

	for (std::unique_ptr<CollisionBox>& collision : collisions_) {
		collision->Update();
	}

	if (hitBox_ == true) {
		sound_->StopWAVE("ElectricWild.wav");

		if (!bossBGM_) {
			sound_->PlayWave("Alone.wav");
			bossBGM_ = true;
			isIvent_ = true;
		}

		if (hitBox_ == true && boss_->GetArive() == true) {
			boss_->Update();
		}
		
		bossHPSprite_->SetSize({ 16.0f * (float)boss_->GetHP(),32.0f });
		bossHPSprite_->Update();
	}

	playerHPSprite_->SetSize({ 128,16.0f });
	playerHPSprite_->Update();


	//当たり判定
	XMFLOAT3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	posA = player_->GetWorldPos();

	posB = eventBox_->GetWorldPos();

	
	//AとBの距離
	float r1 = 3.0f;	//イベントのスケール
	float r2 = 13.0f;	//自機のスケール

	if (Collison(posA, posB, r1, r2)) {
		hitBox_ = true;
	}

	//自機の弾とボスの当たり判定
	posA = boss_->GetWorldPos();

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPos();

		//AとBの距離
		float r1 = 20.0f;	//敵のスケール
		float r2 = 1.0f;	//弾のスケール
		float r = r1 + r2;

		XMFLOAT3 dis;
		dis.x = posB.x - posA.x;
		dis.y = posB.y - posA.y;
		dis.z = posB.z - posA.z;


		if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && hitBox_ == true) {
			bullet->isDead_ = true;
			BulletEffect = true;
			boss_->OnCollision();
			sound_->PlayWave("Hit.wav");
		}

		if (BulletEffect) {
			for (int i = 0; i < 3; i++) {
				XMFLOAT3 pos = posA;

				const float md_vel = 10.0f;
				XMFLOAT3 vel{};
				vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
				vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
				vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

				XMFLOAT3 acc{};
				const float md_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * md_acc;

				particleMan_->Add(60, pos, vel, acc, 2.0f, 0.0f);
			}
			BulletEffect = false;
		}
	}

	posA = player_->GetWorldPos();

	//自機と敵の弾の当たり判定
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		for (const std::unique_ptr<EnemyBullet>& bullet : enemy->GetBullets()) {
			//敵弾の座標
			posB = bullet->GetWorldPos();

			//AとBの距離
			float r1 = 7.0f;	//敵のスケール
			float r2 = 1.0f;	//弾のスケール
			float r = r1 + r2;

			XMFLOAT3 dis;
			dis.x = posB.x - posA.x;
			dis.y = posB.y - posA.y;
			dis.z = posB.z - posA.z;

			if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && player_->GetHP() > 0) {

				bullet->isDead_ = true;

				if (!player_->Getinvincible()) {
					sound_->PlayWave("noise.wav");
				}
				player_->OnCollision();
			}
		}
	}

	//ボス弾と自機の当たり判定
	for (const std::unique_ptr<BossBullet>& bullet : boss_->GetBullets()) {
		//敵弾の座標
		posB = bullet->GetWorldPos();

		//AとBの距離
		float r1 = 7.0f;	//敵のスケール
		float r2 = 1.0f;	//弾のスケール
		float r = r1 + r2;

		XMFLOAT3 dis;
		dis.x = posB.x - posA.x;
		dis.y = posB.y - posA.y;
		dis.z = posB.z - posA.z;

		if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && player_->GetHP() > 0) {

			bullet->isDead_ = true;

			if (!player_->Getinvincible()) {
				sound_->PlayWave("noise.wav");
			}
			player_->OnCollision();
		}
	}

	//
	posA = player_->GetWorldPos();

	for (const std::unique_ptr<CollisionBox>& coll : collisions_) {
		posB = coll->GetWorldPos();

		//AとBの距離
		float r1 = 1.0f;
		float r2 = coll->GetScale().x;
		float r = r1 + r2;

		XMFLOAT3 dis;
		dis.x = posB.x - posA.x;
		dis.y = posB.y - posA.y;
		dis.z = posB.z - posA.z;

		if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && player_->GetHP() > 0) {

			coll->OnCollision();
			player_->wallHit();
		}
		else {
			coll->hit_ = false;
		}
	}

	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		//AとBの距離
		float r1 = 3.0f;	//自機のスケール
		float r2 = 175.0f;	//索敵範囲のスケール

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
			float r1 = 7.0f;	//敵のスケール
			float r2 = 1.0f;	//弾のスケール
			float r = r1 + r2;

			XMFLOAT3 dis;
			dis.x = posB.x - posA.x;
			dis.y = posB.y - posA.y;
			dis.z = posB.z - posA.z;


			if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
				bullet->isDead_ = true;
				enemy->OnCollision();
				BulletEffect = true;
				sound_->PlayWave("Hit.wav");
			}
		}

		if (BulletEffect) {
			for (int i = 0; i < 3; i++) {
				XMFLOAT3 pos = posA;

				const float md_vel = 10.0f;
				XMFLOAT3 vel{};
				vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
				vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
				vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

				XMFLOAT3 acc{};
				const float md_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * md_acc;

				particleMan_->Add(60, pos, vel, acc, 2.0f, 0.0f);
			}
			BulletEffect = false;
		}
	}
}

void GameScene::Draw()
{
	//eventBox_->Draw();

	for (auto& object : objects) {
		object->Draw();
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
	
	//poriObject_->Draw();

	//fbx
	//testObj_->Draw();

	playerHPSprite_->Draw(playerHP_);

	if (hitBox_ == true && boss_->GetArive() == true) {
		bossHPSprite_->Draw(bossHP_);

	}

	if (player_->IsDead() == true) {
		gameoverSprite_->Draw(gameover_);
	}

	if (boss_->GetArive() == false) {
		gameclearSprite_->Draw(gameclear_);
	}

	sceneSprite_->Draw(scene_);

	reticleSprite_->Draw(reticleImage_);

	fillSprite_->Draw(fillter_);
	iventSprite_->Draw(iventImage_);
	//sSprite_->Draw(targetImage_);
}

void GameScene::pDraw()
{
	particleMan_->Draw();
}

void GameScene::EditorLoad()
{
	objects.clear();
	enemys_.clear();
	ReLoad();

}

void GameScene::ReLoad()
{
	// レベルデータの読み込み
	levelData_ = LevelLoader::LoadFile("obj");

	//models.insert(std::make_pair(std::string("player"), playerModel_.get()));
	models.insert(std::make_pair(std::string("boss"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("enemySpawn"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("filed"), filedModel_.get()));
	models.insert(std::make_pair(std::string("IventBlock"), cubeModel_.get()));
	models.insert(std::make_pair(std::string("FliedBlock"), filedCubeModel_.get()));
	models.insert(std::make_pair(std::string("wallBlock"), cubeModel_.get()));

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

			DirectX::XMFLOAT3 eye;
			DirectX::XMStoreFloat3(&eye, levelData_->objects[i].translation);
			Object3D::CameraMoveVector(eye);

			continue;
		}

		//eventボックスの配置
		if (levelData_->objects[i].fileName == "IventBlock") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			eventBox_->Initialize(model, newObject[objSize_].get());
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

			newEnemy[enemySize_]->Initialize(newObject[objSize_].get(), newObject[objSize_]->GetPosition(), player_.get());
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
			boss_->SetBulletModel(cubeModel_.get());

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
	DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, levelData_->objects[size].translation);
	newObject[objSize_]->SetPosition(pos);

	// 回転角
	DirectX::XMFLOAT3 rot;
	DirectX::XMStoreFloat3(&rot, levelData_->objects[size].rotation);
	newObject[objSize_]->SetRotation({ rot.x,rot.y,rot.z });


	// 座標
	DirectX::XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, levelData_->objects[size].scaling);
	newObject[objSize_]->SetScale(scale);
}

void GameScene::Reset()
{
	bossBGM_ = false;
	hitBox_ = false;
	player_.reset();
	playerObject_->Initialize();
	playerObject_->SetScale(XMFLOAT3({ 1,1,1 }));
	playerObject_->SetPosition({ 360,0,400 });
	playerObject_->SetRotation({ 0,0,0 });
	playerObject_->SetCamera({ 0, 20, -30.0f }, { 0, 10, 0 });

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get(), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
	player_->SetBulletModel(cubeModel_.get(), bulletObject_.get());

	EditorLoad();
}

bool GameScene::Collison(XMFLOAT3 posa, XMFLOAT3 posb, float aScale, float bScale)
{
	float r = aScale + bScale;

	XMFLOAT3 dis;
	dis.x = posb.x - posa.x;
	dis.y = posb.y - posa.y;
	dis.z = posb.z - posa.z;

	if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && player_->GetHP() > 0) {
		return true;
	}

	return false;
}
