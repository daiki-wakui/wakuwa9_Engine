#include "TitleScene.h"

void TitleScene::Initialize()
{
	titleImage_ = spBasis_->TextureData(L"Resources/title.png");
	titleUIImage_ = spBasis_->TextureData(L"Resources/titleUI.png");

	sceneChangeImage_ = spBasis_->TextureData(L"Resources/sceneChange.png");

	filterImage_ = spBasis_->TextureData(L"Resources/fillter.png");

	testImage_ = spBasis_->TextureData(L"Resources/testB.png");

	spBasis_->TextureSetting();

	titleSprite_->Initialize(titleImage_);
	titleSprite_->Create();
	titleSprite_->SetSize({ 1280,720 });
	titleSprite_->SetAncP({ 0,0 });

	titleUISprite_->Initialize(titleUIImage_);
	titleUISprite_->Create();
	titleUISprite_->SetSize({ 1280,720 });
	titleUISprite_->SetAncP({ 0,0 });

	sceneSprite_->Initialize();
	sceneSprite_->Create(640, 360);
	sceneSprite_->SetSize({ 1280,720 });
	sceneSprite_->SetColor({ 1,1,1,0 });

	fillSprite_->Initialize();
	fillSprite_->Create(640, 360);
	fillSprite_->SetSize({ 1280,720 });
	fillSprite_->Update();

	skydomModel_ = std::make_unique<Model>();
	skydomModel_->LoadFromObj("world2");

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(XMFLOAT3({ 400,400,400 }));
	skyObject_->SetPosition({ 0,0,100 });

	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("Start.wav");
	sound_->LoadWave("Alarm01.wav");
	sound_->LoadWave("NieR_Title.wav");

	isStartSE_ = false;

	SoundManager::GetInstance()->SetBasis(sound_);
	SoundManager::GetInstance()->Initialize();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	titleSprite_->Update();
	titleUISprite_->Update();

	SoundManager::GetInstance()->Update(0);

	/*if (!playBGM_) {
		sound_->PlayLoopWave("NieR_Title.wav");
		playBGM_ = true;
	}*/

	if (changeStart_) {

		if (!isStartSE_) {
			sound_->PlayWave("Start.wav",0.25f);
			isStartSE_ = true;
		}

		ChangeAlpha_ += 0.05f;
		ChangeAlpha_ = min(ChangeAlpha_, 1);
		sceneSprite_->SetColor({1, 1, 1, ChangeAlpha_});

		if (ChangeAlpha_ >= 1) {
			changeEnd_ = true;
			isStartSE_ = false;
			playBGM_ = false;
		}

		sound_->StopWAVE("NieR_Title.wav");
	}
	else {
		ChangeAlpha_ = 0;
		sceneSprite_->SetColor({ 1, 1, 1, ChangeAlpha_ });
	}

	sceneSprite_->Update();

	skyObject_->SetPosition({ 0,0,100 });
	skyObject_->SetCamera({ 0, 20, -30.0f }, { 0, 10, 0 });
	skyObject_->Update();
}

void TitleScene::Draw()
{

	skyObject_->Draw();

	titleSprite_->Draw(titleImage_);
	titleUISprite_->Draw(titleUIImage_);
	sceneSprite_->Draw(sceneChangeImage_);
	
}

void TitleScene::OffDraw()
{
	fillSprite_->Draw(filterImage_);
}
