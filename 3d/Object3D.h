#pragma once

#include "Model.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "DirectionalLight.h"
#include "LightGroup.h"
#include <cstdint>

class Object3D
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		//XMMATRIX mat;	// �R�c�ϊ��s��

		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		XMFLOAT3 cameraPos;	//�J�������W(���[���h���W)
	};

private: // �萔

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	static void StaticInitialize(ID3D12Device* device, int32_t window_width, int32_t window_height);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Object3D* Create(float scale);

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetEye() { return sEye; }

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetEye(XMFLOAT3& eye);

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetTarget() { return sTarget; }

	static const XMFLOAT3& GetUp() { return sUp; }

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetTarget(XMFLOAT3& target);

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveVector(XMFLOAT3& move);

	/// <summary>
	/// ���C�g�̃Z�b�g
	/// </summary>
	/*static void SetLight(DirectionalLight* light) {
		Object3D::light = light;
	}*/

	static void SetLightGroup(LightGroup* lightGroup) {
		Object3D::sLightGroup = lightGroup;
	}

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* sDevice;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* sCmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> sRootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> sPipelinestate;
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

	//���C�g
	//static DirectionalLight* light;
	static LightGroup* sLightGroup;

private:// �ÓI�����o�֐�
	

	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void InitializeCamera(int32_t window_width, int32_t window_height);

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// ���f���쐬
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

public: // �����o�֐�
	bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() const { return position_; }
	const XMFLOAT3& GetRotation() const { return rotation_; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const XMFLOAT3& position) { this->position_ = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->rotation_ = rotation; }

	//setter
	void SetModel(Model* model) { model_ = model; };

private: // �����o�ϐ�

	//3D���f��(�؂�Ă���)
	Model* model_ = nullptr;


	// �s��p�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0_; 
	// ���[�J���X�P�[��
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position_ = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld_ = {};
	// �e�I�u�W�F�N�g
	Object3D* parent_ = nullptr;

};

