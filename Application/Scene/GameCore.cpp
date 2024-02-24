#include "GameCore.h"
#include "SceneList.h"
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

//初期化
void GameCore::Initialize()
{
	Framework::Initialize();
	postEffect_->Initialize(0);

	sceneManager_->Initialize();
}

//後始末
void GameCore::Finalize()
{
	sceneManager_->Finalize();
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
	
	sceneManager_->Update();

	//ノイズのエフェクト
	postEffect_->Update(sceneManager_->GetGameScene()->GetPlayer());
	
	if (sceneManager_->GetSceneState() == TITLE) {
		postEffect_->SetIsEffect(true);
	}

	if (sceneManager_->ChangeToGameScene()) {

		if (sceneManager_->GetSceneState() == TITLE) {
			postEffect_->SetIsEffect(false);
		}
	}
	
	//デバックImGui
	imGuiM_->Begin();
	GameScene* g = sceneManager_->GetGameScene();

	if (ImGui::TreeNode("StageEditor")) {
		ImGui::Text("Editor");

		//オブジェクト読み込み直す
		if (ImGui::Button("ReLoad")) {
			g->EditorLoad("obj");
		}

		if (ImGui::Button("DebugPoint")) {
			g->GetPlayer()->SetPos(sceneManager_->GetGameScene()->GetDebugPoint()->GetWorldPos());
		}
		ImGui::TreePop();
	}
	

	if (ImGui::TreeNode("BossEditor")) {
		
		ImGui::Text("BossPosition");
		ImGui::Text("%f,%f,%f", g->GetBoss()->GetWorldPos().x, g->GetBoss()->GetWorldPos().y, g->GetBoss()->GetWorldPos().z);
		ImGui::Text("%f,%f,%f", g->GetBoss()->GetBossTailWorldPos().x, g->GetBoss()->GetBossTailWorldPos().y, g->GetBoss()->GetBossTailWorldPos().z);

		ImGui::Text("StateCount %d", g->GetBoss()->GetState());

		ImGui::TreePop();
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
	
	sceneManager_->Draw();

	Object3D::PostDraw();

	//パーティクル描画前準備
	ParticleManager::PreDraw(directX_->GetCommandList());
	
	//パーティクル描画
	sceneManager_->ParticleDraw();

	ParticleManager::PostDraw();
	
	//ここまでの描画にポストエフェクトをかける
	postEffect_->PostDrawScene(directX_->GetCommandList());
	
	//描画前処理
	directX_->PreDraw();

	//ポストエフェクトをかけた描画
	postEffect_->Draw();

	sceneManager_->OffEffectDraw();
	
	//imgui
	if (isDebug) {
		imGuiM_->Draw();
	}

	//描画後処理
	directX_->PostDraw();
}
