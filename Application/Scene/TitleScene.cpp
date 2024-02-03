#include "TitleScene.h"
#include "SceneList.h"
#include "SoundManager.h"
#include "Model3DManager.h"

//初期化
void TitleScene::Initialize()
{
	ConstValues();
	//3Dモデル生成
	skyObject_ = std::make_unique<Object3D>();
	skyObject_->SetModel(Model3DManager::GetInstance()->Get3DModel("world"));
	skyObject_->Initialize();
	skyObject_->SetScale({ m_json["SKYDOME_SCALE_X"],m_json["SKYDOME_SCALE_Y"],m_json["SKYDOME_SCALE_Z"] });
	//skyObject_->SetPosition(GetConstVectorValue("SKYDOME_POS"));

	isStartSE_ = false;

	titleUI_->TitleSceneInitialize();

	SoundManager::GetInstance()->SetBasis(sound_);
	SoundManager::GetInstance()->Initialize();

	Vector3 a;

	a = SetVec("b");
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
			//SoundManager::GetInstance()->PlayWave("Start.wav", GetConstValue("STARTSE_VOLUE"));
			isStartSE_ = true;
		}

		//シーン遷移終了
		if (titleUI_->GetSceneChangeAlpha() >= 1) {
			changeEnd_ = true;
			isStartSE_ = false;
			playBGM_ = false;
		}

		SoundManager::GetInstance()->StopBGM();
	}

	//天球
	//skyObject_->SetPosition(GetConstVectorValue("SKYDOME_POS"));

	//Vector3 eye = { GetConstVectorValue("CAMERA_EYE") };
	//Vector3 terget = { GetConstVectorValue("CAMERA_TERGET") };
	//skyObject_->SetCamera(eye, terget);
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

Vector3 TitleScene::SetVec(std::string name)
{
	Vector3 ans;

	nlohmann::json& vec3 = m_json["Vector3"];

	ans.x = vec3[name][0];
	ans.y = vec3[name][1];
	ans.z = vec3[name][2];

	return ans;
}

void TitleScene::ConstValues()
{
	std::string filenamea = "Resources/json/titleConst.json";

	//ファイルを読み込んで内容を画面に表示する
	//読み込みに失敗した場合はエラーを表示する
	std::ifstream ifs(filenamea.c_str());
	if (ifs.good())
	{
		ifs >> m_json;
	}
	else
	{
		float a = 0;
		a++;
	}
}

float TitleScene::GetConstValue(std::string name)
{
	float pikValue;
	pikValue = v_.at(name);

	return pikValue;
}

Vector3 TitleScene::GetConstVectorValue(std::string namex)
{
	std::string nameVec = namex;

	Vector3 result;
	result.x = v_.at(nameVec +"_X");
	result.y = v_.at(nameVec +"_Y");
	result.z = v_.at(nameVec +"_Z");

	return result;
}
