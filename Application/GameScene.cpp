#include "GameScene.h"
#include "Easing.h"
#include <random>

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

	//�V�[���J�ڎ�
	scene_ = spBasis_->TextureData(L"Resources/scene.png");

	//�t�B���^�[
	fillter_ = spBasis_->TextureData(L"Resources/fillter.png");

	reticleImage_ = spBasis_->TextureData(L"Resources/reticle.png");

	tesImage_ = spBasis_->TextureData(L"Resources/test.png");

	targetImage_ = spBasis_->TextureData(L"Resources/targetPoint.png");

	iventImage_ = spBasis_->TextureData(L"Resources/Ivent.png");

	warningImage_ = spBasis_->TextureData(L"Resources/warning.png");

	manualImageRB_ = spBasis_->TextureData(L"Resources/RB.png");

	exImage_ = spBasis_->TextureData(L"Resources/ex.png");

	damageFilter_ = spBasis_->TextureData(L"Resources/damageFilter.png");

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

	dFilterSprite_->Initialize(spBasis_, windows_);
	dFilterSprite_->Create(640, 360);
	dFilterSprite_->SetSize({ 1280,720 });
	dFilterSprite_->SetColor({ 1,1,1,0 });
	dFilterSprite_->Update();

	sSprite_->Initialize(spBasis_, windows_);
	sSprite_->Create(0, 0);
	sSprite_->SetSize({ 32,32 });
	//sSprite_->SetAncP({ 0,0 });
	sSprite_->Update();

	RBSprite_->Initialize(spBasis_, windows_);
	RBSprite_->Create(0, 0);
	RBSprite_->SetSize({ 160,160 });

	iventSprite_->Initialize(spBasis_, windows_);
	iventSprite_->Create(640, 360);
	iventSprite_->SetSize({ 1280,720 });
	iventSprite_->SetColor({ 1,1,1,0 });
	iventSprite_->Update();

	waringSprite_->Initialize(spBasis_, windows_);
	waringSprite_->Create(640, 360);
	waringSprite_->SetSize({ 1280,720 });
	waringSprite_->Update();

	//OBJ���烂�f����ǂݍ���
	playerModel_ = std::make_unique<Model>();
	playerModel_->LoadFromObj("player");

	podModel_ = std::make_unique<Model>();
	podModel_->LoadFromObj("pod");

	skydomModel_ = std::make_unique<Model>();
	skydomModel_->LoadFromObj("world");

	enemyModel_ = std::make_unique<Model>();
	enemyModel_->LoadFromObj("enemySou");

	enemyModel2_ = std::make_unique<Model>();
	enemyModel2_->LoadFromObj("enemySou2");

	cubeModel_ = std::make_unique<Model>();
	cubeModel_->LoadFromObj("cube");

	playerBulletCubeModel_ = std::make_unique<Model>();
	playerBulletCubeModel_->LoadFromObj("cube3");

	filedCubeModel_ = std::make_unique<Model>();
	filedCubeModel_->LoadFromObj("cube2");

	filedModel_ = std::make_unique<Model>();
	filedModel_->LoadFromObj("filed");

	filedTentoModel_ = std::make_unique<Model>();
	filedTentoModel_->LoadFromObj("nier_obj");

	filedTouModel_ = std::make_unique<Model>();
	filedTouModel_->LoadFromObj("nier_filed");

	konnpeModel_ = std::make_unique<Model>();
	konnpeModel_->LoadFromObj("konnpe");

	poriModel_ = std::make_unique<Model>();
	poriModel_->LoadFromObj("pori");

	shadowModel_ = std::make_unique<Model>();
	shadowModel_->LoadFromObj("cirshadow");

	shadowObject_ = std::make_unique<Object3D>();
	shadowObject_->SetModel(shadowModel_.get());
	shadowObject_->Initialize();
	shadowObject_->SetScale({ 1.5f, 1.5f, 1.5f });

	poriObject_ = std::make_unique<Object3D>();
	poriObject_->SetModel(poriModel_.get());
	poriObject_->Initialize();
	poriObject_->SetScale({ 5,5,5 });
	poriObject_->SetPosition({ 0,10,10 });


	eventBox_ = std::make_unique<EventBox>();

	//3D�I�u�W�F�N�g����
	playerObject_ = std::make_unique<Object3D>();
	playerObject_->SetModel(playerModel_.get());
	playerObject_->Initialize();
	playerObject_->SetScale(XMFLOAT3({ 1,1,1 }));
	playerObject_->SetPosition({ 0,0,0 });

	podObject_ = std::make_unique<Object3D>();
	podObject_->SetModel(podModel_.get());
	podObject_->Initialize();

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(playerBulletCubeModel_.get());
	bulletObject_->Initialize();

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(XMFLOAT3({ 900,900,900 }));
	skyObject_->SetPosition({ 0,0,100 });

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
	sound_->LoadWave("NieR_freld2.wav");
	sound_->LoadWave("Alone.wav");
	sound_->LoadWave("Shot.wav");
	sound_->LoadWave("Hit.wav");
	sound_->LoadWave("noise.wav");
	sound_->LoadWave("Warning.wav");
	sound_->LoadWave("electric_shock3.wav");

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
	//���Z�b�g
	if (keyboard_->keyInstantPush(DIK_B)) {
		Reset();
	}

	ObjectUpdate();

	//�X�v���C�g�X�V����
	SpriteUpdate();

	shadowObject_->SetPosition(player_->GetWorldPos());
	shadowObject_->Update();
	
	

	//�e�̍X�V����
	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Update();
	}

	//�f�X�t���O���������e���폜
	effects_.remove_if([](std::unique_ptr<Effect>& bullet) {
		return bullet->IsDead();
	});
	
	XMFLOAT3 eye = Object3D::GetEye();
	XMFLOAT3 target = Object3D::GetTarget();

	if (change_) {
		sound_->StopWAVE("NieR_freld2.wav");
		if (hitBox_ == true) {
			sound_->StopWAVE("Alone.wav");
		}
		
		playBGM_ = false;
		power = 1;
		sceneSprite_->SetSize({ 1920,1920 });
	}

	//�Q�[�����
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
	XMFLOAT3 podPos;

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

	//���e�B�N��Update
	//if (!enemys_.empty()) {
	//	screenPos_ = enemys_.front()->GetObj()->Screen();

	//	spPos_.x = screenPos_.x;
	//	spPos_.y = screenPos_.y;

	//	sSprite_->SetPosition(spPos_);

	//	sSprite_->Update();

	//	player_->SetEnemy(enemys_.front().get());
	//}

	
	
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

		//�C�x���g�V�[���I���
		if (timer_ > maxTime_) {
			isIvent_ = false;
			XMFLOAT3 eye = { 0,20,-30 };

			Object3D::SetEye(eye);
			eye = { 0,10,0 };
			Object3D::SetTarget(eye);
			iventEye_ = { 360,20,700 };
			movieEnd_ = true;
			sound_->PlayWave("Warning.wav", 3);
		}
	}
	else {
		alpha_ -= 0.05f;
		alpha_ = max(alpha_, 0);
	}

	//enemy�̎��S�t���O
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
	});

	//enemy�̎��S�t���O
	enemycharges_.remove_if([](std::unique_ptr<EnemyCharge>& enemy) {
		return enemy->IsDead();
	});

	//�G�̓���
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update(!start_);
	}

	//�G�̓���
	for (std::unique_ptr<EnemyCharge>& enemy : enemycharges_) {
		enemy->Update();
	}

	for (std::unique_ptr<CollisionBox>& collision : collisions_) {
		collision->Update();
	}

	AllCollison();
}

