#include "SceneManager.h"

SceneManager::~SceneManager()
{
	
}

//初期化
void SceneManager::Initialize()
{
	gamescene_->Initialize();
	gamescene_->SetStart(true);
	titlescene_->Initialize();
}

//更新処理
void SceneManager::Update()
{
	if (keyboard_->keyInstantPush(DIK_SPACE) || gamePad_->PushInstantB()) {

		//タイトルシーンからシーン遷移開始
		if (nowScene_ == TITLE) {
			titlescene_->SetStart(true);
			gamescene_->SetChange(false);
			gamescene_->Reset();
		}
	}

	//シーン遷移可能なら変える処理
	if (ChangeToGameScene()) {
		nowScene_ = GAME;	//ゲームシーン
		gamescene_->SetStart(true);
	}

	if (ChangeToTitleScene()) {
		nowScene_ = TITLE;	
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
	if (nowScene_ == TITLE) {
		titlescene_->Update();
		
	}
	//ゲームシーンの更新処理
	else {
		gamescene_->Update();
	}
}


}

void SceneManager::Draw()
{
	//タイトルシーン描画
	if (nowScene_ == TITLE) {
		titlescene_->Draw();
	}
	//ゲームシーン描画
	else {
		gamescene_->Draw();
	}
}

//後始末
void SceneManager::Finalize()
{
	titlescene_->Finalize();
	gamescene_->Finalize();
}

//パーティクルの描画
void SceneManager::ParticleDraw()
{
	gamescene_->pDraw();
}

//ポストエフェクトをかけないスプライト描画
void SceneManager::OffEffectDraw()
{
	titlescene_->OffDraw();
}

//ゲームシーンに遷移するかの関数
bool SceneManager::ChangeToGameScene()
{
	if (titlescene_->GetChange()) {
		return true;
	}

	return false;
}

//タイトルシーンに遷移するかの関数
bool SceneManager::ChangeToTitleScene()
{
	if (gamescene_->GetChange()) {
		return true;
	}

	return false;
}
