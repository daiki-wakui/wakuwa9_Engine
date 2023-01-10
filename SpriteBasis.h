#pragma once
#include "DirectXBasis.h"
#include <string>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXMath.h>
#include <DirectXTex.h>

#include <array>

class SpriteBasis
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result;

	DirectXBasis* dxBasis = nullptr;

	//SRVの最大個数
	static const size_t kMaxSRVCount = 2056;

	//テクスチャバッファ生成
	ComPtr<ID3D12Resource> texBuff = nullptr;
	std::array<ComPtr<ID3D12Resource>, kMaxSRVCount> textrueBuffer_;

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath;

public:
	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	void Initialize(DirectXBasis* dxBasis, uint32_t index);
	void Update();
	void Draw();
	void Setting();

	void TextureData();

	void LoadShader();

	void LoadTexture(uint32_t index, const wchar_t* fileName);
	void SetTextureCommands(uint32_t index);

	/*const uint32_t& GetTexIndex() const { return textureIndex_; };
	void SetTexIndex(const uint32_t& textureIndex) { textureIndex_ = textureIndex; };*/

	DirectXBasis* GetDxBasis() const { return dxBasis; };
	ID3D12RootSignature* GetRootSignature() const { return rootSignature.Get(); };
	ID3D12PipelineState* GetPipelineState() const { return pipelineState.Get(); };
};

