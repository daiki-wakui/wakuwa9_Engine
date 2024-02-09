#include "LevelEditor.h"

//初期化
void LevelEditor::Initialize()
{
	//3Dオブジェクト生成
	playerObject_ = std::make_unique<Object3D>();
	playerObject_->SetModel(m->Get3DModel("player"));

	podObject_ = std::make_unique<Object3D>();
	podObject_->SetModel(m->Get3DModel("playerBit"));
	podObject_->Initialize();
}

void LevelEditor::Finalize()
{
	models.clear();
	objects.clear();
}

//更新処理
void LevelEditor::Update()
{
	for (std::unique_ptr<BaseObject>& objects_ : gameObjects_) {
		objects_->Update();
	}

	for (std::unique_ptr<Door>& door : doors_) {
		door->Update();
	}

	if (dPoint_->GetIsArive()) {
		dPoint_->Update();
	}
}

//描画関数
void LevelEditor::Darw()
{
	for (std::unique_ptr<BaseObject>& objects_ : gameObjects_) {
		objects_->Draw();
	}

	for (std::unique_ptr<Door>& door : doors_) {
		door->Draw();
	}
}

void LevelEditor::EditorLoad(const std::string filename)
{
	objects.clear();
	doors_.clear();
	gameObjects_.clear();
	ReLoad(filename);

	for (std::unique_ptr<Door>& door : doors_) {
		door->SetTutorial(true);
	}

	auto be = gameObjects_.begin();
	auto end = gameObjects_.end();
	for (auto iter = be; iter != end; iter++) {
		if (iter->get()->GetName() == "player") {
			player_ = iter->get();
		}
	}
}

