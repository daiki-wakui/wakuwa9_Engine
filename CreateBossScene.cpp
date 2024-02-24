#include "CreateBossScene.h"

void CreateBossScene::Initialize()
{
	bossObject_ = std::make_unique<Object3D>();
	bossObject_->SetModel(bossModel_.get());
	bossObject_->Initialize();

	player_->Initialize(playerModel_.get(), bossObject_.get(), keyboard_, gamePad_, bossObject_.get());

	boss_->Initialize(bossModel_.get(), { 0,0,0 }, bossObject_.get(), player_.get());
}

void CreateBossScene::Finalize()
{
}

void CreateBossScene::Update()
{
	player_->Update();
	//boss_->Update(true);
}

void CreateBossScene::Draw()
{
	player_->Draw();
	boss_->Draw();
}
