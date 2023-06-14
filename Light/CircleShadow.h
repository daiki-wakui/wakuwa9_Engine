#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <stdint.h>

//丸影
class CircleShadow
{
private:// エイリアス

	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//サブクラス

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

private:	//メンバ変数

	XMVECTOR dir = { 1,0,0,0 };

	float distanceCasterLight = 100.0f;

	XMFLOAT3 casterPos = { 0,0,0 };

	XMFLOAT3 atten = { 0.5f,0.6f,0.0f };

	XMFLOAT2 factorAngleCos = { 0.2f,0.5f };

	bool active = false;

public:	//メンバ関数

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

