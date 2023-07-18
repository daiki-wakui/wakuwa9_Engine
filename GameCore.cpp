#include "GameCore.h"
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

void GameCore::Initialize()
{
	Framework::Initialize();

	gamescene_->SetBasis(windows_, directX_, imGuiM_, spBasis_, sound_);
	gamescene_->SetInputInfo(keyboard_, gamePad_);
	gamescene_->Initialize();

	
	postEffect_->SetDirectX(spBasis_, windows_, keyboard_);
	postEffect_->Initialize(0);

	gaussianEffect_->SetDirectX(spBasis_, windows_, keyboard_);
	gaussianEffect_->Initialize(1);

	//TitleScene::SetBasis(windows_, directX_, imGuiM_, spBasis_, sound_);
	//TitleScene::SetInputInfo(keyboard_, gamePad_);

	//Framework::sceneManager_->SetNextScene(scene.get());
	//Framework::sceneManager_->SetNextScene(scene);

}

void GameCore::Finalize()
{
	gamescene_->Finalize();
	Framework::Finalize();
}

void GameCore::Update()
{
	Framework::Update();

	gamescene_->Update();

	imGuiM_->Begin();

	imGuiM_->End();
}

void GameCore::Draw()
{

	postEffect_->PreDrawScene(directX_->GetCommandList());
	
	Object3D::PreDraw(directX_->GetCommandList());
	FbxObject3d::PreSet(directX_->GetCommandList());

	gamescene_->Draw();

	
	//Framework::sceneManager_->Draw();

	Object3D::PostDraw();

	
	
	postEffect_->PostDrawScene(directX_->GetCommandList());

	// 描画前処理
	directX_->PreDraw();

	postEffect_->Draw();

	imGuiM_->Draw();

	//描画後処理
	directX_->PostDraw();
}
