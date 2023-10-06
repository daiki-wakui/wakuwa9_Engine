#include "GameCore.h"
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

//������
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

	/*postEffect2_->SetDirectX(spBasis_, windows_, keyboard_);
	postEffect2_->Initialize(2);*/

	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("ElectricWild.wav");
}

//��n��
void GameCore::Finalize()
{
	titlescene_->Finalize();
	gamescene_->Finalize();
	Framework::Finalize();
}

//�X�V����
void GameCore::Update()
{
	Framework::Update();

	//�m�C�Y�̃G�t�F�N�g
	postEffect_->Update(gamescene_->GetPlayer());

	//�^�C�g���V�[������V�[���J�ڊJ�n
	if (keyboard_->keyInstantPush(DIK_SPACE) || gamePad_->PushInstantB()) {
		titlescene_->SetStart(true);
		gamescene_->SetChange(false);
	}

	//�^�C�g���V�[������Q�[���V�[����
	if (titlescene_->GetChange()) {
		if (state == 0) {
			postEffect_->SetIsEffect(false);
		}
		state = 1;	//�Q�[���V�[��
		gamescene_->SetStart(true);
		
	}

	//�^�C�g���V�[���ɖ߂�
	if (keyboard_->keyInstantPush(DIK_T)) {
		titlescene_->SetStart(false);
		gamescene_->SetStart(false);
		titlescene_->SetChange(false);
		gamescene_->SetChange(true);
		gamescene_->Reset();
	}

	//�^�C�g���V�[���̍X�V����
	if (state == 0) {
		titlescene_->Update();
		postEffect_->SetIsEffect(true);
	}
	//�Q�[���V�[���̍X�V����
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
	
	//�f�o�b�NImGui
	imGuiM_->Begin();
	ImGui::Text("Editor");

	//�I�u�W�F�N�g�ǂݍ��ݒ���
	if (ImGui::Button("ReLoad")) {
		gamescene_->EditorLoad();
	}

	imGuiM_->End();
}

//�`�揈��
void GameCore::Draw()
{
	//�`��O����
	postEffect_->PreDrawScene(directX_->GetCommandList());
	Object3D::PreDraw(directX_->GetCommandList());
	FbxObject3d::PreSet(directX_->GetCommandList());
	
	//�^�C�g���V�[���`��
	if (state == 0) {
		titlescene_->Draw();
	}
	//�Q�[���V�[���`��
	else {
		gamescene_->Draw();
	}

	Object3D::PostDraw();

	//�p�[�e�B�N���`��O����
	ParticleManager::PreDraw(directX_->GetCommandList());
	//�p�[�e�B�N���`��
	gamescene_->pDraw();

	ParticleManager::PostDraw();
	
	//�����܂ł̕`��Ƀ|�X�g�G�t�F�N�g��������
	postEffect_->PostDrawScene(directX_->GetCommandList());


	
	
	
	//�`��O����
	directX_->PreDraw();

	//�|�X�g�G�t�F�N�g���������`��
	postEffect_->Draw();

	titlescene_->OffDraw();

	//imgui
	if (isDebug) {
		imGuiM_->Draw();
	}
	

	//�`��㏈��
	directX_->PostDraw();
}