#include "SceneManager.h"

SceneManager::~SceneManager()
{
	
}

void SceneManager::Initialize()
{
	gamescene_->Initialize();
	gamescene_->SetStart(true);
	titlescene_->Initialize();
}

void SceneManager::Update()
{
	//タイトルシーンからシーン遷移開始
	if (keyboard_->keyInstantPush(DIK_SPACE) || gamePad_->PushInstantB()) {

		if (state == 0) {
			titlescene_->SetStart(true);
			gamescene_->SetChange(false);
			gamescene_->Reset();
		}
	}


	if (ChangeToGameScene()) {
		state = 1;	//ゲームシーン
		gamescene_->SetStart(true);
	}


	//タイトルシーンに戻る
	if (keyboard_->keyInstantPush(DIK_T)) {
		titlescene_->SetStart(false);
		gamescene_->SetStart(false);
		titlescene_->SetChange(false);
		gamescene_->SetChange(true);
		gamescene_->Reset();
	}

	//タイトルシーンの更新処理
	if (state == 0) {
		titlescene_->Update();
		
	}
	//ゲームシーンの更新処理
	else {
		gamescene_->Update();
	}

	if (gamescene_->GetChange()) {
		state = 0;
	}


}

void SceneManager::Draw()
{
	//タイトルシーン描画
	if (state == 0) {
		titlescene_->Draw();
	}
	//ゲームシーン描画
	else {
		gamescene_->Draw();
	}
}

void SceneManager::Finalize()
{
	titlescene_->Finalize();
	gamescene_->Finalize();
}

void SceneManager::ParticleDraw()
{
	gamescene_->pDraw();
}

void SceneManager::OffEffectDraw()
{
	titlescene_->OffDraw();
}

bool SceneManager::ChangeToGameScene()
{
	if (titlescene_->GetChange()) {
		return true;
	}

	return false;
}
