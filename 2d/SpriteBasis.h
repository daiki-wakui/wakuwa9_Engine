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

	//SRV�̍ő��
	static const size_t sMaxSRVCount = 2056;

	//�e�N�X�`���o�b�t�@����
	ComPtr<ID3D12Resource> texBuff_ = nullptr;
	std::array<ComPtr<ID3D12Resource>, sMaxSRVCount> textrueBuffers_;

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc_{};

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};

	int32_t texNum_ = -1;
	UINT incrementSize_;

public:
	ID3DBlob* vsBlob_ = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob_ = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob_ = nullptr; // �G���[�I�u�W�F�N�g

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