void GameScene::SpriteUpdate()
{
	dFilterSprite_->Update();
	sceneSprite_->Update();
	iventSprite_->Update();
	waringSprite_->Update();
	RBSprite_->Update();
	fillSprite_->Update();

	iventSprite_->SetColor({ 1,1,1,alpha_ });

	screenPosPlayer_ = playerObject_->Screen();
	//�`���[�g���A��UI
	RBSprite_->SetPosition({ screenPosPlayer_.x - 175,screenPosPlayer_.y - 90 });

	if (!manualOK_) {
		alphaRB_ += 0.15f;
		alphaRB_ = min(alphaRB_, 1);
		RBSprite_->SetColor({ 1,1,1,alphaRB_ });
	}
	else {
		alphaRB_ -= 0.15f;
		alphaRB_ = max(alphaRB_, 0);
		RBSprite_->SetColor({ 1,1,1,alphaRB_ });
	}

	//���C�t��Ȃ����ɏo��t�B���^�[
	if (player_->GetHP() <= 1) {
		isLifeOne_ = true;
	}
	else {
		isLifeOne_ = false;
	}
	//���C�t��Ȃ����ɏo��t�B���^�[
	if (isLifeOne_) {
		fillTimer_++;

		if (fillTimer_ < 50) {
			fillAlpha_ += 0.07f;
			fillAlpha_ = min(fillAlpha_, 1);
		}
		else {
			fillAlpha_ -= 0.05f;
			fillAlpha_ = max(fillAlpha_, 0);
		}
		dFilterSprite_->SetColor({ 1,1,1,fillAlpha_ });

	}
	else {
		fillTimer_ = 0;
	}

	

	//�V�[���؂�ւ�
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
				sound_->PlayLoopWave("NieR_freld2.wav", 0.5f);
				playBGM_ = true;
				Reset();
			}
		}
		sceneSprite_->SetSize(pos);
	}

	if (hitBox_ && !isIvent_) {
		if (count_ < 3) {
			pow_++;
			wSize_.y = waringSprite_->GetSize().y;
		}
		else {
			pow_ = 0;
		}

		//���[�j���O�_��
		if (pow_ > 2) {
			pow_ = 0;
			count_++;
		}

		//�_�Ō�̓���
		if (count_ == 3) {
			popFrame_++;

			if (popFrame_ > 60) {
				wTimer_++;
				wTimer_ = min(wTimer_, wMax_);

				wSize_ = wSize_.lerp({ 1280,720,0 }, { 1280,0,0 }, Easing::EaseOutCubic(wTimer_, wMax_));

				waringSprite_->SetSize({ wSize_.x,wSize_.y });
			}
		}
	}

	//�{�XHP�ABGM�炵
	if (hitBox_ == true) {
		sound_->StopWAVE("NieR_freld2.wav");

		if (!bossBGM_) {
			sound_->PlayWave("Alone.wav");
			bossBGM_ = true;
			isIvent_ = true;
		}

		if (hitBox_ == true && boss_->GetArive() == true) {
			boss_->Update(isIvent_);
		}

		bossHPSprite_->SetSize({ 16.0f * (float)boss_->GetHP(),32.0f });
		bossHPSprite_->Update();
	}

	//���E���e�B�N��
	if (gamePad_->PushButtonRB()) {
		manualOK_ = true;

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

	playerHPSprite_->SetSize({ 32.0f * (float)player_->GetHP(),16.0f });
	playerHPSprite_->Update();
}

