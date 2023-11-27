#include "GameCore.h"
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

//初期化
void GameCore::Initialize()
{
	Framework::Initialize();

	//gamescene_->Initialize();
	//gamescene_->SetStart(true);


	titlescene_->Initialize();

	postEffect_->SetDirectX(spBasis_, windows_, keyboard_);
	postEffect_->Initialize(0);

	/*postEffect2_->SetDirectX(spBasis_, windows_, keyboard_);
	postEffect2_->Initialize(2);*/
}

//後始末
void GameCore::Finalize()
{
	titlescene_->Finalize();
	//gamescene_->Finalize();
	Framework::Finalize();
}

//更新処理
void GameCore::Update()
{
	Framework::Update();

	//ノイズのエフェクト
	postEffect_->Update();

	titlescene_->Update();
	
	//デバックImGui
	imGuiM_->Begin();
	ImGui::Text("Editor");

	////オブジェクト読み込み直す
	//if (ImGui::Button("ReLoad")) {
	//	gamescene_->EditorLoad("obj");
	//}

	//if (ImGui::Button("DebugPoint")) {
	//	gamescene_->GetPlayer()->SetPos(gamescene_->GetDebugPoint()->GetWorldPos());
	//}

	imGuiM_->End();
}

//描画処理
void GameCore::Draw()
{
	//描画前準備
	postEffect_->PreDrawScene(directX_->GetCommandList());
	Object3D::PreDraw(directX_->GetCommandList());
	FbxObject3d::PreSet(directX_->GetCommandList());
	
	titlescene_->Draw();

	Object3D::PostDraw();

	//パーティクル描画前準備
	ParticleManager::PreDraw(directX_->GetCommandList());
	//パーティクル描画
//	gamescene_->pDraw();

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