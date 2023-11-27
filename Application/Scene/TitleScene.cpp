#include "TitleScene.h"

void TitleScene::Initialize()
{
	titleUI_->Initialize();

	skydomModel_ = std::make_unique<Model>();
	skydomModel_->LoadFromObj("cube");

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomModel_.get());
	skyObject_->Initialize();
	//skyObject_->SetScale(Vector3({ 400,400,400 }));
	skyObject_->SetPosition({ 0,10,0 });

	isStartSE_ = false;

	SoundManager::GetInstance()->SetBasis(sound_);
	SoundManager::GetInstance()->Initialize();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	titleUI_->TitleUpdate(changeStart_);

	skyObject_->Update();
}

void TitleScene::Draw()
{
	skyObject_->Draw();
	//titleUI_->Draw();
}

void TitleScene::OffDraw()
{
	titleUI_->OffDraw();
}
