#pragma once
#include "DirectXBasis.h"
#include <string>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <array>
#include <cstdint>

class SpriteBasis
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result_;

	DirectXBasis* dxBasis_ = nullptr;

	//SRVの最大個数
	static const size_t sMaxSRVCount = 2056;

	//テクスチャバッファ生成
	ComPtr<ID3D12Resource> texBuff_ = nullptr;
	std::array<ComPtr<ID3D12Resource>, sMaxSRVCount> textrueBuffers_;

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc_{};

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};

	int32_t texNum_ = -1;
	UINT incrementSize_;

public:
	ID3DBlob* vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob_ = nullptr; // エラーオブジェクト

	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;

	void Initialize(DirectXBasis* dxBasis);
	void TextureSetting();
	void Update();
	void Draw();
	void Setting();

	int32_t TextureData(const wchar_t* name);

	
	UINT GetincrementSize() const { return incrementSize_; };

	void LoadShader();

	DirectXBasis* GetDxBasis() const { return dxBasis_; };
	ID3D12RootSignature* GetRootSignature() const { return rootSignature_.Get(); };
	ID3D12PipelineState* GetPipelineState() const { return pipelineState_.Get(); };
};