void GameScene::SpriteDraw()
{
	if (!isIvent_) {
		playerHPSprite_->Draw(playerHP_);
	}

	if (hitBox_ == true && boss_->GetArive() == true && isIvent_ == false) {
		bossHPSprite_->Draw(bossHP_);
	}

	if (player_->IsDead() == true) {
		gameoverSprite_->Draw(gameover_);
	}

	if (boss_->GetArive() == false) {
		gameclearSprite_->Draw(gameclear_);
	}
	RBSprite_->Draw(manualImageRB_);

	sceneSprite_->Draw(scene_);

	reticleSprite_->Draw(reticleImage_);

	fillSprite_->Draw(fillter_);

	dFilterSprite_->Draw(damageFilter_);

	iventSprite_->Draw(iventImage_);

	if (pow_ < 1 && hitBox_ && movieEnd_) {
		waringSprite_->Draw(warningImage_);
	}

}

void GameScene::ObjectUpdate()
{
	particleMan_->Update();

	//�|�������̃I�u�W�F�N�g�G�t�F�N�g
	if (keyboard_->keyInstantPush(DIK_K)) {
		if (!isEffect_) {
			isEffect_ = true;
		}
	}

	if (isEffect_) {
		int size = 0;
		//�e�̐����Ə�����

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

	if (player_->IsDead() == false && isIvent_ == false) {
		player_->Draw();
	}

	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		if (enemy->GetIsMove()) {
			enemy->Draw();
		}
	}

	//�G�̓���
	for (std::unique_ptr<EnemyCharge>& enemy : enemycharges_) {
		enemy->Draw();
	}

	if (hitBox_ == true && boss_->GetArive() == true) {
		boss_->Draw();
	}

	//obj
	skyObject_->Draw();
	
	//�e�̍X�V����
	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Draw();
	}

	if (isShotEffect_) {
		poriObject_->Draw();
		isShotEffect_ = false;
	}
	
	//fbx
	//testObj_->Draw();

	SpriteDraw();
	
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
	enemycharges_.clear();
	ReLoad();
}

