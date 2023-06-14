#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <stdint.h>

//�ۉe
class CircleShadow
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
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanseCasterLight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 factorAngleCos;
		uint32_t active;
		float pad4;
	};

private:	//�����o�ϐ�

	XMVECTOR dir = { 1,0,0,0 };

	float distanceCasterLight = 100.0f;

	XMFLOAT3 casterPos = { 0,0,0 };

	XMFLOAT3 atten = { 0.5f,0.6f,0.0f };

	XMFLOAT2 factorAngleCos = { 0.2f,0.5f };

	bool active = false;

public:	//�����o�֐�

	inline void SetDir(const XMVECTOR& dir) { this->dir = DirectX::XMVector3Normalize(dir); };
	inline const XMVECTOR& GetDir() { return dir; };

	inline void SetCasterPos(const XMFLOAT3& casterPos) { this->casterPos = casterPos; };
	inline const XMFLOAT3& GetCasterPos() { return casterPos; };

	inline void SetDistanceCasterLight(float distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; };
	inline const float GetDistanceCasterLight() { return distanceCasterLight; };

	inline void SetAtten(const XMFLOAT3& atten) { this->atten = atten; };
	inline const XMFLOAT3& GetAtten() { return atten; };

	inline void SetFactorAngle(const XMFLOAT2& factorAngle) {
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}

	inline const XMFLOAT2& GetFactorAngleCos() { return factorAngleCos; };
	inline void SetActive(bool active) { this->active = active; };
	inline bool IsActive() { return active; };
};

