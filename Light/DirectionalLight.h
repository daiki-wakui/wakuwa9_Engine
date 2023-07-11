#pragma once

#include <DirectXMath.h>
#include <stdint.h>

//���s����
class DirectionalLight
{
private: //�G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
		uint32_t active;
	};

public: //�����o�֐�

	//���C�g�������Z�b�g
	inline void SetLightDir(const XMVECTOR& lightdir) { lightdir_ = DirectX::XMVector3Normalize(lightdir); }


	//���C�g�������擾
	inline const XMVECTOR& GetLightDir() { return lightdir_; }


	//���C�g�F���Z�b�g
	inline void SetLightColor(const XMFLOAT3& lightcolor) { lightcolor_ = lightcolor; }

	//���C�g�F���擾
	inline const XMFLOAT3& GetLightColor() { return lightcolor_; }

	//�L���t���O���Z�b�g
	inline void SetActive(bool active) { active_ = active; }


	//�L���`�F�b�N
	inline bool IsActive() { return active_; }

private: //�����o�ϐ�

	//���C�g�����i�P�ʃx�N�g���j
	XMVECTOR lightdir_ = { 1,0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor_ = { 1,1,1 };
	//�L���t���O
	bool active_ = false;
};

