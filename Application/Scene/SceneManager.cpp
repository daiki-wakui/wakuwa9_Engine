#include "SceneManager.h"
#include "SceneList.h"

//デストラクタ
SceneManager::~SceneManager()
{
	scene_->Finalize();
	delete scene_;
}

//初期化
void SceneManager::Initialize()
{

}

//更新処理
void SceneManager::Update()
{
	if (nextScene_) {
		if (scene_) {
			scene_->Finalize();
			delete scene_;
			//.reset
		}

		//unique_ move
		scene_ = nextScene_;
		nextScene_ = nullptr;


		scene_->SetSceneManager(this);

		scene_->Initialize();
	}

	scene_->Update();
}

//描画関数
void SceneManager::Draw()
{
	scene_->Draw();
}

//後始末
void SceneManager::Finalize()
{
	
}
