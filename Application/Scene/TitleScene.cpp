#include "TitleScene.h"
#include "SoundManager.h"

void TitleScene::Initialize()
{
	
	Model3DManager::Load3DModel();

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomTitleModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(Vector3({ 400,400,400 }));
	skyObject_->SetPosition({ 0,0,100 });

	isStartSE_ = false;

	titleUI_->TitleSceneInitialize();

	SoundManager::GetInstance()->SetBasis(sound_);
	SoundManager::GetInstance()->Initialize();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	titleUI_->TitleUpdate(changeStart_);

	SoundManager::GetInstance()->Update(0);

	if (changeStart_) {

		if (!isStartSE_) {
			SoundManager::GetInstance()->PlayWave("Start.wav", 0.25f);
			isStartSE_ = true;
		}

		if (titleUI_->GetSceneChangeAlpha() >= 1) {
			changeEnd_ = true;
			isStartSE_ = false;
			playBGM_ = false;
		}

		SoundManager::GetInstance()->StopBGM();
	}

	skyObject_->SetPosition({ 0,0,100 });
	skyObject_->SetCamera({ 0, 20, -30.0f }, { 0, 10, 0 });
	skyObject_->Update(false);
}

void TitleScene::Draw()
{

	skyObject_->Draw();


	titleUI_->TitleDraw();
}

void TitleScene::OffDraw()
{
	titleUI_->OffDraw();
}
