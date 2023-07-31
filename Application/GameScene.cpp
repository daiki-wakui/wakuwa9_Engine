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

	fillter_ = spBasis_->TextureData(L"Resources/fillter.png");

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

	fillSprite_->Initialize(spBasis_, windows_);
	fillSprite_->Create(640, 360);
	fillSprite_->SetSize({ 1280,720 });
	fillSprite_->Update();

	//OBJ���烂�f����ǂݍ���
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

	filedCubeModel_ = std::make_unique<Model>();
	filedCubeModel_->LoadFromObj("cube2");

	filedModel_ = std::make_unique<Model>();
	filedModel_->LoadFromObj("filed");


	eventBox_ = std::make_unique<EventBox>();

	//3D�I�u�W�F�N�g����
	playerObject_ = std::make_unique<Object3D>();
	//playerObject->SetModel(playerModel2);
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

	//FBX�t�@�C���ǂݍ���

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

			if (!playBGM_) {
				sound_->PlayWave("ElectricWild.wav");
				playBGM_ = true;
			}
			
		}

		sceneSprite_->SetSize(pos);
	}

	particleMan_->Update();

	sprite_->Update();

	fillSprite_->Update();

	skyObject_->SetPosition(player_->GetWorldPos());

	skyObject_->Update();
	objectFloor_->Update();

	//testObj_->Update();

	if (keyboard_->keyInstantPush(DIK_B)) {
		sound_->PlayWave("Alarm01.wav");
	}
	

	//�Q�[�����
	eventBox_->Update();

	for (auto& object : objects) {
		object->Update();
	}

	if (player_->IsDead() == false) {
		player_->Update();
		if (player_->GetIsShot()) {
			sound_->PlayWave("Shot.wav");
			player_->SetIsShot(false);
		}
	}

	//enemy�̎��S�t���O
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});

	//�G�̓���
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update(!start_);
	}

	for (std::unique_ptr<CollisionBox>& collision : collisions_) {
		collision->Update();
	}

	if (HitBox == true) {
		sound_->StopWAVE("ElectricWild.wav");

		if (!bossBGM_) {
			sound_->PlayWave("Alone.wav");
			bossBGM_ = true;
		}

		boss_->Update();
		bossHPSprite_->SetSize({ 16.0f * (float)boss_->GetHP(),32.0f });
		bossHPSprite_->Update();
	}

	playerHPSprite_->SetSize({ 32.0f * (float)player_->GetHP(),16.0f });
	playerHPSprite_->Update();


	//�����蔻��
	XMFLOAT3 posA, posB;

	//���e���X�g�̎擾
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	posA = player_->GetWorldPos();

	posB = eventBox_->GetWorldPos();

	//A��B�̋���
	float r1 = 3.0f;	//�C�x���g�̃X�P�[��
	float r2 = 13.0f;	//���@�̃X�P�[��
	float r = r1 + r2;

	XMFLOAT3 dis;
	dis.x = posB.x - posA.x;
	dis.y = posB.y - posA.y;
	dis.z = posB.z - posA.z;


	if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {

		HitBox = true;
	}

	//���@�̒e�ƃ{�X�̓����蔻��
	posA = boss_->GetWorldPos();

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//���e�̍��W
		posB = bullet->GetWorldPos();

		//A��B�̋���
		float r1 = 20.0f;	//�G�̃X�P�[��
		float r2 = 1.0f;	//�e�̃X�P�[��
		float r = r1 + r2;

		XMFLOAT3 dis;
		dis.x = posB.x - posA.x;
		dis.y = posB.y - posA.y;
		dis.z = posB.z - posA.z;


		if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && HitBox == true) {
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

	//���@�ƓG�̒e�̓����蔻��
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		for (const std::unique_ptr<EnemyBullet>& bullet : enemy->GetBullets()) {
			//�G�e�̍��W
			posB = bullet->GetWorldPos();

			//A��B�̋���
			float r1 = 7.0f;	//�G�̃X�P�[��
			float r2 = 1.0f;	//�e�̃X�P�[��
			float r = r1 + r2;

			XMFLOAT3 dis;
			dis.x = posB.x - posA.x;
			dis.y = posB.y - posA.y;
			dis.z = posB.z - posA.z;

			if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && player_->GetHP() > 0) {

				bullet->isDead_ = true;
				player_->OnCollision();
			}
		}
	}

	//
	posA = player_->GetWorldPos();

	for (const std::unique_ptr<CollisionBox>& coll : collisions_) {
		posB = coll->GetWorldPos();

		//A��B�̋���
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

			int a = 0;
			a++;
		}
		else {
			coll->hit_ = false;
		}
	}


	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		posA = enemy->GetWorldPos();

		//���@�̒e�ƓG�̓����蔻��
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//���e�̍��W
			posB = bullet->GetWorldPos();

			//A��B�̋���
			float r1 = 7.0f;	//�G�̃X�P�[��
			float r2 = 1.0f;	//�e�̃X�P�[��
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

	if (player_->IsDead() == false) {
		player_->Draw();
	}

	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw();
	}

	/*for (std::unique_ptr<CollisionBox>& collision : collisions_) {
		collision->Draw();
	}*/

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

	fillSprite_->Draw(fillter_);
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
	// ���x���f�[�^�̓ǂݍ���
	levelData_ = LevelLoader::LoadFile("obj");

	//models.insert(std::make_pair(std::string("player"), playerModel_.get()));
	models.insert(std::make_pair(std::string("boss"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("enemySpawn"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("filed"), filedModel_.get()));
	models.insert(std::make_pair(std::string("IventBlock"), cubeModel_.get()));
	models.insert(std::make_pair(std::string("FliedBlock"), filedCubeModel_.get()));
	models.insert(std::make_pair(std::string("wallBlock"), cubeModel_.get()));


	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (int32_t i = 0; i < levelData_->objects.size(); i++) {
		// �t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(levelData_->objects[i].fileName);
		if (it != models.end()) {
			model = it->second;
		}

		//�Ή����郂�f�����Ȃ���Δ�΂�
		if (model == nullptr) {
			continue;
		}
		//blender��̃J�����Z�b�g
		if (levelData_->objects[i].fileName == "camera") {

			DirectX::XMFLOAT3 eye;
			DirectX::XMStoreFloat3(&eye, levelData_->objects[i].translation);
			Object3D::CameraMoveVector(eye);

			continue;
		}

		//event�{�b�N�X�̔z�u
		if (levelData_->objects[i].fileName == "IventBlock") {
			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);

			eventBox_->Initialize(model, newObject[objSize].get());
			objSize++;
		}
		else if (levelData_->objects[i].fileName == "wallBlock") {
			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);

			//�I�u�W�F�N�g�����ƍ��W�����
			collBox[collSize] = std::make_unique<CollisionBox>();

			collBox[collSize]->Initialize(model, newObject[objSize].get());
			collBox[collSize]->SetScale(newObject[objSize]->GetScale());

			collisions_.push_back(std::move(collBox[collSize]));

			objSize++;
			collSize++;
		}
		else if (levelData_->objects[i].fileName == "enemySpawn") {

			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);

			//�I�u�W�F�N�g�����ƍ��W�����
			newEnemy[enemySize] = std::make_unique<Enemy>();

			newEnemy[enemySize]->Initialize(newObject[objSize].get(), newObject[objSize]->GetPosition(), player_.get());
			newEnemy[enemySize]->SetBulletModel(cubeModel_.get());

			//�G��o�^����
			enemys_.push_back(std::move(newEnemy[enemySize]));

			objSize++;
			enemySize++;
		}
		else if (levelData_->objects[i].fileName == "boss") {
			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);


			newObject[objSize]->SetScale({ 15,15,15 });
			boss_->Initialize(model,newObject[objSize]->GetPosition(), newObject[objSize].get(), player_.get());
			boss_->SetBulletModel(cubeModel_.get());

			objSize++;
		}
		else {
			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);

			// �z��ɓo�^
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

	// ���W
	DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, levelData_->objects[size].translation);
	newObject[objSize]->SetPosition(pos);

	// ��]�p
	DirectX::XMFLOAT3 rot;
	DirectX::XMStoreFloat3(&rot, levelData_->objects[size].rotation);
	newObject[objSize]->SetRotation({ rot.x,rot.y,rot.z });


	// ���W
	DirectX::XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, levelData_->objects[size].scaling);
	newObject[objSize]->SetScale(scale);
}
