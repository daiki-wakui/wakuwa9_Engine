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

	//�e�N�X�`���o�b�t�@����
	ComPtr<ID3D12Resource> texBuff = nullptr;

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

public:
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

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

