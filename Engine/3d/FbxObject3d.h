#pragma once
#include "FbxModel.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include <cstdint>

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
	static const int32_t MAX_BONES = 32;

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
	void SetModel(FbxModel* model) { model_ = model; }

	static void PreSet(ID3D12GraphicsCommandList* cmdList);

	void PlayAnimation();

protected: //�����o�ϐ�

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransfrom_;

	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin_;

	XMFLOAT3 scale_ = { 1,1,1 };

	XMFLOAT3 rotation_ = { 0,0,0 };

	XMFLOAT3 positon_ = { 0,0,0 };

	XMMATRIX matWorld_ = {};

	FbxModel* model_ = nullptr;

	//��t���[���̎���
	FbxTime frameTime_;

	FbxTime startTime_;

	FbxTime endTime_;

	FbxTime currentTime_;

	bool isPlay_ = false;

public:	//�ÓI�����o�֐�

	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::sDevice = device; }

	static void SetCamera(XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) {
		FbxObject3d::sEye = eye,
		FbxObject3d::sTarget = target,
		FbxObject3d::sUp = up;
	}

	//�O���t�B�b�N�X�p�C�v���C��
	static void CreateGraphicsPipeline();

	//���[�g�V�O�l�N�`��
	static ComPtr<ID3D12RootSignature> sRootsignature;

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> sPipelinestate;

	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* sCmdList;

	static void InitializeCamera(int32_t window_width, int32_t window_height);

	static void StaticInitialize(ID3D12Device* device, int32_t window_width, int32_t window_height);


	void SetPosition(const XMFLOAT3& position) { positon_ = position; }
	void SetRotation(const XMFLOAT3& rotation) { rotation_ = rotation; }
	void SetScale(const XMFLOAT3& scale) { scale_ = scale; }


private:	//�ÓI�����o�ϐ�

	static ID3D12Device* sDevice;

	// �r���[�s��
	static XMMATRIX sMatView;
	// �ˉe�s��
	static XMMATRIX sMatProjection;

	// ���_���W
	static XMFLOAT3 sEye;
	// �����_���W
	static XMFLOAT3 sTarget;
	// ������x�N�g��
	static XMFLOAT3 sUp;

};

