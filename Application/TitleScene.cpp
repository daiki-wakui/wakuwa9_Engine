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
	tex1_ = spBasis_->TextureData(L"Resources/backTitle.png");

	title_ = spBasis_->TextureData(L"Resources/title.png");;

	scene_ = spBasis_->TextureData(L"Resources/scene.png");

	spBasis_->TextureSetting();


	titleSprite_->Initialize(spBasis_, windows_);
	titleSprite_->Create(640, 360);
	titleSprite_->SetSize({ 1280,720 });
	titleSprite_->Update();

	sceneSprite_->Initialize(spBasis_, windows_);
	sceneSprite_->Create(640, 360);
	sceneSprite_->SetSize({ 0,0 });
	sceneSprite_->Update();


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
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	titleSprite_->Update();

	if (!playBGM_) {
		sound_->PlayLoopWave("PerituneMaterial.wav",0.5f);
		playBGM_ = true;
	}

	if (start_) {

		if (power < 2) {
			sound_->PlayWave("Start.wav");
		}

		sound_->StopWAVE("PerituneMaterial.wav");

		pos = sceneSprite_->GetSize();

		power+=2;

		pos.x += power;
		pos.y += power;

		if (pos.y > 1920) {
			pos.y = 1920;
			pos.x = 1920;
			change_ = true;
			start_ = false;
		}
	}

	sceneSprite_->SetSize(pos);
	

	sceneSprite_->Update();

	skyObject_->Update();
}

void TitleScene::Draw()
{
	//titleSprite_->Draw(tex1_);

	skyObject_->Draw();

	titleSprite_->Draw(title_);
	sceneSprite_->Draw(scene_);
	
}
