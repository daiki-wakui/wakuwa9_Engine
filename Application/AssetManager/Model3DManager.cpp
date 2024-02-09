#include "Model3DManager.h"

Model3DManager* Model3DManager::GetInstance()
{
	static Model3DManager instance;

	return &instance;
}

void Model3DManager::insertModel()
{
	models_.insert(std::make_pair("world", skydomTitleModel_.get()));
	models_.insert(std::make_pair("player", playerModel_.get()));
	models_.insert(std::make_pair("playerBit", podModel_.get()));
	models_.insert(std::make_pair("worldGame", skydomGameModel_.get()));
	models_.insert(std::make_pair("enemy", enemyModel_.get()));
	models_.insert(std::make_pair("redCube", cubeModel_.get()));
	models_.insert(std::make_pair("whiteCube", playerBulletCubeModel_.get()));
	models_.insert(std::make_pair("blackCube", filedCubeModel_.get()));
	models_.insert(std::make_pair("filedFloor", filedModel_.get()));
	models_.insert(std::make_pair("filedArch", filedTentoModel_.get()));
	models_.insert(std::make_pair("filedTower", filedTouModel_.get()));
	models_.insert(std::make_pair("effctModel", konnpeModel_.get()));
	models_.insert(std::make_pair("bulletEffctModel", poriModel_.get()));
	models_.insert(std::make_pair("shadow", shadowModel_.get()));
	models_.insert(std::make_pair("rightDoar", drModel_.get()));
	models_.insert(std::make_pair("leftDoar", LeftDoorModel_.get()));
	models_.insert(std::make_pair("bossFiled", bossFiledModel_.get()));
	models_.insert(std::make_pair("bossFiledGate", bossFiledGateModel_.get()));
	models_.insert(std::make_pair("bossBody", bossModel_.get()));
	models_.insert(std::make_pair("bossBullet", bossBulletModel_.get()));
	models_.insert(std::make_pair("bossTail", frameModel_.get()));
}

//ゲームシーンで使うモデル読み込み
void Model3DManager::LoadGame3DModel()
{
	//OBJからモデルを読み込む
	skydomTitleModel_->LoadFromObj("world2");
	playerModel_->LoadFromObj("player");
	podModel_->LoadFromObj("pod");
	skydomGameModel_->LoadFromObj("world");
	enemyModel_->LoadFromObj("enemySou");
	enemyModel2_->LoadFromObj("enemySou2");
	cubeModel_->LoadFromObj("cube");
	playerBulletCubeModel_->LoadFromObj("cube3");
	filedCubeModel_->LoadFromObj("cube2");
	filedModel_->LoadFromObj("filed");
	filedTentoModel_->LoadFromObj("nier_obj");
	filedTouModel_->LoadFromObj("nier_filed");
	konnpeModel_->LoadFromObj("konnpe");
	poriModel_->LoadFromObj("pori");
	shadowModel_->LoadFromObj("cirshadow");
	drModel_->LoadFromObj("doar");
	LeftDoorModel_->LoadFromObj("doar2");
	bossFiledModel_->LoadFromObj("bossfiled");
	bossFiledGateModel_->LoadFromObj("bossfiledfront");
	bossModel_->LoadFromObj("bossbody");
	bossBulletModel_->LoadFromObj("bossbullet");
	frameModel_->LoadFromObj("b_tele");
	bulletCononModel_->LoadFromObj("bossconon");
}

Model* Model3DManager::Get3DModel(std::string name)
{
	Model* pikModel;

	pikModel = models_.at(name);

	return pikModel;
}
