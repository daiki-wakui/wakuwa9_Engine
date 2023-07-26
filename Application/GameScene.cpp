#include "GameScene.h"

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
	postTex = spBasis_->TextureData(L"Resources/white1x1.png");

	tex1_ = spBasis_->TextureData(L"Resources/001.png");
	tex2_ = spBasis_->TextureData(L"Resources/test.png");

	backTex_ = spBasis_->TextureData(L"Resources/backTitle.png");

	playerHP_ = spBasis_->TextureData(L"Resources/playerHP.png");
	bossHP_ = spBasis_->TextureData(L"Resources/red.png");

	title_ = spBasis_->TextureData(L"Resources/title.png");;
	gameover_ = spBasis_->TextureData(L"Resources/gameover.png");
	gameclear_ = spBasis_->TextureData(L"Resources/gameclear.png");

	scene_ = spBasis_->TextureData(L"Resources/scene.png");

	spBasis_->TextureSetting();

	sprite_->Initialize(spBasis_, windows_);
	sprite_->Create(50, 50);

	backSprite_->Initialize(spBasis_, windows_);
	backSprite_->Create(640, 360);
	backSprite_->SetSize({ 1280,720 });
	backSprite_->Update();

	playerHPSprite_->Initialize(spBasis_, windows_);
	playerHPSprite_->Create(100, 30);

	bossHPSprite_->Initialize(spBasis_, windows_);
	bossHPSprite_->Create(640, 30);

	titleSprite_->Initialize(spBasis_, windows_);
	titleSprite_->Create(640, 360);
	titleSprite_->SetSize({ 1280,720 });
	titleSprite_->Update();

	gameoverSprite_->Initialize(spBasis_, windows_);
	gameoverSprite_->Create(640, 360);
	gameoverSprite_->SetSize({ 1280,720 });
	gameoverSprite_->Update();

	gameclearSprite_->Initialize(spBasis_, windows_);
	gameclearSprite_->Create(640, 360);
	gameclearSprite_->SetSize({ 1280,720 });
	gameclearSprite_->Update();

	sceneSprite_->Initialize(spBasis_, windows_);
	sceneSprite_->Create(640, 360);
	sceneSprite_->SetSize({ 1920,1920 });
	sceneSprite_->Update();

	//OBJからモデルを読み込む
	playerModel_ = std::make_unique<Model>();
	playerModel_->LoadFromObj("player");

	podModel_ = std::make_unique<Model>();
	podModel_->LoadFromObj("pod");

	floorModel_ = std::make_unique<Model>();
	floorModel_->LoadFromObj("floor");

	skydomModel_ = std::make_unique<Model>();
	skydomModel_->LoadFromObj("world");

	enemyModel_ = std::make_unique<Model>();
	enemyModel_->LoadFromObj("enemySou");

	cubeModel_ = std::make_unique<Model>();
	cubeModel_->LoadFromObj("cube");

	filedModel_ = std::make_unique<Model>();
	filedModel_->LoadFromObj("filed");


	eventBox_ = std::make_unique<EventBox>();

	//3Dオブジェクト生成
	playerObject_ = std::make_unique<Object3D>();
	//playerObject->SetModel(playerModel2);
	playerObject_->SetModel(playerModel_.get());
	playerObject_->Initialize();
	playerObject_->SetScale(XMFLOAT3({ 1,1,1 }));
	playerObject_->SetPosition({ 0,10,0 });

	podObject_ = std::make_unique<Object3D>();
	podObject_->SetModel(podModel_.get());
	podObject_->Initialize();

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(cubeModel_.get());
	bulletObject_->Initialize();

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(XMFLOAT3({ 400,400,400 }));
	skyObject_->SetPosition({ 0,0,100 });

	objectFloor_ = std::make_unique<Object3D>();
	objectFloor_->SetModel(floorModel_.get());
	objectFloor_->Initialize();
	objectFloor_->SetScale(XMFLOAT3({ 5,5,5 }));
	objectFloor_->SetPosition({ 0,-10,0 });


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
}

