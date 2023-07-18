#include "SceneManager.h"

SceneManager::~SceneManager()
{
	//scene_->Finalize();
	//delete scene_;
}

void SceneManager::Update()
{
	if (nextScene_) {

		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}

		scene_ = nextScene_;

		nextScene_ = nullptr;

		
		scene_->Initialize();
	}

	//���s�V�[���̍X�V
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}
