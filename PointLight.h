#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

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
		unsigned int active;
	};

public:  //�����o�ϐ�

	XMFLOAT3 lightpos = { 0,0,0 };
	XMFLOAT3 lightcolor = { 1,1,1 };
	//���C�g�̋��������W��
	XMFLOAT3 lightAtten = { 1.0f,1.0f,1.0f };
	//�L���t���O
	bool active = false;

public: //�����o�֐�
	inline void SetLightPos(const XMFLOAT3& lightpos) { this->lightpos = lightpos; };
	inline const XMFLOAT3& GetLightPos() { return lightpos; };

	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; };
	inline const XMFLOAT3& GetLightcolor() { return lightcolor; };

	inline void SetLightAtten(const XMFLOAT3& lightAtten) { this->lightAtten = lightAtten; };
	inline const XMFLOAT3& GetLightAtten() { return lightAtten; };

	inline void SetActive(bool active) { this->active = active; };
	inline bool IsActive() { return active; };
};
