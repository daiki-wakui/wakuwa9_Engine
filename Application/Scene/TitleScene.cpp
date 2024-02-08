#include "TitleScene.h"
#include "SceneList.h"
#include "SoundManager.h"
#include "Model3DManager.h"

#include "SceneManager.h"


//初期化
void TitleScene::Initialize()
{
	json_ = std::make_unique<constJsonValue>();
	json_->LoadConstValue("Resources/json/titleConst.json");

	//3Dモデル生成
	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(Model3DManager::GetInstance()->Get3DModel("world"));
	skyObject_->Initialize();
	skyObject_->SetScale(json_->LoadVector3("SKYDOME_SCALE"));
	skyObject_->SetPosition(json_->LoadVector3("SKYDOME_POS"));

	isStartSE_ = false;
	titleUI_->TitleSceneInitialize();

	SoundManager::GetInstance()->SetBasis(sound_);
	SoundManager::GetInstance()->Initialize();
}

//後始末
void TitleScene::Finalize()
{
}

//更新処理
void TitleScene::Update()
{
	if (keyboard_->keyInstantPush(DIK_G)) {
		sceneManager_->ChangeScene("GAME");
	}

	titleUI_->TitleUpdate(changeStart_);

	SoundManager::GetInstance()->Update(TITLE);

	//シーン遷移開始
	if (changeStart_) {
		//SE1回鳴らす
		if (!isStartSE_) {
			SoundManager::GetInstance()->PlayWave("Start.wav", json_->LoadFloat("STARTSE_VOLUE"));
			isStartSE_ = true;
		}

		//シーン遷移終了
		if (titleUI_->GetSceneChangeAlpha() >= json_->LoadFloat("MAX_ALPHA")) {
			changeEnd_ = true;
			isStartSE_ = false;
			playBGM_ = false;
		}

		SoundManager::GetInstance()->StopBGM();
	}

	//天球
	skyObject_->SetPosition(json_->LoadVector3("SKYDOME_POS"));
	skyObject_->SetCamera(json_->LoadVector3("CAMERA_EYE"), json_->LoadVector3("CAMERA_TERGET"));
	skyObject_->Update(false);
}

//描画関数
void TitleScene::Draw()
{
	skyObject_->Draw();
	titleUI_->TitleDraw();
}

//ポストエフェクトをかけない描画
void TitleScene::OffDraw()
{
	titleUI_->OffDraw();
}