#include "SceneManager.h"

SceneManager::~SceneManager()
{
	//scene_->Finalize();
	//delete scene_;
}

void SceneManager::Update()
{
	
}

void SceneManager::Draw()
{
	scene_->Draw();
}
