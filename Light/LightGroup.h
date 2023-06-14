#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include <cstdint>

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
	static const int32_t DirLightNum = 3;
	static const int32_t PointLightNum = 3;
	static const int32_t SpotLightNum = 3;
	static const int32_t CircleShadowNum = 1;

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
		//�ۉe
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
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
	void SetDirLightActive(int32_t index, bool active);

	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(int32_t index, const XMVECTOR& lightdir);

	//���s�����̃��C�g�F���Z�b�g
	void SetDirLightColor(int32_t index, const XMFLOAT3& lightcolor);

	//�_����
	void SetPointLightActive(int32_t index, bool active);

	void SetPointLightPos(int32_t index, const XMFLOAT3& lightpos);

	void SetPointLightColor(int32_t index, const XMFLOAT3& lightcolor);

	void SetPointLightAtten(int32_t index, const XMFLOAT3& lightatten);


	//�X�|�b�g���C�g
	void SetSpotLightActive(int32_t index, bool active);

	void SetSpotLightDir(int32_t index, const XMVECTOR& lightdir);

	void SetSpotLightPos(int32_t index, const XMFLOAT3& lightpos);

	void SetSpotLightColor(int32_t index, const XMFLOAT3& lightcolor);

	void SetSpotLightAtten(int32_t index, const XMFLOAT3& lightAtten);

	void SetSpotLightFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle);


	//�ۉe
	void SetCircleShadowActive(int32_t index,bool active);

	void SetCircleShadowCasterPos(int32_t index, const XMFLOAT3& casterPos);

	void SetCircleShadowDir(int32_t index, const XMVECTOR& lightdir);

	void SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight);

	void SetCircleShadowAtten(int32_t index, const XMFLOAT3& lightAtten);

	void SetCircleShadowFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle);


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

	//�ۉe�̔z��
	CircleShadow circleShadows[CircleShadowNum];

	//�_�[�e�B�t���O
	bool dirty = false;
};


