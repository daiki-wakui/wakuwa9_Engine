#include "TitleScene.h"
#include "SceneList.h"
#include "SoundManager.h"
#include "Model3DManager.h"

//初期化
void TitleScene::Initialize()
{
	//3Dモデル生成
	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(Model3DManager::GetInstance()->Get3DModel("world"));
	skyObject_->Initialize();
	skyObject_->SetScale(SKYDOME_SCALE);
	skyObject_->SetPosition(SKYDOME_POS);

	isStartSE_ = false;

	titleUI_->TitleSceneInitialize();

	SoundManager::GetInstance()->SetBasis(sound_);
	SoundManager::GetInstance()->Initialize();

	
	std::string filenamea = "Resources/json/normal.json";

	//ファイルを読み込んで内容を画面に表示する
	//読み込みに失敗した場合はエラーを表示する
	std::ifstream ifs(filenamea.c_str());
	if (ifs.good())
	{
		nlohmann::json m_json;
		ifs >> m_json;

		

		v_.insert(std::make_pair("speed", m_json["speed"]));
		v_.insert(std::make_pair("add", m_json["add"]));

		//読み込んだデータをそれぞれの変数に代入する
		float speed;
		speed = m_json["speed"];
		float firingangle;
		firingangle = m_json["add"];
	}
	else
	{
		float a = 0;
		a++;
	}
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
