#include "TitleScene.h"

void TitleScene::SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound)
{
	windows_ = windows;
	directX_ = directX;
	imGuiM_ = imGuiM;
	spBasis_ = spBasis;
	sound_ = sound;
}

void TitleScene::SetInputInfo(KeyBoard* keyboard, GamePad* gamePad)
{
	keyboard_ = keyboard;
	gamePad_ = gamePad;
}

void TitleScene::Initialize()
{
	titleImage_ = spBasis_->TextureData(L"Resources/title.png");;

	sceneChangeImage_ = spBasis_->TextureData(L"Resources/scene.png");

	filterImage_ = spBasis_->TextureData(L"Resources/fillter.png");

	testImage_ = spBasis_->TextureData(L"Resources/testB.png");

	spBasis_->TextureSetting();

	titleSprite_->Initialize(spBasis_, windows_, titleImage_);
	titleSprite_->Create();
	titleSprite_->SetSize({ 1280,720 });
	titleSprite_->SetAncP({ 0,0 });

	sceneSprite_->Initialize(spBasis_, windows_);
	sceneSprite_->Create(640, 360);
	sceneSprite_->SetSize({ 0,0 });

	fillSprite_->Initialize(spBasis_, windows_);
	fillSprite_->Create(640, 360);
	fillSprite_->SetSize({ 1280,720 });
	fillSprite_->Update();

	skydomModel_ = std::make_unique<Model>();
	skydomModel_->LoadFromObj("world");

	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(XMFLOAT3({ 400,400,400 }));
	skyObject_->SetPosition({ 0,0,100 });

	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("Start.wav");
	sound_->LoadWave("Alarm01.wav");
	sound_->LoadWave("NieR_Title.wav");
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	titleSprite_->Update();

	if (!playBGM_) {
		sound_->PlayLoopWave("NieR_Title.wav",0.75f);
		playBGM_ = true;
	}

	if (changeStart_) {

		if (addSize_ < 2) {
			sound_->PlayWave("Start.wav");
		}

		sound_->StopWAVE("NieR_Title.wav");

		changeSize_ = sceneSprite_->GetSize();

		addSize_+=2;

		changeSize_.x += addSize_;
		changeSize_.y += addSize_;

		if (changeSize_.y > 1920) {
			changeSize_.y = 1920;
			changeSize_.x = 1920;
			changeEnd_ = true;
			changeStart_ = false;
			playBGM_ = false;
			addSize_ = 1;
		}
	}

	sceneSprite_->SetSize(changeSize_);
	
	if (changeEnd_) {
		changeSize_ = { 0,0 };
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

	sceneSprite_->Draw(sceneChangeImage_);
	
}

void TitleScene::OffDraw()
{
	fillSprite_->Draw(filterImage_);
}