void LevelEditor::ReLoad(const std::string filename)
{
	// レベルデータの読み込み
	levelData_ = LevelLoader::LoadFile(filename);

	models.insert(std::make_pair(std::string("player"), m->Get3DModel("player")));
	models.insert(std::make_pair(std::string("debugpoint"), m->Get3DModel("player")));
	models.insert(std::make_pair(std::string("boss"), m->Get3DModel("bossBody")));
	models.insert(std::make_pair(std::string("enemySpawn"), m->Get3DModel("enemy")));
	models.insert(std::make_pair(std::string("enemyc"), m->Get3DModel("enemy")));
	models.insert(std::make_pair(std::string("enemySpawn2"), m->Get3DModel("enemy")));
	models.insert(std::make_pair(std::string("filed"), m->Get3DModel("filedFloor")));
	models.insert(std::make_pair(std::string("IventBlock"), m->Get3DModel("redCube")));
	models.insert(std::make_pair(std::string("changeBlock"), m->Get3DModel("redCube")));
	models.insert(std::make_pair(std::string("FliedBlock"), m->Get3DModel("blackCube")));
	models.insert(std::make_pair(std::string("wallBlock"), m->Get3DModel("redCube")));
	models.insert(std::make_pair(std::string("FliedT"), m->Get3DModel("filedArch")));
	models.insert(std::make_pair(std::string("Fliedtou"), m->Get3DModel("filedTower")));
	models.insert(std::make_pair(std::string("dr"), m->Get3DModel("rightDoar")));
	models.insert(std::make_pair(std::string("d"), m->Get3DModel("leftDoar")));
	models.insert(std::make_pair(std::string("bossf"), m->Get3DModel("bossFiled")));
	models.insert(std::make_pair(std::string("bossf2"), m->Get3DModel("bossFiledGate")));

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

			podObject_->SetPosition(playerObject_->GetPosition());

			//オブジェクト生成と座標情報代入
			baseObject_[objNum_] = std::make_unique<PlayerBasis>();
			baseObject_[objNum_]->SetInfo(newObject[objSize_]->GetPosition(), { 1,0,0 });
			baseObject_[objNum_]->Initialize(m->Get3DModel("player"), playerObject_.get());
			//オブジェクトを登録する
			gameObjects_.push_back(std::move(baseObject_[objNum_]));
			objNum_++;

			//オブジェクト生成と座標情報代入
			baseObject_[objNum_] = std::make_unique<BitBasis>();
			baseObject_[objNum_]->Initialize(m->Get3DModel("playerBit"), podObject_.get());
			//オブジェクトを登録する
			gameObjects_.push_back(std::move(baseObject_[objNum_]));
			objNum_++;
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
			newObject[objSize_]->SetPosition({ rocalPos.x + DOOR_POS_VOLUE_X,rocalPos.y,rocalPos.z });

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

			//オブジェクト生成と座標情報代入
			baseObject_[objNum_] = std::make_unique<BossMovieBox>();
			baseObject_[objNum_]->Initialize(model, newObject[objSize_].get());
			//オブジェクトを登録する
			gameObjects_.push_back(std::move(baseObject_[objNum_]));
			objNum_++;

			//eventBox_->Initialize(model, newObject[objSize_].get());
			objSize_++;
		}
		else if (levelData_->objects[i].fileName == "changeBlock") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			//オブジェクト生成と座標情報代入
			baseObject_[objNum_] = std::make_unique<BossMovieBox>();
			baseObject_[objNum_]->Initialize(model, newObject[objSize_].get());
			//オブジェクトを登録する
			gameObjects_.push_back(std::move(baseObject_[objNum_]));
			objNum_++;

			//ChangeBox_->Initialize(model, newObject[objSize_].get(), true);
			objSize_++;
		}
		else if (levelData_->objects[i].fileName == "enemySpawn") {

			//オブジェクト生成と座標情報代入
			Inport(model, i);

			//オブジェクト生成と座標情報代入
			baseObject_[objNum_] = std::make_unique<EnemyBasis>();
			baseObject_[objNum_]->Initialize(model, newObject[objSize_].get());
			//オブジェクトを登録する
			gameObjects_.push_back(std::move(baseObject_[objNum_]));
			objNum_++;

			//敵を登録する
			
			objSize_++;
			enemySize_++;
		}
		else if (levelData_->objects[i].fileName == "enemyc") {

			//オブジェクト生成と座標情報代入
			Inport(model, i);

			//オブジェクト生成と座標情報代入
			baseObject_[objNum_] = std::make_unique<EnemyBasis>();
			baseObject_[objNum_]->Initialize(model, newObject[objSize_].get());
			//オブジェクトを登録する
			gameObjects_.push_back(std::move(baseObject_[objNum_]));
			objNum_++;


			objSize_++;
			enemySize_++;
		}
		else if (levelData_->objects[i].fileName == "boss") {
			//オブジェクト生成と座標情報代入
			Inport(model, i);


			newObject[objSize_]->SetScale(BOSS_SCALE);
			//boss_->Initialize(model, newObject[objSize_]->GetPosition(), newObject[objSize_].get(), player_.get());
			//boss_->SetBulletModel(m->Get3DModel("bossBullet"));
		//	boss_->SetBossModels(m->Get3DModel("bossTail"));

			//オブジェクト生成と座標情報代入
			baseObject_[objNum_] = std::make_unique<BossBasis>();
			baseObject_[objNum_]->Initialize(model, newObject[objSize_].get());
			//オブジェクトを登録する
			gameObjects_.push_back(std::move(baseObject_[objNum_]));
			objNum_++;


			objSize_++;
		}
		else {
			//オブジェクト生成と座標情報代入
			Inport(model, i);

			////オブジェクト生成と座標情報代入
			baseObject_[objNum_] = std::make_unique<StageObject>();
			baseObject_[objNum_]->Initialize(model, newObject[objSize_].get());
			////オブジェクトを登録する
			gameObjects_.push_back(std::move(baseObject_[objNum_]));
			objNum_++;

			// 配列に登録
			//objects.push_back(newObject[objSize_].get());
			objSize_++;
		}
	}
}

void LevelEditor::Inport(Model* model, int32_t size)
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
