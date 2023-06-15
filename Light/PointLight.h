#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <stdint.h>

//�_����
class PointLight
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

public:

	struct ConstBufferData
	{
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		uint32_t active;
	};

public:  //�����o�ϐ�

	XMFLOAT3 lightpos_ = { 0,0,0 };
	XMFLOAT3 lightcolor_ = { 1,1,1 };
	//���C�g�̋��������W��
	XMFLOAT3 lightAtten_ = { 1.0f,1.0f,1.0f };
	//�L���t���O
	bool active_ = false;

public: //�����o�֐�
	inline void SetLightPos(const XMFLOAT3& lightpos) { lightpos_ = lightpos; };
	inline const XMFLOAT3& GetLightPos() { return lightpos_; };

	inline void SetLightColor(const XMFLOAT3& lightcolor) { lightcolor_ = lightcolor; };
	inline const XMFLOAT3& GetLightcolor() { return lightcolor_; };

	inline void SetLightAtten(const XMFLOAT3& lightAtten) { lightAtten_ = lightAtten; };
	inline const XMFLOAT3& GetLightAtten() { return lightAtten_; };

	inline void SetActive(bool active) { active_ = active; };
	inline bool IsActive() { return active_; };
};

