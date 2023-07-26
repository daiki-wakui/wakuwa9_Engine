#include "GameCore.h"
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

void GameCore::Initialize()
{
	Framework::Initialize();

	gamescene_->SetBasis(windows_, directX_, imGuiM_, spBasis_, sound_);
	gamescene_->SetInputInfo(keyboard_, gamePad_);
	gamescene_->Initialize();

	titlescene_->SetBasis(windows_, directX_, imGuiM_, spBasis_, sound_);
	titlescene_->SetInputInfo(keyboard_, gamePad_);
	titlescene_->Initialize();

	
	postEffect_->SetDirectX(spBasis_, windows_, keyboard_);
	postEffect_->Initialize(0);

	gaussianEffect_->SetDirectX(spBasis_, windows_, keyboard_);
	gaussianEffect_->Initialize(1);

	//TitleScene::SetBasis(windows_, directX_, imGuiM_, spBasis_, sound_);
	//TitleScene::SetInputInfo(keyboard_, gamePad_);

	//Framework::sceneManager_->SetNextScene(scene.get());
	//Framework::sceneManager_->SetNextScene(scene);

	particleMan_->Initialize();
	particleMan_->Update();
}

void GameCore::Finalize()
{
	titlescene_->Finalize();
	gamescene_->Finalize();
	Framework::Finalize();
}

void GameCore::Update()
{
	Framework::Update();

	// カメラ移動
	if (keyboard_->keyPush(DIK_W) || keyboard_->keyPush(DIK_A) || keyboard_->keyPush(DIK_S) || keyboard_->keyPush(DIK_D))
	{
		if (keyboard_->keyPush(DIK_W)) { ParticleManager::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
		else if (keyboard_->keyPush(DIK_S)) { ParticleManager::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
		if (keyboard_->keyPush(DIK_D)) { ParticleManager::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
		else if (keyboard_->keyPush(DIK_A)) { ParticleManager::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }
	}

	particleMan_->Update();

	postEffect_->Update(gamescene_->GetPlayer());

	if (keyboard_->keyInstantPush(DIK_SPACE)) {
		state++;
		postEffect_->SetIsEffect(false);

		if (state > 1) {
			state = 0;
		}
	}

	if (keyboard_->keyInstantPush(DIK_T)) {
		state = 0;
	}

	if (state == 0) {
		titlescene_->Update();
		//postEffect_->SetIsEffect(true);
	}
	else {
		gamescene_->Update();

	}
	
	imGuiM_->Begin();
	ImGui::Text("Editor");

	if (ImGui::Button("ReLoad")) {
		gamescene_->EditorLoad();
	}

	imGuiM_->End();
}

void GameCore::Draw()
{

	postEffect_->PreDrawScene(directX_->GetCommandList());
	
	Object3D::PreDraw(directX_->GetCommandList());
	FbxObject3d::PreSet(directX_->GetCommandList());

	if (state == 0) {
		titlescene_->Draw();
	}
	else {
		gamescene_->Draw();
	}
	
	
	
	//Framework::sceneManager_->Draw();

	Object3D::PostDraw();

	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw(directX_->GetCommandList());
	particleMan_->Draw();
	ParticleManager::PostDraw();
	
	postEffect_->PostDrawScene(directX_->GetCommandList());

	// 描画前処理
	directX_->PreDraw();

	postEffect_->Draw();

	imGuiM_->Draw();

	//描画後処理
	directX_->PostDraw();
}
