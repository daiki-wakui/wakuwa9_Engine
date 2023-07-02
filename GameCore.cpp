#include "GameCore.h"

void GameCore::Initialize()
{
	//windowsAPI������
	windows_->Initalize();

	//DirectX������
	directX_->Initialize(windows_.get());

	//keyborad������
	keyboard_->Initialize(windows_->GetHInstancee(), windows_->GetHwnd());

	imGuiM_->Initialize(windows_.get(), directX_.get());

	spBasis_->Initialize(directX_.get());

	postTex = spBasis_->TextureData(L"Resources/white1x1.png");

	tex1_ = spBasis_->TextureData(L"Resources/001.png");
	tex2_ = spBasis_->TextureData(L"Resources/test.png");
	tex3_ = spBasis_->TextureData(L"Resources/title.png");

	

	backTex_ = spBasis_->TextureData(L"Resources/backTitle.png");

	playerHP_ = spBasis_->TextureData(L"Resources/playerHP.png");
	bossHP_ = spBasis_->TextureData(L"Resources/red.png");

	spBasis_->TextureSetting();

	sprite_->Initialize(spBasis_.get(), windows_.get());
	sprite_->Create(50, 50);

	backSprite_->Initialize(spBasis_.get(), windows_.get());
	backSprite_->Create(640, 360);
	backSprite_->SetSize({ 1280,720 });
	backSprite_->Update();

	playerHPSprite_->Initialize(spBasis_.get(), windows_.get());
	playerHPSprite_->Create(100, 30);

	bossHPSprite_->Initialize(spBasis_.get(), windows_.get());
	bossHPSprite_->Create(640, 30);

	postEffect_->SetDirectX(spBasis_.get(), windows_.get(),keyboard_.get());
	postEffect_->Initialize(0);

	gaussianEffect_->SetDirectX(spBasis_.get(), windows_.get(), keyboard_.get());
	gaussianEffect_->Initialize(1);

	Object3D::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());

	//Fbx������
	FbxLoader::GetInstance()->Initialize(directX_->GetDevice());

	XMFLOAT3 eye = Object3D::GetEye();
	XMFLOAT3 target = Object3D::GetTarget();
	XMFLOAT3 up = Object3D::GetUp();

	FbxObject3d::SetCamera(eye, target, up);

	FbxObject3d::StaticInitialize(directX_->GetDevice(), windows_->GetWindowWidth(), windows_->GetWindowHeight());

	//DirectionalLight::StaticInitalize(dxBasis->GetDevice());



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

	filedModel_ = std::make_unique<Model>();
	filedModel_->LoadFromObj("filed");


	eventBox_ = std::make_unique<EventBox>();

	//3D�I�u�W�F�N�g����
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

	enemyObject_ = std::make_unique<Object3D>();
	enemyObject_->SetModel(enemyModel_.get());
	enemyObject_->Initialize();

	enemyObject2_ = std::make_unique<Object3D>();
	enemyObject2_->SetModel(enemyModel_.get());
	enemyObject2_->Initialize();

	enemyObject3_ = std::make_unique<Object3D>();
	enemyObject3_->SetModel(enemyModel_.get());
	enemyObject3_->Initialize();

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

	LightGroup::StaticInitialize(directX_->GetDevice());

	//���C�g����
	lightGroup->Initialize();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());

	//�I�[�f�B�I������
	sound_->Initialize();

	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("Alarm01.wav");



	// ���x���f�[�^�̓ǂݍ���
	levelData_ = LevelLoader::LoadFile("obj");

	//models.insert(std::make_pair(std::string("player"), playerModel_.get()));
	//models.insert(std::make_pair(std::string("enemy"), floorModel));
	//models.insert(std::make_pair(std::string("enemySpawn"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("filed"), filedModel_.get()));
	models.insert(std::make_pair(std::string("IventBlock"), cubeModel_.get()));


	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData_->objects) {
		// �t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		if (objectData.fileName == "camera") {

			DirectX::XMFLOAT3 eye;
			DirectX::XMStoreFloat3(&eye, objectData.translation);
			Object3D::CameraMoveVector(eye);
		}

		if (objectData.fileName == "IventBlock") {
			// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
			Object3D* newObject = Object3D::Create(1.0f);
			//newObject->SetModel(playerModel);
			newObject->SetModel(model);

			// ���W
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			newObject->SetPosition(pos);

			// ��]�p
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			newObject->SetRotation({ rot.x,rot.y,rot.z });


			// ���W
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);


			eventBox_->Initialize(model, newObject);
		}
		else {
			// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
			Object3D* newObject = Object3D::Create(1.0f);
			//newObject->SetModel(playerModel);
			newObject->SetModel(model);


			// ���W
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			newObject->SetPosition(pos);

			// ��]�p
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			newObject->SetRotation({ rot.x,rot.y,rot.z });


			// ���W
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);

			// �z��ɓo�^
			objects.push_back(newObject);
		}

	}

	player_->Initialize(playerModel_.get(), playerObject_.get(), keyboard_.get(), podObject_.get());
	player_->SetBulletModel(cubeModel_.get(), bulletObject_.get());

	newEnemy->Initialize(enemyObject_.get(), {-25,0,30}, player_.get());
	newEnemy->SetBulletModel(cubeModel_.get());
	newEnemy2->Initialize(enemyObject2_.get(), {0,0,30}, player_.get());
	newEnemy2->SetBulletModel(cubeModel_.get());
	newEnemy3->Initialize(enemyObject3_.get(), {25,0,30}, player_.get());
	newEnemy3->SetBulletModel(cubeModel_.get());

	//�G��o�^����
	enemys_.push_back(std::move(newEnemy));
	enemys_.push_back(std::move(newEnemy2));
	enemys_.push_back(std::move(newEnemy3));

	boss_->Initialize(enemyModel_.get(), bossObject_.get());
}