void GameScene::Update()
{
	if (start_) {
		pos = sceneSprite_->GetSize();

		power--;

		pos.x += power;
		pos.y += power;

		if (pos.y < 0) {
			pos.y = 0;
			pos.x = 0;
			//change_ = true;
			start_ = false;
		}

		sceneSprite_->SetSize(pos);
	}
	sceneSprite_->Update();

	particleMan_->Update();

	sprite_->Update();

	skyObject_->Update();
	objectFloor_->Update();

	//testObj_->Update();

	//sound_->PlayWave("Alarm01.wav");

	//ゲーム画面
	eventBox_->Update();

	for (auto& object : objects) {
		object->Update();
	}

	if (player_->IsDead() == false) {
		player_->Update();

	}

	//enemyの死亡フラグ
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});

	//敵の動き
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}

	if (HitBox == true) {
		boss_->Update();
		bossHPSprite_->SetSize({ 16.0f * (float)boss_->GetHP(),32.0f });
		bossHPSprite_->Update();
	}

	playerHPSprite_->SetSize({ 32.0f * (float)player_->GetHP(),16.0f });
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
	float r = r1 + r2;

	XMFLOAT3 dis;
	dis.x = posB.x - posA.x;
	dis.y = posB.y - posA.y;
	dis.z = posB.z - posA.z;


	if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {

		HitBox = true;
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


		if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && HitBox == true) {
			bullet->isDead_ = true;
			boss_->OnCollision();
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
				//player_->OnCollision();
			}
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
				//enemy->OnCollision();
				BulletEffect = true;
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

	if (player_->IsDead() == false) {
		player_->Draw();
	}

	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw();
	}

	if (HitBox == true && boss_->GetArive() == true) {
		boss_->Draw();
	}

	//obj
	skyObject_->Draw();
	
	//fbx
	//testObj_->Draw();

	//sprite_->Draw(tex1_);

	playerHPSprite_->Draw(playerHP_);

	if (HitBox == true && boss_->GetArive() == true) {
		bossHPSprite_->Draw(bossHP_);

	}

	if (player_->IsDead() == true) {
		gameoverSprite_->Draw(gameover_);
	}

	if (boss_->GetArive() == false) {
		gameclearSprite_->Draw(gameclear_);
	}

	sceneSprite_->Draw(scene_);
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

			eventBox_->Initialize(model, newObject[objSize].get());
			objSize++;
		}
		else if (levelData_->objects[i].fileName == "enemySpawn") {

			//オブジェクト生成と座標情報代入
			Inport(model, i);

			//オブジェクト生成と座標情報代入
			newEnemy[enemySize] = std::make_unique<Enemy>();

			newEnemy[enemySize]->Initialize(newObject[objSize].get(), newObject[objSize]->GetPosition(), player_.get());
			newEnemy[enemySize]->SetBulletModel(cubeModel_.get());

			//敵を登録する
			enemys_.push_back(std::move(newEnemy[enemySize]));

			objSize++;
			enemySize++;
		}
		else if (levelData_->objects[i].fileName == "boss") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);


			newObject[objSize]->SetScale({ 15,15,15 });
			boss_->Initialize(model, newObject[objSize].get());


			objSize++;
		}
		else {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			// 配列に登録
			objects.push_back(newObject[objSize].get());
			objSize++;
		}

	}
}

void GameScene::Inport(Model* model, int32_t size)
{
	newObject[objSize] = std::make_unique<Object3D>();
	newObject[objSize]->SetModel(model);
	newObject[objSize]->Initialize();

	// 座標
	DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, levelData_->objects[size].translation);
	newObject[objSize]->SetPosition(pos);

	// 回転角
	DirectX::XMFLOAT3 rot;
	DirectX::XMStoreFloat3(&rot, levelData_->objects[size].rotation);
	newObject[objSize]->SetRotation({ rot.x,rot.y,rot.z });


	// 座標
	DirectX::XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, levelData_->objects[size].scaling);
	newObject[objSize]->SetScale(scale);
}
