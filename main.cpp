#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "SpriteB.h"

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPI�̐����N���X
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyborad�N���X�̐���
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectX�̊�Ր����N���X
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//windowsAPI������
	winApp->Initalize();
	windows.reset(winApp);
	MSG msg{};

	//DirectX������
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	// �X�v���C�g�ÓI������
	SpriteB::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//keyborad������
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	XMFLOAT2 pos = { 0,0 };

	SpriteB::LoadTexture(0, L"Resources/test.png");

	SpriteB* sp1 = SpriteB::Create(0, { 0,0 });

	//OBJ���烂�f����ǂݍ���
	Model* model = Model::LoadFromObj("boss");
	Model* model2 = Model::LoadFromObj("world");

	//3D�I�u�W�F�N�g����
	Object3D* object3d = Object3D::Create(5.0f);
	Object3D* object3d2 = Object3D::Create(5.0f);
	Object3D* object3d3 = Object3D::Create(100.0f);

	//3D�I�u�W�F�N�g��3D���f����R�Â�
	object3d->SetModel(model);
	object3d2->SetModel(model);
	object3d3->SetModel(model2);

	object3d->SetPosition({ -20,0,+5 });
	object3d2->SetPosition({ +20,0,+5 });

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (winApp->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}
		object3d->Update();
		object3d2->Update();
		object3d3->Update();

		//keyborad�X�V����
		input_->Update();

		XMFLOAT3 pos3d;
		XMFLOAT3 pos3d2;

		pos3d = object3d->GetRotation();
		pos3d2 = object3d2->GetPosition();

		if (input_->keyPush(DIK_D)) {
			pos3d.y++;
		}
		if (input_->keyPush(DIK_A)) {
			pos3d.y--;
		}
		if (input_->keyPush(DIK_S)) {
			pos3d.x--;
		}
		if (input_->keyPush(DIK_W)) {
			pos3d.x++;
		}

		if (input_->keyPush(DIK_RIGHT)) {
			pos3d2.x++;
		}
		if (input_->keyPush(DIK_LEFT)) {
			pos3d2.x--;
		}
		if (input_->keyPush(DIK_UP)) {
			pos3d2.y++;
		}
		if (input_->keyPush(DIK_DOWN)) {
			pos3d2.y--;
		}

		object3d->SetRotation(pos3d);
		object3d2->SetPosition(pos3d2);

	/*	XMMATRIX matTrans;
		matTrans = XMMatrixTranslation(pos.x, pos.y, 0.0f);
		matWorld *= matTrans;

		constMapTransform->mat = matWorld * matProjection;*/

#pragma region DirectX���t���[������

		// �`��O����
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		object3d->Draw();
		object3d2->Draw();
		object3d3->Draw();

		Object3D::PostDraw();

		SpriteB::PreDraw(dxBasis->GetCommandList());

		sp1->Draw();

		SpriteB::PostDraw();

		//// �p�C�v���C���X�e�[�g�ݒ�
		//dxBasis->GetCommandList()->SetPipelineState(pipelineState.Get());

		//// ���[�g�V�O�l�`���̐ݒ�R�}���h
		//dxBasis->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		//// �v���~�e�B�u�`��̐ݒ�R�}���h
		//dxBasis->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

		//// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		//dxBasis->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		////�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
		//ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get()};
		//dxBasis->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		////SRV�q�[�v�̐擪�A�h���X���擾
		//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		////SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
		//dxBasis->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		////�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		//dxBasis->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

		//// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		//dxBasis->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		//// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
		//dxBasis->GetCommandList()->IASetIndexBuffer(&ibView);

		//// �`��R�}���h
		//dxBasis->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��


		

		//�`��㏈��
		dxBasis->PostDraw();


		
#pragma endregion

	}

	delete model;
	delete object3d;
	delete object3d2;
	delete object3d3;
	//�E�B���h�E�N���X��o�^����
	winApp->Release();

	return 0;
}
