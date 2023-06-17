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

//ライト
class LightGroup
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

public: //定数
	static const int32_t sDIRLIGHT_NUM = 3;
	static const int32_t sPOINTLIGHT_NUM = 3;
	static const int32_t sSPOTLIGHT_NUM = 3;
	static const int32_t sCIRCLESHADOW_NUM = 1;

public: //サブクラス

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[sDIRLIGHT_NUM];
		//点光源用
		PointLight::ConstBufferData pointLights[sPOINTLIGHT_NUM];
		//スポットライト用
		SpotLight::ConstBufferData spotLights[sSPOTLIGHT_NUM];
		//丸影
		CircleShadow::ConstBufferData circleShadows[sCIRCLESHADOW_NUM];
	};

public: // 静的メンバ関数

	//静的初期化
	static void StaticInitialize(ID3D12Device* device);

	
	//インスタンス生成
	void Create();

private: //静的メンバ変数

	//デバイス
	static ID3D12Device* sDevice;

public: //メンバ関数
	
	//初期化
	void Initialize();

	
	//更新
	void Update();

	
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	
	//定数バッファ転送
	void TransferConstBuffer();

	//標準のライト設定
	void DefaultLightSetting();

	//環境光のライト色をセット
	void SetAmbientColor(const XMFLOAT3& color);

	//平行光源の有効フラグをセット
	void SetDirLightActive(int32_t index, bool active);

	//平行光源のライト方向をセット
	void SetDirLightDir(int32_t index, const XMVECTOR& lightdir);

	//平行光源のライト色をセット
	void SetDirLightColor(int32_t index, const XMFLOAT3& lightcolor);

	//点光源
	void SetPointLightActive(int32_t index, bool active);

	void SetPointLightPos(int32_t index, const XMFLOAT3& lightpos);

	void SetPointLightColor(int32_t index, const XMFLOAT3& lightcolor);

	void SetPointLightAtten(int32_t index, const XMFLOAT3& lightatten);


	//スポットライト
	void SetSpotLightActive(int32_t index, bool active);

	void SetSpotLightDir(int32_t index, const XMVECTOR& lightdir);

	void SetSpotLightPos(int32_t index, const XMFLOAT3& lightpos);

	void SetSpotLightColor(int32_t index, const XMFLOAT3& lightcolor);

	void SetSpotLightAtten(int32_t index, const XMFLOAT3& lightAtten);

	void SetSpotLightFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle);


	//丸影
	void SetCircleShadowActive(int32_t index,bool active);

	void SetCircleShadowCasterPos(int32_t index, const XMFLOAT3& casterPos);

	void SetCircleShadowDir(int32_t index, const XMVECTOR& lightdir);

	void SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight);

	void SetCircleShadowAtten(int32_t index, const XMFLOAT3& lightAtten);

	void SetCircleShadowFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle);


private: //メンバ変数

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	//環境光の色
	XMFLOAT3 ambientColor = { 1,1,1 };

	//平行光源の配列
	DirectionalLight dirLights[sDIRLIGHT_NUM];

	//点光源の配列
	PointLight pointLights[sPOINTLIGHT_NUM];

	//スポットライトの配列
	SpotLight spotLights[sSPOTLIGHT_NUM];

	//丸影の配列
	CircleShadow circleShadows[sCIRCLESHADOW_NUM];

	//ダーティフラグ
	bool dirty = false;
};


