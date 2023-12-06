#include "Model3DManager.h"

Model3DManager::Model3DManager()
{
}

Model3DManager::~Model3DManager()
{
}

void Model3DManager::LoadTitle3DModel()
{
	//タイトル
	skydomTitleModel_ = std::make_unique<Model>();
	skydomTitleModel_->LoadFromObj("world2");


}

void Model3DManager::LoadGame3DModel()
{
	//ゲーム
//OBJからモデルを読み込む
	playerModel_ = std::make_unique<Model>();
	playerModel_->LoadFromObj("player");

	podModel_ = std::make_unique<Model>();
	podModel_->LoadFromObj("pod");

	skydomGameModel_ = std::make_unique<Model>();
	skydomGameModel_->LoadFromObj("world");

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

	bossFiledModel_ = std::make_unique<Model>();
	bossFiledModel_->LoadFromObj("bossfiled");

	bossFiledGateModel_ = std::make_unique<Model>();
	bossFiledGateModel_->LoadFromObj("bossfiledfront");

	bossModel_ = std::make_unique<Model>();
	bossModel_->LoadFromObj("bossbody");

	bossBulletModel_ = std::make_unique<Model>();
	bossBulletModel_->LoadFromObj("bossbullet");

	frameModel_->LoadFromObj("b_tele");
	bulletCononModel_->LoadFromObj("bossconon");
}
