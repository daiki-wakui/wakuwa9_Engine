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

	//for (int i = 0; i < 100; i++) {
	//	const float md_pos = 10.0f;
	//	XMFLOAT3 pos{};
	//	pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
	//	pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
	//	pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;

	//	const float md_vel = 0.1f;
	//	XMFLOAT3 vel{};
	//	vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

	//	XMFLOAT3 acc{};
	//	const float md_acc = 0.001f;
	//	acc.y = -(float)rand() / RAND_MAX * md_acc;

	//	particleMan_->Add(60, pos, vel, acc);
	//}
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

	Object3D::PostDraw();
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw(directX_->GetCommandList());
	
	gamescene_->pDraw();

	ParticleManager::PostDraw();
	
	postEffect_->PostDrawScene(directX_->GetCommandList());

	// 描画前処理
	directX_->PreDraw();

	postEffect_->Draw();

	imGuiM_->Draw();

	//描画後処理
	directX_->PostDraw();
}
