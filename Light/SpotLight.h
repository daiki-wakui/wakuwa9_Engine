#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <stdint.h>

class SpotLight
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

public:	//�T�u�N���X

	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightactoranglecos;
		uint32_t active;
		float pad4;
	};

private: //�����o�ϐ�
	//����
	XMVECTOR lightdir_ = { 1,0,0,0 };
	//���W
	XMFLOAT3 lightpos_ = { 0,0,0 };
	//�F
	XMFLOAT3 lightcolor_ = { 1,1,1 };
	//���������W��
	XMFLOAT3 lightAtten_ = { 1.0f,1.0f,1.0f };
	//�����p�x
	XMFLOAT2 lightFactorAngleCos_ = { 0.5f,0.2f };
	//�L���t���O
	bool active_ = false;

public: //�����o�֐�

	inline void SetLightDir(const XMVECTOR& lightdir) { lightdir_ = DirectX::XMVector3Normalize(lightdir); };
	inline const XMVECTOR& GetLightDir() { return lightdir_; };

	inline void SetLightPos(const XMFLOAT3& lightpos) { lightpos_ = lightpos; };
	inline const XMFLOAT3& GetLightPos() { return lightpos_; };

	inline void SetLightColor(const XMFLOAT3& lightcolor) { lightcolor_ = lightcolor; };
	inline const XMFLOAT3& GetLightColor() { return lightcolor_; };

	inline void SetLightAtten(const XMFLOAT3& lightatten) { lightAtten_ = lightatten; };
	inline const XMFLOAT3& GetLightAtten() { return lightAtten_; };

	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle) {
		lightFactorAngleCos_.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		lightFactorAngleCos_.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	};

	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos_; };
	inline void SetActive(bool active) { active_ = active; };
	inline bool IsActive() { return active_; };
};