void GameScene::ReLoad()
{
	// ���x���f�[�^�̓ǂݍ���
	levelData_ = LevelLoader::LoadFile("obj");

	//models.insert(std::make_pair(std::string("player"), playerModel_.get()));
	models.insert(std::make_pair(std::string("boss"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("enemySpawn"), enemyModel_.get()));
	models.insert(std::make_pair(std::string("enemySpawn2"), enemyModel2_.get()));
	models.insert(std::make_pair(std::string("filed"), filedModel_.get()));
	models.insert(std::make_pair(std::string("IventBlock"), cubeModel_.get()));
	models.insert(std::make_pair(std::string("FliedBlock"), filedCubeModel_.get()));
	models.insert(std::make_pair(std::string("wallBlock"), cubeModel_.get()));
	models.insert(std::make_pair(std::string("FliedT"), filedTentoModel_.get()));
	models.insert(std::make_pair(std::string("Fliedtou"), filedTouModel_.get()));


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

			eventBox_->Initialize(model, newObject[objSize_].get());
			objSize_++;
		}
		else if (levelData_->objects[i].fileName == "wallBlock") {
			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);

			//�I�u�W�F�N�g�����ƍ��W�����
			collBox[collSize_] = std::make_unique<CollisionBox>();

			collBox[collSize_]->Initialize(model, newObject[objSize_].get());
			collBox[collSize_]->SetScale(newObject[objSize_]->GetScale());

			collisions_.push_back(std::move(collBox[collSize_]));

			objSize_++;
			collSize_++;
		}
		else if (levelData_->objects[i].fileName == "enemySpawn") {

			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);

			//�I�u�W�F�N�g�����ƍ��W�����
			newEnemy[enemySize_] = std::make_unique<Enemy>();

			newEnemy[enemySize_]->SetShadow(shadowModel_.get());
			newEnemy[enemySize_]->Initialize(newObject[objSize_].get(), newObject[objSize_]->GetPosition(), player_.get());
			newEnemy[enemySize_]->SetBulletModel(cubeModel_.get());
			


			//�G��o�^����
			enemys_.push_back(std::move(newEnemy[enemySize_]));

			objSize_++;
			enemySize_++;
		}
		else if (levelData_->objects[i].fileName == "enemySpawn2") {

			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);

			//�I�u�W�F�N�g�����ƍ��W�����
			newEnemy2[enemySize2_] = std::make_unique<EnemyCharge>();

			newEnemy2[enemySize2_]->Initialize(newObject[objSize_].get(), newObject[objSize_]->GetPosition(), player_.get());
	
			//�G��o�^����
			enemycharges_.push_back(std::move(newEnemy2[enemySize2_]));

			objSize_++;
			enemySize2_++;
		}
		else if (levelData_->objects[i].fileName == "boss") {
			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);


			newObject[objSize_]->SetScale({ 15,15,15 });
			boss_->Initialize(model,newObject[objSize_]->GetPosition(), newObject[objSize_].get(), player_.get());
			boss_->SetBulletModel(cubeModel_.get());

			objSize_++;
		}
		else {
			//�I�u�W�F�N�g�����ƍ��W�����
			Inport(model, i);

			// �z��ɓo�^
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

	// ���W
	DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, levelData_->objects[size].translation);
	newObject[objSize_]->SetPosition(pos);

	// ��]�p
	DirectX::XMFLOAT3 rot;
	DirectX::XMStoreFloat3(&rot, levelData_->objects[size].rotation);
	newObject[objSize_]->SetRotation({ rot.x,rot.y,rot.z });


	// ���W
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
	playerObject_->SetPosition({ 0,0,0 });
	playerObject_->SetRotation({ 0,0,0 });
	playerObject_->SetCamera({ 0, 20, -30.0f }, { 0, 10, 0 });

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get(), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
	player_->SetBulletModel(playerBulletCubeModel_.get(), bulletObject_.get());

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

void GameScene::AllCollison()
{
	//�����蔻��
	XMFLOAT3 posA, posB;

	//���e���X�g�̎擾
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	posA = player_->GetWorldPos();

	posB = eventBox_->GetWorldPos();


	//A��B�̋���
	float r1 = 3.0f;	//�C�x���g�̃X�P�[��
	float r2 = 13.0f;	//���@�̃X�P�[��

	if (Collison(posA, posB, r1, r2)) {
		hitBox_ = true;
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

				if (!player_->Getinvincible()) {
					sound_->PlayWave("noise.wav");

				}
				player_->OnCollision();

				if (player_->GetHP() <= 1) {
					sound_->PlayWave("electric_shock3.wav");
				}
			}
		}
	}

	//�{�X�e�Ǝ��@�̓����蔻��
	for (const std::unique_ptr<BossBullet>& bullet : boss_->GetBullets()) {
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
		}
		else {
			coll->hit_ = false;
		}
	}

	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		//A��B�̋���
		float r1 = 3.0f;	//���@�̃X�P�[��
		float r2 = 175.0f;	//���G�͈͂̃X�P�[��

		posA = player_->GetWorldPos();
		posB = enemy->GetWorldPos();

		if (Collison(posA, posB, r1, r2)) {
			enemy->SetIsMove(true);
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

			if (enemy->IsDead()) {
				isEffect_ = true;
				startEffect_ = enemy->GetWorldPos();
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
