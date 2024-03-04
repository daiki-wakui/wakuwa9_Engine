#include "GameCore.h"
#include "SceneList.h"
#include "Model3DManager.h"

#include "SceneFactory.h"

#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

//初期化
void GameCore::Initialize()
{
	Framework::Initialize();
	postEffect_->Initialize(0);

	Model3DManager* m = Model3DManager::GetInstance();
	m->LoadGame3DModel();
	m->insertModel();

	//BaseScene* scene = new TitleScene();
	sceneFactory_ = new SceneFactory();
	sceneManager_->SetSceneFactory(sceneFactory_);
	sceneManager_->ChangeScene("TITLE");

	//sceneManager_->SetNextScene(scene);
}

//後始末
void GameCore::Finalize()
{
	delete sceneFactory_;
	Framework::Finalize();
}

//更新処理
void GameCore::Update()
{
#ifdef _DEBUG
	if (keyboard_->keyInstantPush(DIK_P)) {
		isDebug++;
		isDebug = isDebug % 2;
	}
#endif // _DEBUG
	

	Framework::Update();

	if (sceneManager_->GetNowSccene() == "TITLE") {
		postEffect_->SetIsEffect(true);
	}

	postEffect_->Update();

	//デバックImGui
	imGuiM_->Begin();
	ImGui::Text("Editor");

	//オブジェクト読み込み直す
	/*if (ImGui::Button("ReLoad")) {
		sceneManager_->GetGameScene()->EditorLoad("obj");
	}*/

	/*if (ImGui::Button("DebugPoint")) {
		sceneManager_->GetGameScene()->GetPlayer()->SetPos(sceneManager_->GetGameScene()->GetDebugPoint()->GetWorldPos());
	}*/

	imGuiM_->End();
}

//描画処理
void GameCore::Draw()
{
	//描画前準備
	postEffect_->PreDrawScene(directX_->GetCommandList());
	Object3D::PreDraw(directX_->GetCommandList());
	FbxObject3d::PreSet(directX_->GetCommandList());
	
	sceneManager_->Draw();

	Object3D::PostDraw();

	//パーティクル描画前準備
	ParticleManager::PreDraw(directX_->GetCommandList());
	

	ParticleManager::PostDraw();
	
	//ここまでの描画にポストエフェクトをかける
	postEffect_->PostDrawScene(directX_->GetCommandList());
	
	//描画前処理
	directX_->PreDraw();

	//ポストエフェクトをかけた描画
	postEffect_->Draw();

	
	//imgui
	if (isDebug) {
		imGuiM_->Draw();
	}

	//描画後処理
	directX_->PostDraw();
}