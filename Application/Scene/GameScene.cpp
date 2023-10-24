#include "GameScene.h"
#include "Easing.h"
#include <random>
#include "SoundManager.h"

void GameScene::SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound)
{
	windows_ = windows;
	directX_ = directX;
	imGuiM_ = imGuiM;
	spBasis_ = spBasis;
	sound_ = sound;
}

void GameScene::Initialize()
{

	playerHP_ = spBasis_->TextureData(L"Resources/playerHP.png");
	bossHP_ = spBasis_->TextureData(L"Resources/red.png");

	title_ = spBasis_->TextureData(L"Resources/title.png");;
	gameover_ = spBasis_->TextureData(L"Resources/gameover.png");
	gameclear_ = spBasis_->TextureData(L"Resources/gameclear.png");

	//シーン遷移時
	scene_ = spBasis_->TextureData(L"Resources/sceneChange.png");

	//フィルター
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
	playerHPSprite_->Create(50, 30);
	playerHPSprite_->SetAncP({ 0,0 });

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
	sceneSprite_->SetSize({ 1280,720 });
	sceneSprite_->SetColor({ 1,1,1,1 });

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

	//OBJからモデルを読み込む
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

	drModel_ = std::make_unique<Model>();
	drModel_->LoadFromObj("doar");

	LeftDoorModel_ = std::make_unique<Model>();
	LeftDoorModel_->LoadFromObj("doar2");

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
	ChangeBox_ = std::make_unique<EventBox>();

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

	//FBXファイル読み込み
	testModel_ = std::make_unique<FbxModel>();
	FbxLoader::GetInstance()->LoadModelFromFile(testModel_.get(), "boneTest");

	testObj_ = std::make_unique<FbxObject3d>();
	testObj_->Initialize();
	testObj_->SetModel(testModel_.get());
	testObj_->StartAnimation();

	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("Alarm01.wav");
	sound_->LoadWave("NieR_freld2.wav");
	sound_->LoadWave("Alone.wav");
	sound_->LoadWave("Shot.wav");
	sound_->LoadWave("Hit.wav");
	sound_->LoadWave("noise.wav");
	sound_->LoadWave("Warning.wav");
	sound_->LoadWave("electric_shock3.wav");
	sound_->LoadWave("NieR_boss.wav");

	Reset();

	player_->Initialize(playerModel_.get(), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
	player_->SetBulletModel(cubeModel_.get(), bulletObject_.get());

	particleMan_->Initialize(L"Resources/effect1.png");
	particleMan_->Update();

	resetOn_ = false;
	isIvent_ = false;

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
	if (keyboard_->keyInstantPush(DIK_N)) {
		EditorLoad("obj");
		SoundManager::GetInstance()->SetFiledBGM(true);
	}

	//リセット
	if (isChangeStage_) {
		bossBGM_ = false;
		hitBox_ = false;

		EditorLoad("obj");
		SoundManager::GetInstance()->SetFiledBGM(true);

		isChangeStage_ = false;
	}

	if (ChangeAlpha_ == 0) {
		SoundManager::GetInstance()->Update(1);
	}

	ObjectUpdate();

	//スプライト更新処理
	SpriteUpdate();

	shadowObject_->SetPosition({ playerObject_->GetPosition().x,2,playerObject_->GetPosition().z });
	//shadowObject_->SetPosition({ playerObject_->GetPosition().x,5,pl);
	shadowObject_->Update();

	//弾の更新処理
	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Update();
	}

	//デスフラグが立った弾を削除
	effects_.remove_if([](std::unique_ptr<Effect>& bullet) {
		return bullet->IsDead();
	});
	
	XMFLOAT3 rocalEye = Object3D::GetEye();
	XMFLOAT3 rocalTarget = Object3D::GetTarget();

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

	//if (door_->GetIsArive()) {
	//	
	//}
	
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

	//レティクルUpdate
	if (!enemys_.empty()) {
		screenPos_ = enemys_.front()->GetObj()->Screen();

		spPos_.x = screenPos_.x;
		spPos_.y = screenPos_.y;

		sSprite_->SetPosition(spPos_);

		sSprite_->Update();

		player_->SetEnemy(enemys_.front().get());
	}
	
	if (isIvent_) {
		alpha_ += 0.05f;
		alpha_ = min(alpha_, 1);
		timer_++;

		iventEye_ = iventEye_.lerp(iventEye_, endEye_, Easing::EaseInCubic(timer_, maxTime_));

		XMFLOAT3 tmpEye;
		tmpEye.x = iventEye_.x;
		tmpEye.y = iventEye_.y;
		tmpEye.z = iventEye_.z;

		XMFLOAT3 target;
		target.x = iventTarget_.x;
		target.y = iventTarget_.y;
		target.z = iventTarget_.z;

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

	//enemyの死亡フラグ
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
	});

	//enemyの死亡フラグ
	enemycharges_.remove_if([](std::unique_ptr<EnemyCharge>& enemy) {
		return enemy->IsDead();
	});

	//敵の動き
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update(start_);
	}

	//敵の動き
	for (std::unique_ptr<EnemyCharge>& enemy : enemycharges_) {
		enemy->Update();
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
	dFilterSprite_->Update();
	sceneSprite_->Update();
	iventSprite_->Update();
	waringSprite_->Update();
	RBSprite_->Update();
	fillSprite_->Update();

	iventSprite_->SetColor({ 1,1,1,alpha_ });

	screenPosPlayer_ = playerObject_->Screen();
	//チュートリアルUI
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

	//ライフ危ない時に出るフィルター
	if (player_->GetHP() <= 1) {
		isLifeOne_ = true;
	}
	else {
		isLifeOne_ = false;
	}
	//ライフ危ない時に出るフィルター
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

	//シーン切り替え
	if (start_) {

		ChangeAlpha_ -= 0.05f;
		ChangeAlpha_ = max(ChangeAlpha_, 0);
		sceneSprite_->SetColor({ 1,1,1,ChangeAlpha_ });

		if (!resetOn_) {
			resetOn_ = true;
			Reset();
		}
	}

	if (hitBox_ && !isIvent_) {
		if (count_ < 3) {
			pow_++;
			wSize_.y = waringSprite_->GetSize().y;
		}
		else {
			pow_ = 0;
		}

		//ワーニング点滅
		if (pow_ > 2) {
			pow_ = 0;
			count_++;
		}

		//点滅後の動き
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

	//ボスHP、BGM鳴らし
	if (hitBox_ == true) {

		if (!bossBGM_) {
			SoundManager::GetInstance()->StopBGM();
			isIvent_ = true;
		}

		if (hitBox_ == true && boss_->GetArive() == true) {
			boss_->Update(isIvent_);
		}

		bossHPSprite_->SetSize({ 16.0f * (float)boss_->GetHP(),32.0f });
		bossHPSprite_->Update();
	}

	//左右レティクル
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

	if (player_->IsDead()) {
		gameoverSprite_->Draw(gameover_);
	}

	if (!boss_->GetArive()) {
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

	testObj_->Update();

	if (keyboard_->keyInstantPush(DIK_J)) {
		testObj_->StopAnimation();
	}

	if (keyboard_->keyInstantPush(DIK_K)) {
		testObj_->PlayAnimation();
	}
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

	//敵の動き
	for (std::unique_ptr<EnemyCharge>& enemy : enemycharges_) {
		enemy->Draw();
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
	
	//fbx
	//testObj_->Draw();

	SpriteDraw();
	
	//sSprite_->Draw(targetImage_);
	
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
	enemycharges_.clear();
	doors_.clear();
	ReLoad(filename);
}

void GameScene::ReLoad(const std::string filename)
{
	// レベルデータの読み込み
	levelData_ = LevelLoader::LoadFile(filename);

	models.insert(std::make_pair(std::string("player"), playerModel_.get()));
	models.insert(std::make_pair(std::string("debugpoint"), playerModel_.get()));
	models.insert(std::make_pair(std::string("boss"), enemyModel_.get()));
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



		if (levelData_->objects[i].fileName == "player") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			playerObject_->Initialize();
			playerObject_->SetScale(XMFLOAT3({ 1,1,1 }));
			playerObject_->SetPosition(newObject[objSize_]->GetPosition());
			playerObject_->SetRotation(newObject[objSize_]->GetRotation());
			playerObject_->SetCamera({ 0, 10, -30.0f }, { 0, 10, 0 });

			player_.reset();
			player_ = std::make_unique<Player>();
			player_->Initialize(playerModel_.get(), playerObject_.get(), keyboard_, gamePad_, podObject_.get());
			player_->SetBulletModel(playerBulletCubeModel_.get(), bulletObject_.get());
		}
		else if (levelData_->objects[i].fileName == "debugpoint") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			dPoint_->Initialize(model, newObject[objSize_].get());
			//eventBox_->Initialize(model, newObject[objSize_].get());
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
		else if (levelData_->objects[i].fileName == "enemySpawn2") {

			//オブジェクト生成と座標情報代入
			Inport(model, i);

			//オブジェクト生成と座標情報代入
			newEnemy2[enemySize2_] = std::make_unique<EnemyCharge>();

			newEnemy2[enemySize2_]->Initialize(newObject[objSize_].get(), newObject[objSize_]->GetPosition(), player_.get());
	
			//敵を登録する
			enemycharges_.push_back(std::move(newEnemy2[enemySize2_]));

			objSize_++;
			enemySize2_++;
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
	DirectX::XMFLOAT3 rocalPos;
	DirectX::XMStoreFloat3(&rocalPos, levelData_->objects[size].translation);
	newObject[objSize_]->SetPosition(rocalPos);

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
	SoundManager::GetInstance()->SetVolue(0);
	SoundManager::GetInstance()->SetFiledBGM(false);

	EditorLoad("d");

	for (std::unique_ptr<Door>& door : doors_) {
		door->SetTutorial(true);
	}
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
	//当たり判定
	XMFLOAT3 posA, posB;
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
			float r = r1 + r2;

			XMFLOAT3 dis;
			dis.x = posB.x - posA.x;
			dis.y = posB.y - posA.y;
			dis.z = posB.z - posA.z;


			if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r) && hitBox_ == true) {
				bullet->isDead_ = true;
				BulletEffect = true;
				boss_->OnCollision();
				sound_->PlayWave("Hit.wav", 0.5f);
			}

			if (BulletEffect) {
				for (int i = 0; i < 3; i++) {
					XMFLOAT3 tmppos = posA;

					const float md_vel = 10.0f;
					XMFLOAT3 vel{};
					vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
					vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
					vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

					XMFLOAT3 acc{};
					const float md_acc = 0.001f;
					acc.y = -(float)rand() / RAND_MAX * md_acc;

					particleMan_->Add(60, tmppos, vel, acc, 2.0f, 0.0f);
				}
				BulletEffect = false;
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
					sound_->PlayWave("electric_shock3.wav",1.1f);
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

	//
	posA = player_->GetWorldPos();

	for (const std::unique_ptr<CollisionBox>& coll : collisions_) {
		posB = coll->GetWorldPos();

		//AとBの距離
		r1 = 1.0f;
		r2 = coll->GetScale().x;
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
			float r = r1 + r2;

			XMFLOAT3 dis;
			dis.x = posB.x - posA.x;
			dis.y = posB.y - posA.y;
			dis.z = posB.z - posA.z;


			if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
				bullet->isDead_ = true;
				enemy->OnCollision();
				BulletEffect = true;
				sound_->PlayWave("Hit.wav",0.5f);
			}

			if (enemy->IsDead()) {
				isEffect_ = true;
				startEffect_ = enemy->GetWorldPos();
			}
		}

		if (BulletEffect) {
			for (int i = 0; i < 3; i++) {
				XMFLOAT3 tmpPos = posA;

				const float md_vel = 10.0f;
				XMFLOAT3 vel{};
				vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
				vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
				vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

				XMFLOAT3 acc{};
				const float md_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * md_acc;

				particleMan_->Add(60, tmpPos, vel, acc, 2.0f, 0.0f);
			}
			BulletEffect = false;
		}
	}
}
