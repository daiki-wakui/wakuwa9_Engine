#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>

class DirectionalLight
{
private:
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv;		//ライトへの方向を表すベクトル
		XMFLOAT3 lightcolor;	//ライトの色
		static bool active;	//有効フラグ
	};

private://静的メンバ変数

	//デバイス
	static ID3D12Device* device;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//ライト光線方向
	XMVECTOR lightdir = { 1,0,0,0 };

	//ライト色
	XMFLOAT3 lightcolor = { 1,1,1 };

	//ダーティフラグ
	bool dirty = false;

public://静的メンバ関数

	//静的初期化
	static void StaticInitalize(ID3D12Device* device);

	static DirectionalLight* Creare();

public://メンバ関数

	//初期化
	void Initalize();

	//更新
	void Update();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	//定数バッファ転送
	void TransferConstBuffer();

	//ライトの方向をセット
	void SetLightDir(const XMVECTOR& lightdir);

	//ライト方向を取得
	XMVECTOR GetLightDir() { return lightdir; };


	XMFLOAT3 GetLightColor() { return lightcolor; };

	//ライトの色をセット
	void SetLightColor(const XMFLOAT3& lightcolor);

	bool active = ConstBufferData::active;	//有効フラグ

	//有効フラグをセット
	inline void SetActive(bool active) { this->active = active; }

	//有効チェック
	inline bool IsActive() { return active; }

};

