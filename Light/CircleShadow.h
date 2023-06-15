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

	XMVECTOR dir_ = { 1,0,0,0 };

	float distanceCasterLight_ = 100.0f;

	XMFLOAT3 casterPos_ = { 0,0,0 };

	XMFLOAT3 atten_ = { 0.5f,0.6f,0.0f };

	XMFLOAT2 factorAngleCos_ = { 0.2f,0.5f };

	bool active_ = false;

public:	//�����o�֐�

	inline void SetDir(const XMVECTOR& dir) { dir_ = DirectX::XMVector3Normalize(dir); };
	inline const XMVECTOR& GetDir() { return dir_; };

	inline void SetCasterPos(const XMFLOAT3& casterPos) { casterPos_ = casterPos; };
	inline const XMFLOAT3& GetCasterPos() { return casterPos_; };

	inline void SetDistanceCasterLight(float distanceCasterLight) { distanceCasterLight_ = distanceCasterLight; };
	inline const float GetDistanceCasterLight() { return distanceCasterLight_; };

	inline void SetAtten(const XMFLOAT3& atten) { atten_ = atten; };
	inline const XMFLOAT3& GetAtten() { return atten_; };

	inline void SetFactorAngle(const XMFLOAT2& factorAngle) {
		factorAngleCos_.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		factorAngleCos_.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}

	inline const XMFLOAT2& GetFactorAngleCos() { return factorAngleCos_; };
	inline void SetActive(bool active) { active_ = active; };
	inline bool IsActive() { return active_; };
};

