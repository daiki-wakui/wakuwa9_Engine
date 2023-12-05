#include "TitleScene.h"
#include "SceneList.h"
#include "SoundManager.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	//3Dモデル読み込み
	Model3DManager::LoadTitle3DModel();
	//3Dモデル生成
	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(skydomTitleModel_.get());
	skyObject_->Initialize();
	skyObject_->SetScale(SKYDOME_SCALE);
	skyObject_->SetPosition(SKYDOME_POS);

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

	SoundManager::GetInstance()->Update(TITLE);

	//シーン遷移開始
	if (changeStart_) {
		//SE1回鳴らす
		if (!isStartSE_) {
			SoundManager::GetInstance()->PlayWave("Start.wav", STARTSE_VOLUE);
			isStartSE_ = true;
		}

		//シーン遷移終了
		if (titleUI_->GetSceneChangeAlpha() >= MAX_ALPHA) {
			changeEnd_ = true;
			isStartSE_ = false;
			playBGM_ = false;
		}

		SoundManager::GetInstance()->StopBGM();
	}

	//天球
	skyObject_->SetPosition(SKYDOME_POS);
	skyObject_->SetCamera(CAMERA_EYE, CAMERA_TERGET);
	skyObject_->Update(false);

	if (keyboard_->keyInstantPush(DIK_G)) {
		sceneManager_->ChangeScene("GAME");
	}
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
