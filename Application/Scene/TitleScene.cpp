#include "TitleScene.h"
#include "SceneList.h"
#include "SoundManager.h"
#include "MyRandom.h"

//初期化
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

//後始末
void TitleScene::Finalize()
{
}

//更新処理
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


	popFTimer_++;
	if (popFTimer_ > 10) {
		ffpos_.x = MyRandom::GetFloatRandom(-200, 200);
		ffpos_.y = MyRandom::GetFloatRandom(-100, 50);
		ffpos_.z = MyRandom::GetFloatRandom(20, 150);

		ffv_.x = MyRandom::GetFloatRandom(-0.1f, 0.1f);
		ffv_.y = MyRandom::GetFloatRandom(-0.1f, 0.1f);
		ffv_.z = MyRandom::GetFloatRandom(-0.1f, 0.1f);

		std::unique_ptr<FieldEffect> newFF = std::make_unique<FieldEffect>();
		newFF->Initialize(ffpos_, {0,0,0}, 0.8f ,ffv_, poriTFModel_.get());
		fEffects_.push_back(std::move(newFF));
		popFTimer_ = 0;
	}

	//弾の更新処理
	for (std::unique_ptr<FieldEffect>& ff : fEffects_) {
		ff->Update();
	}

	//デスフラグが立った弾を削除
	fEffects_.remove_if([](std::unique_ptr<FieldEffect>& ff) {
		return ff->IsDead();
		});
}

//描画関数
void TitleScene::Draw()
{
	

	skyObject_->Draw();

	for (std::unique_ptr<FieldEffect>& ff : fEffects_) {
		ff->Draw();
	}

	titleUI_->TitleDraw();
}

//ポストエフェクトをかけない描画
void TitleScene::OffDraw()
{
	titleUI_->OffDraw();
}
