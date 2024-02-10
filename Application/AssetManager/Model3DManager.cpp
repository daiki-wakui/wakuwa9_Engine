#include "Model3DManager.h"

//コンストラクタ
Model3DManager::Model3DManager()
{
}

//デストラクタ
Model3DManager::~Model3DManager()
{
}

//タイトルシーンで使うモデル読み込み
void Model3DManager::LoadTitle3DModel()
{
	skydomTitleModel_ = std::make_unique<Model>();
	skydomTitleModel_->LoadFromObj("world2");

	poriTFModel_ = std::make_unique<Model>();
	poriTFModel_->LoadFromObj("poriF");
}

//ゲームシーンで使うモデル読み込み
void Model3DManager::LoadGame3DModel()
{
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

	poriFModel_ = std::make_unique<Model>();
	poriFModel_->LoadFromObj("poriF");
	

	poriHitFModel_ = std::make_unique<Model>();
	poriHitFModel_->LoadFromObj("hitf");

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
