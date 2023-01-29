#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

//���C�g
class LightGroup
{
private:// �G�C���A�X

	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�萔
	static const int DirLightNum = 3;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;

public: //�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
		//�X�|�b�g���C�g�p
		SpotLight::ConstBufferData spotLights[SpotLightNum];
	};

public: // �ÓI�����o�֐�

	//�ÓI������
	static void StaticInitialize(ID3D12Device* device);

	
	//�C���X�^���X����
	static LightGroup* Create();

private: //�ÓI�����o�ϐ�

	//�f�o�C�X
	static ID3D12Device* device;

public: //�����o�֐�
	
	//������
	void Initialize();

	
	//�X�V
	void Update();

	
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	
	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	//�W���̃��C�g�ݒ�
	void DefaultLightSetting();

	//�����̃��C�g�F���Z�b�g
	void SetAmbientColor(const XMFLOAT3& color);

	//���s�����̗L���t���O���Z�b�g
	void SetDirLightActive(int index, bool active);

	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(int index, const XMVECTOR& lightdir);

	//���s�����̃��C�g�F���Z�b�g
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);

	//�_����
	void SetPointLightActive(int index, bool active);

	void SetPointLightPos(int index, const XMFLOAT3& lightpos);

	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);

	void SetPointLightAtten(int index, const XMFLOAT3& lightatten);


	//�X�|�b�g���C�g
	void SetSpotLightActive(int index, bool active);

	void SetSpotLightDir(int index, const XMVECTOR& lightdir);

	void SetSpotLightPos(int index, const XMFLOAT3& lightpos);

	void SetSpotLightColor(int index, const XMFLOAT3& lightcolor);

	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);

	void SetSpotLightFactorAngle(int index, XMFLOAT2& lightFactorAngle);

private: //�����o�ϐ�

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	//�����̐F
	XMFLOAT3 ambientColor = { 1,1,1 };

	//���s�����̔z��
	DirectionalLight dirLights[DirLightNum];

	//�_�����̔z��
	PointLight pointLights[PointLightNum];

	//�X�|�b�g���C�g�̔z��
	SpotLight spotLights[SpotLightNum];

	//�_�[�e�B�t���O
	bool dirty = false;
};


