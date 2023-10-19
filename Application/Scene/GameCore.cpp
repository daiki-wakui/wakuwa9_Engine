#include "GameCore.h"
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

//初期化
void GameCore::Initialize()
{
	Framework::Initialize();

	gamescene_->SetBasis(windows_, directX_, imGuiM_, spBasis_, sound_);
	gamescene_->Initialize();

	titlescene_->SetBasis(windows_, directX_, imGuiM_, spBasis_, sound_);
	titlescene_->Initialize();

	postEffect_->SetDirectX(spBasis_, windows_, keyboard_);
	postEffect_->Initialize(0);

	/*postEffect2_->SetDirectX(spBasis_, windows_, keyboard_);
	postEffect2_->Initialize(2);*/

	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("ElectricWild.wav");
}

//後始末
void GameCore::Finalize()
{
	titlescene_->Finalize();
	gamescene_->Finalize();
	Framework::Finalize();
}

//更新処理
void GameCore::Update()
{
	Framework::Update();

	//ノイズのエフェクト
	postEffect_->Update(gamescene_->GetPlayer());

	//タイトルシーンからシーン遷移開始
	if (keyboard_->keyInstantPush(DIK_SPACE) || gamePad_->PushInstantB()) {
		titlescene_->SetStart(true);
		gamescene_->SetChange(false);
		gamescene_->Reset();
	}

	//タイトルシーンからゲームシーンへ
	if (titlescene_->GetChange()) {
		if (state == 0) {
			postEffect_->SetIsEffect(false);
		}
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
		postEffect_->SetIsEffect(true);
	}
	//ゲームシーンの更新処理
	else {
		gamescene_->Update();
	}

	if (gamescene_->GetChange()) {
		state = 0;
	}

	if (keyboard_->keyInstantPush(DIK_P)) {
		isDebug++;
		isDebug = isDebug % 2;
	}
	
	//デバックImGui
	imGuiM_->Begin();
	ImGui::Text("Editor");

	//オブジェクト読み込み直す
	if (ImGui::Button("ReLoad")) {
		gamescene_->EditorLoad("obj");
	}

	if (ImGui::Button("DebugPoint")) {
		gamescene_->GetPlayer()->SetPos(gamescene_->GetDebugPoint()->GetWorldPos());
	}

	imGuiM_->End();
}

//描画処理
void GameCore::Draw()
{
	//描画前準備
	postEffect_->PreDrawScene(directX_->GetCommandList());
	Object3D::PreDraw(directX_->GetCommandList());
	FbxObject3d::PreSet(directX_->GetCommandList());
	
	//タイトルシーン描画
	if (state == 0) {
		titlescene_->Draw();
	}
	//ゲームシーン描画
	else {
		gamescene_->Draw();
	}

	Object3D::PostDraw();

	//パーティクル描画前準備
	ParticleManager::PreDraw(directX_->GetCommandList());
	//パーティクル描画
	gamescene_->pDraw();

	ParticleManager::PostDraw();
	
	//ここまでの描画にポストエフェクトをかける
	postEffect_->PostDrawScene(directX_->GetCommandList());


	
	
	
	//描画前処理
	directX_->PreDraw();

	//ポストエフェクトをかけた描画
	postEffect_->Draw();

	titlescene_->OffDraw();

	//imgui
	if (isDebug) {
		imGuiM_->Draw();
	}
	

	//描画後処理
	directX_->PostDraw();
}