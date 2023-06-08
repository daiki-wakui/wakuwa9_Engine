#pragma once
#include "FbxModel.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3d
{
protected:	//�G�C���A�X

	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataTransform
	{
		//XMMATRIX mat;	// �R�c�ϊ��s��

		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		XMFLOAT3 cameraPos;	//�J�������W(���[���h���W)
	};

	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct CounstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: //�����o�֐�

	//������
	void Initialize();

	//���t���[��
	void Update();

	void Draw();

	//���f���̃Z�b�g
	void SetModel(FbxModel* model) { this->model = model; }

	static void PreSet(ID3D12GraphicsCommandList* cmdList);

protected: //�����o�ϐ�

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransfrom;

	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;

	XMFLOAT3 scale = { 1,1,1 };

	XMFLOAT3 rotation = { 0,0,0 };

	XMFLOAT3 positon = { 0,0,0 };

	XMMATRIX matWorld;

	FbxModel* model = nullptr;

public:	//�ÓI�����o�֐�

	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }

	static void SetCamera(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up) {
		FbxObject3d::eye = eye,
		FbxObject3d::target = target,
		FbxObject3d::up = up;
	}

	//�O���t�B�b�N�X�p�C�v���C��
	static void CreateGraphicsPipeline();

	//���[�g�V�O�l�N�`��
	static ComPtr<ID3D12RootSignature> rootsignature;

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList_;

	static void InitializeCamera(int window_width, int window_height);

	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);


	void SetPosition(const XMFLOAT3& position) { this->positon = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }


private:	//�ÓI�����o�ϐ�

	static ID3D12Device* device;

	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;

	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;

};