void GameCore::Finalize()
{
	imGuiM_->Finalize();
	
	sound_->Finalize();

	testModel_->fbxScene_->Destroy();

	FbxLoader::GetInstance()->Finalize();

	models.clear();
	objects.clear();

	windows_->Release();
}

void GameCore::Inport()
{

}

void GameCore::Update()
{
	//keyborad�X�V����
	keyboard_->Update();

	lightGroup->Update();

	sprite_->Update();

	//playerObject_->Update();
	skyObject_->Update();
	objectFloor_->Update();

	//testObj_->Update();

	//sound_->PlayWave("Alarm01.wav");

	eventBox_->Update();

	for (auto& object : objects) {
		object->Update();
	}
	player_->Update();

	//enemy�̎��S�t���O
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});

	//�G�̓���
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
			boss_->OnCollision();

			//enemy->OnCollision();

			//if (enemy->IsDead() == true) {
			//	//knockDownNum++;
			//}
			//SoundPlayWave(xAudio2.Get(), soundData2);
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

				/*if (isPlayerDamege == 0 && invincible == 0) {
					
					isPlayerDamege = 1;
					
					SoundPlayWave(xAudio2.Get(), soundData5);
				}*/
			}
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

				if (enemy->IsDead() == true) {
					//knockDownNum++;
				}
				//SoundPlayWave(xAudio2.Get(), soundData2);
			}
		}
	}

	imGuiM_->Begin();

	imGuiM_->End();
}

void GameCore::Draw()
{

	postEffect_->PreDrawScene(directX_->GetCommandList());
	
	//backSprite_->Draw(backTex_);

	Object3D::PreDraw(directX_->GetCommandList());
	FbxObject3d::PreSet(directX_->GetCommandList());

	//obj
	//playerObject_->Draw();
	skyObject_->Draw();
	//objectFloor_->Draw();

	//fbx
	//testObj_->Draw();

	eventBox_->Draw();

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
	

	Object3D::PostDraw();

	//sprite_->Draw(tex1_);

	playerHPSprite_->Draw(playerHP_);

	if (HitBox == true && boss_->GetArive() == true) {
		bossHPSprite_->Draw(bossHP_);

	}
	
	postEffect_->PostDrawScene(directX_->GetCommandList());

	// �`��O����
	directX_->PreDraw();

	postEffect_->Draw();

	//�`��㏈��
	directX_->PostDraw();
}
