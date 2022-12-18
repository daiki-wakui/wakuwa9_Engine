#pragma once
#include "DirectXBasis.h"
#include <string>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXMath.h>
#include <DirectXTex.h>

class SpriteBasis
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result;

	DirectXBasis* dxBasis = nullptr;

	//テクスチャバッファ生成
	ComPtr<ID3D12Resource> texBuff = nullptr;

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

public:
	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	void Initialize(DirectXBasis* dxBasis);
	void Update();
	void Draw();
	void Setting();

	void TextureData();

	void LoadShader();

	DirectXBasis* GetDxBasis() const { return dxBasis; };
	ID3D12RootSignature* GetRootSignature() const { return rootSignature.Get(); };
	ID3D12PipelineState* GetPipelineState() const { return pipelineState.Get(); };
};

